function m = build_geometry(data);
% Fonction de construction de la géométrie 3D de la palme.
%
% Paramètre(s)
%		data : la structure de données générées par l'interface graphique MonoFin
%
% Résultat(s)
%		m		 : la géométrie en 3D, un objet héritant de 'geom3'


% Tableau qui va accueillir les courbes de Bézier décrivant le contour de la palme.
curves = {};

% Itération sur les courbes décrivant la surface contenue dans 'data'.
for i=1:length(data.segments)
	s = data.segments(i);
	
	% Remarque
	%		La structure 'data' ne contient que la moitié du contour de la palme.
	%		L'autre moitié est reconstruite par symétrie selon l'axe Y.
	
	% Courbe positive (au dessus de l'axe Y).
	curves{i*2-1} = curve2( ...
		[s.x(1) s.x(2) s.x(2) s.x(3)], ...
		[s.y(1) s.y(2) s.y(2) s.y(3)]);
	
	% Courbe négative (le reflet au dessous de l'axe Y).
	curves{i*2} = curve2( ...
		[s.x(1) s.x(2) s.x(2) s.x(3)], ...
		[-s.y(1) -s.y(2) -s.y(2) -s.y(3)]);
end

% s = geomcsg(curves);

% Assemblage des courbes de Bézier en un solide (héritant de 'solid2') représentant la surface de la palme.
s = geomcoerce('solid', curves);

% Suppression des bordures internes à l'objet.
% s = geomdel(s);

% Calcul de la longueur de la palme sur son axe central de symétrie.
% Cette valeur définie la longueur maximale d'une strate constituant la palme.
L = data.metaSegments.absoluteLength;

% Avant, on utilisait l'ordre naturel des segments. La structure de données ne garantie pas ce comportement...
% L = data.segments(length(data.segments)).x(3) - data.segments(1).x(1);

% Translation de l'origine de la palme.
dX = data.metaSegments.dX;
dY = data.metaSegments.dY;
% dX = data.segments(1).x(1);
% dY = data.segments(1).y(1);

s = move(s, -dX, -dY);

% Mise à l'échelle de la palme.
s = scale(s, data.length/L, data.length/L);

% Position z actuelle : utilisée pour l'empilement des strates.
z = 0;

% Création des strates (dans un cell array).
layers = {};
% for i=1:length(data.layers)
for i=length(data.layers):-1:1
	st = data.layers(i);

	% Duplication de la surface de la palme dans s1.
	s1 = s;

	% Si la strate en a besoin, on la découpe.
	if (st.length < 1)
		% Duplication de la surface s1 avec décallage vers la direction -X.
		s2 = move(s1, - (1 - st.length) * data.length, 0);

		% Récupération des solides résultant de la différence entre les deux surfaces.
		leftovers = split(s1 - s2);
		
		% Suppression des résidus liés à la différence booléenne.
		% Tout objet situé à droite (entièrement) de la droite d'équation Y = st.length * xlength est considéré
		% comme un résidu.
		leftovers = filter_leftovers(leftovers, st.length * data.length);
		
		% S'il reste des résidus suite au filtre, on peut découper la surface.
		if (length(leftovers) > 0)
			s1 = s1 - geomcoerce('solid', leftovers);
		end
	end

	% Extrusion de la surface en une strate solide. Passage d'un objet de type 'solid2' à 'solid3'.
	% On donne à la strate son épaisseur souhaitée.
	s1 = extrude(s1, 'distance', st.thickness);

	% Positionnement de la strate dans l'espace (en hauteur selon Z).
	layers{i} = move(s1, 0, 0, z);
	
	% Prépare le positionnement de la prochaine strate.
	z = z + st.thickness;
end

% Retourne l'assemblage de toutes les strates dans un objet de type 'solid3'.
m = geomcoerce('solid', layers);

% % % % % %
% % % % % %
% % % % % %

function l = filter_leftovers(leftovers, xLimit);
% Filtre une liste d'objets de type 'solid2' et ne conserve que ceux qui sont à gauche de la droite d'équation Y = xLimit.
%
% Paramètre(s)
%		leftovers : une liste d'objet de type 'solid2' à filtrer
%		xLimit		: une valeur décrivant une droite d'équation Y = xLimit
%
% Sortie(s)
%		l 				: une liste contenant les objets résidu


% Initialisation de la liste des résidus.
l = {};

% Index du prochain élément à insérer dans la liste.
id = 1;

% Parcours tous les résidus.
for i=1:length(leftovers)
	% Récupère les coordonnées des points composant l'objet résidu.
	x = get(leftovers{i}, 'mp');
  
	% On ne garde pas les solid2 étant à gauche de la droite d'équation Y = xLimit.
	% x(1,:) retourne le vecteur ligne contenant uniquement la composante X des coordonées des points.
	if not(all(x(1,:) < xLimit))
		% Ajout du résidu à la liste.
		l{id} = leftovers{i};
		id = id + 1;
	end
end
