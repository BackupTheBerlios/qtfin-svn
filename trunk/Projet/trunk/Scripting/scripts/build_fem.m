function fem = build_fem(data);
% Initalise et retourne une structure FEM (Finite Element Method) param�trant une analyse modale (Eigenfrequency).
% La m�thode utilis�e dans COMSOL est 'Structural Mechanics / Solid, Stress-Strain / Eigenfrequency analysis (3D)'.
%
% Param�tre(s)
%		data : la structure de donn�es g�n�r�es par l'interface graphique MonoFin et modifi�e par le script 'main.m'
%
% Sortie(s)
%		fem  : la structure de donn�es reconnue par COMSOL et contenant l'analyse

% Un commentaire marqu� '%% !!' dans ce fichier d�crit une ligne ou un bloc de code qui a �t� modifi�
% pour adapter l'analyse modale � notre cas. Les autres lignes ont �t� g�n�r�es par COMSOL Multiphysics.


flclear fem

% COMSOL version
clear vrsn
vrsn.name = 'COMSOL 3.4';
vrsn.ext = '';
vrsn.major = 0;
vrsn.build = 248;
fem.version = vrsn;

% Analyzed geometry
clear s
s.objs = {data.geometry}; %% !! d�claration de la g�om�trie de palme
s.name = {'monofin'};
s.tags ={'monofin'};

fem.draw = struct('s', s);
fem.geom = geomcsg(fem);

%% !! Application mode 1 : 'Structural Mechanics / Solid, Stress-Strain / Eigenfrequency analysis'
clear appl
appl.mode.class = 'FlSolid3';
appl.dim = {'u2','v2','w2'};
appl.name = 'sld2';
appl.gporder = 4;
appl.cporder = 2;
appl.assignsuffix = '_sld2';
clear prop
prop.analysis = 'eigen';
clear weakconstr
weakconstr.value = 'off';
weakconstr.dim = {'lm4','lm5','lm6'};
prop.weakconstr = weakconstr;
appl.prop = prop;

%% !!
% Fixation des faces de la monopalme en lien avec le chausson.
appl.bnd = get_bnd(data.geometry);
%% !!

%% !!
% Association des propri�t�s physiques aux strates.
appl.equ = get_equ(data);
%% !!

fem.appl{1} = appl;
fem.frame = {'ref'};
fem.border = 1;
clear units;
units.basesystem = 'SI';
fem.units = units;

% ODE Settings
clear ode
clear units;
units.basesystem = 'SI';
ode.units = units;
fem.ode = ode;

% Multiphysics
fem = multiphysics(fem);

%% !!
% Test si le script doit r�soudre le probl�me (lancer la simulation).
if (data.settings.solve_problem)
	% Initialize mesh
	% Le param�tre data.settings.mesh_sizes prend sa valeur entre 1 et 9.
	% Il correpond � un mode de "mesh" pr�configur� dans COMSOL :
	% 	1 : maillage tr�s fin
	% 	5 : maillage par d�faut
	% 	9 : maillage grossier
	fem.mesh = meshinit(fem, 'hauto', data.settings.mesh_sizes);

	% Extend mesh
	fem.xmesh = meshextend(fem);

	% Solve problem
	fem.sol = femeig(fem, ...
    'solcomp', {'u2','v2','w2'}, ...
    'outcomp', {'u2','v2','w2'});
end
%% !!            

% % % % % %
% % % % % %
% % % % % %

function bnd = get_bnd(g);
% Fonction retournant la structure de donn�es n�cessaire dans la structure 'fem' pour fixer les faces (avec des contraintes Fx = Fy = Fz = 0) en
% relation avec le chausson sur la palme.
% On consid�re que toutes les faces qui ont un cot� sur la droite d'�quation Y = 0 (apr�s changement du rep�re de la g�om�trie) sont des faces � figer.
%
% Param�tre(s)
%		g 	: la g�om�trie de la palme
%
% Sortie(s)
%		bnd : la structure 'bnd' fixant les faces du chausson sur la palme


clear bnd

bnd.Hy = {0,1};
bnd.Hx = {0,1};
bnd.Hz = {0,1};

% Initialisation du tableau des indices (avec que des 1 pour le moment).
% Le tableau compte autant d'indices que de face dans la g�om�trie.
% Pour chaque indice, la valeur peut �tre :
%		1 : face libre
%		2 : face fig�e
bnd.ind = ones(1, length(get(g, 'face')));

% Documentation sur les structures bas niveau des g�om�trie dans COMSOL :

% pedge is a 10-by-npe  matrix representing the embeddings of the edges in faces. The 
% first row gives the index of the edge in edge. Rows 2 and 3 contain the start and 
% end vertex indices in pvertex. Rows 4 and 5 give the parameter values of the these 
% vertices. Row 6 and 7 give the indices of the faces to the left and right of the edge, 
% respectively. Row 8 gives a sign and index to the parameter curve (if any), and row 
% 9 gives the index to the surface. Row 10 contains a relative local tolerance for the 
% entity. 

% pvertex is a 6-by-npv matrix containing embeddings of vertices in faces. Row 1 
% contains the vertex index (i.e. column in VERTEX), rows 2 and 3 contain (s, t) 
% coordinates of the vertex on the face, row 4 contains a face index, and row 5 
% contains the manifold index into mfd. Row 6 contains a relative local tolerance for 
% the entity.

% vertex is a 5-by-nv matrix representing the vertices of the 3D geometry. Rows 1, 
% 2, and 3 provide the 3D coordinates of the vertices. Row 4 provides the subdomain 
% number. Row 5 contains a relative local tolerance for the entity. For nontolerant 
% entities the tolerance is NaN.

pedges = get(g, 'pedge');
pvertex = get(g, 'pvertex');
vertex = get(g, 'vertex');

for i=1:length(pedges)
	
	% pedges(2, i) : start vertex indices in pvertex
	% pedges(3, i) : end vertex indices in pvertex
	
	% D�s que l'on a un [x y] = [0 0] quelque part, on a une face (ou deux) � figer.
	
	if all(vertex(1:2, pvertex(1, pedges(2, i)))' == [0 0]) && all(vertex(1:2, pvertex(1, pedges(3, i)))' == [0 0])
		if pedges(6, i) > 0
			bnd.ind(pedges(6, i)) = 2;
		end
		if pedges(7, i) > 0
			bnd.ind(pedges(7, i)) = 2;
		end
	end

end

% % % % % %
% % % % % %
% % % % % %

function equ = get_equ(data);
% Associe � chaque strate ses propri�t�s physiques.
%
% Param�tre(s)
%		data : la structure de donn�es g�n�r�es par l'interface graphique MonoFin et modifi�e par le script 'main.m'
%
% Sortie(s)
%		equ : la structure 'equ' qui affecte � chaque strate ses propri�t�s physiques


clear equ

% It�ration des strates.
% Chaque strate correspond � un sous-domaine dans la g�om�trie.
for i=1:length(data.layers)
  % Affectation des propri�t�s physiques
	equ.nu{i} = data.layers(i).nu;
	equ.E{i} = data.layers(i).E;
	equ.rho{i} = data.layers(i).rho;
		
	% Affectation de l'identifiant du sous-domaine.
	equ.ind(i) = i;

	% Attention
	%		Ici, on a toujours les indices des strates dans 'data.layers' qui correspondent avec les indices
	% 	de sous-domaine dans la g�om�trie. Les tests ont montr� que ce n'est pas un hasard. Cependant,
	%		il pourrait �tre souhaitable plus tard de r�cup�rer l'identifiant d'un sous-domaine en analysant
	%		la g�om�trie dynamiquement.
end
