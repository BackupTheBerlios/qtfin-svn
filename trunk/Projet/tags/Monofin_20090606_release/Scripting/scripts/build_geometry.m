function m = build_geometry(data);
% Fonction de construction de la g�om�trie 3D de la palme.
%
% Param�tre(s)
%		data : la structure de donn�es g�n�r�es par l'interface graphique MonoFin
%
% R�sultat(s)
%		m		 : la g�om�trie en 3D, un objet h�ritant de 'geom3'


% Tableau qui va accueillir les courbes de B�zier d�crivant le contour de la palme.
curves = {};

% It�ration sur les courbes d�crivant la surface contenue dans 'data'.
for i=1:length(data.segments)
	s = data.segments(i);
	
	% Remarque
	%		La structure 'data' ne contient que la moiti� du contour de la palme.
	%		L'autre moiti� est reconstruite par sym�trie selon l'axe Y.
	
	% Courbe positive (au dessus de l'axe Y).
	curves{i*2-1} = curve2( ...
		[s.x(1) s.x(2) s.x(2) s.x(3)], ...
		[s.y(1) s.y(2) s.y(2) s.y(3)]);
	
	% Courbe n�gative (le reflet au dessous de l'axe Y).
	curves{i*2} = curve2( ...
		[s.x(1) s.x(2) s.x(2) s.x(3)], ...
		[-s.y(1) -s.y(2) -s.y(2) -s.y(3)]);
end

% s = geomcsg(curves);

% Assemblage des courbes de B�zier en un solide (h�ritant de 'solid2') repr�sentant la surface de la palme.
s = geomcoerce('solid', curves);

% Suppression des bordures internes � l'objet.
% s = geomdel(s);

% Calcul de la longueur de la palme sur son axe central de sym�trie.
% Cette valeur d�finie la longueur maximale d'une strate constituant la palme.
L = data.metaSegments.absoluteLength;

% Avant, on utilisait l'ordre naturel des segments. La structure de donn�es ne garantie pas ce comportement...
% L = data.segments(length(data.segments)).x(3) - data.segments(1).x(1);

% Translation de l'origine de la palme.
dX = data.metaSegments.dX;
dY = data.metaSegments.dY;
% dX = data.segments(1).x(1);
% dY = data.segments(1).y(1);

s = move(s, -dX, -dY);

% Mise � l'�chelle de la palme.
s = scale(s, data.length/L, data.length/L);

% Position z actuelle : utilis�e pour l'empilement des strates.
z = 0;

% Cr�ation des strates (dans un cell array).
layers = {};
% for i=1:length(data.layers)
for i=length(data.layers):-1:1
	st = data.layers(i);

	% Duplication de la surface de la palme dans s1.
	s1 = s;

	% Si la strate en a besoin, on la d�coupe.
	if (st.length < 1)
		% Duplication de la surface s1 avec d�callage vers la direction -X.
		s2 = move(s1, - (1 - st.length) * data.length, 0);

		% R�cup�ration des solides r�sultant de la diff�rence entre les deux surfaces.
		leftovers = split(s1 - s2);
		
		% Suppression des r�sidus li�s � la diff�rence bool�enne.
		% Tout objet situ� � droite (enti�rement) de la droite d'�quation Y = st.length * xlength est consid�r�
		% comme un r�sidu.
		leftovers = filter_leftovers(leftovers, st.length * data.length);
		
		% S'il reste des r�sidus suite au filtre, on peut d�couper la surface.
		if (length(leftovers) > 0)
			s1 = s1 - geomcoerce('solid', leftovers);
		end
	end

	% Extrusion de la surface en une strate solide. Passage d'un objet de type 'solid2' � 'solid3'.
	% On donne � la strate son �paisseur souhait�e.
	s1 = extrude(s1, 'distance', st.thickness);

	% Positionnement de la strate dans l'espace (en hauteur selon Z).
	layers{i} = move(s1, 0, 0, z);
	
	% Pr�pare le positionnement de la prochaine strate.
	z = z + st.thickness;
end

% Retourne l'assemblage de toutes les strates dans un objet de type 'solid3'.
m = geomcoerce('solid', layers);

% % % % % %
% % % % % %
% % % % % %

function l = filter_leftovers(leftovers, xLimit);
% Filtre une liste d'objets de type 'solid2' et ne conserve que ceux qui sont � gauche de la droite d'�quation Y = xLimit.
%
% Param�tre(s)
%		leftovers : une liste d'objet de type 'solid2' � filtrer
%		xLimit		: une valeur d�crivant une droite d'�quation Y = xLimit
%
% Sortie(s)
%		l 				: une liste contenant les objets r�sidu


% Initialisation de la liste des r�sidus.
l = {};

% Index du prochain �l�ment � ins�rer dans la liste.
id = 1;

% Parcours tous les r�sidus.
for i=1:length(leftovers)
	% R�cup�re les coordonn�es des points composant l'objet r�sidu.
	x = get(leftovers{i}, 'mp');
  
	% On ne garde pas les solid2 �tant � gauche de la droite d'�quation Y = xLimit.
	% x(1,:) retourne le vecteur ligne contenant uniquement la composante X des coordon�es des points.
	if not(all(x(1,:) < xLimit))
		% Ajout du r�sidu � la liste.
		l{id} = leftovers{i};
		id = id + 1;
	end
end
