% Déclaration du dossier contenant les scripts utilisés.
path('D:\Projects\PSurfaceDesigner\Release\Scripts', path);

% Initialisation d'une variable.
clear monofin;

% Longueur de l'axe central de la palme en mètre.
monofin.length = 25e-2;

% Courbes de Bézier cubiques (P0, P1, P1, P2).
%		x : [P0.x P1.x P2.x], vecteur des X des points P0, P1 et P2.
%		y : [P0.y P1.y P2.y], vecteur des Y des points P0, P1 et P2.
% 
% Remarque
% 	L'unité dans laquelle sont exprimées les coordonnées est irrévélante.
% 	Elle proviennent de l'interface graphique MonoFin et seront ramenées à des mètres
%		en respectant la longeur spécifiée dans monofin.length.
monofin.segments = [ ...
	struct('x', [-10 -2 -1], 'y', [0 -65 -98]), ... 
	struct('x', [-1 130 404], 'y', [-98 -248 -280]), ... 
	struct('x', [404 332 400], 'y', [-280 -145 0])];

% Strates constituant la palme. La première (resp. dernière) strate correspond au dessous (resp. dessus) de la palme.
%		thickness : épaisseur de la strate exprimée en mètre
%		length 		: longueur de la strate en % de monofin.length
%		E 				: module de Young en Pa
%		nu 				: coefficient de Poisson
%		rho 			: densité en kg/m^3
monofin.layers = [ ...
	struct('thickness', 1e-2, 'length', 1, 'E', 200000000000, 'nu', 0.33, 'rho', 5000), ... 
	struct('thickness', .5e-2, 'length', .9, 'E', 200000000000, 'nu', 0.33, 'rho', 7850), ... 
	struct('thickness', 1e-2, 'length', .8, 'E', 200000000000, 'nu', 0.33, 'rho', 7850)];

% Paramètres de la palme.
%		solve_problem : booléen indiquant si le problème doit être résolu
%		mesh_sizes		: si le problème est à résoudre, indique la finesse du maillage (1 : fin, 9 : grossier, 5 : valeur par défaut)
%		output_path 	: chemin du fichier COMSOL (au format mph) que le script va générer
monofin.settings = struct( ...
  'solve_problem', true, ...
  'mesh_sizes', 5, ...
  'output_path', 'D:\Projects\PSurfaceDesigner\Release\Scripts\temp');

% Appel du point d'entrée du script.
main(monofin);
