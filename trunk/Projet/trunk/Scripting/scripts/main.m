function main(data);
% Point d'entrée du script généré par l'interface graphique MonoFin.
%
% Paramètre(s)
%		data : la structure de données générées par l'interface graphique MonoFin
%
% Le script effectue les tâches suivantes :
%		- génération de la géométrie de la palme en 3D
%		- paramétrage d'une analyse modale (Eigenfrequency)
%		- éventuellement le lancement du calcul des fréquences propres
%		- la génération d'un fichier binaire COMSOL contenant le résultat de l'appel au script


% Génération de la géométrie.
data.geometry = build_geometry(data);

% Construction de la structure FEM (Finite Element Method) contenant le paramétrage
% de l'analyse modale.
data.fem = build_fem(data);

% Génération du fichier binaire COMSOL de sortie.
fem = data.fem;
flsave(data.settings.output_path, fem);
clear fem
