function viewer_main(data);
% Point d'entrée du script généré par l'interface graphique MonoFin pour générer une image de la géométrie.
%
% Paramètre(s)
%		data : la structure de données générées par l'interface graphique MonoFin
%
% Le script effectue les tâches suivantes :
%		- génération de l'image
%		- sauvegarde de l'image


% Génération de la géométrie.
data.geometry = build_geometry(data);

% Génération de l'image.
geomplot(data.geometry);

% Sauvegarde de l'image.
saveimage(data.settings.output_path, 'type', 'png', 'width', 800, 'height', 600, 'unit', 'pixel');