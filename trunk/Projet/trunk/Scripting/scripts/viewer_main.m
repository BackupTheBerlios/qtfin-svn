function viewer_main(data);
% Point d'entr�e du script g�n�r� par l'interface graphique MonoFin pour g�n�rer une image de la g�om�trie.
%
% Param�tre(s)
%		data : la structure de donn�es g�n�r�es par l'interface graphique MonoFin
%
% Le script effectue les t�ches suivantes :
%		- g�n�ration de l'image
%		- sauvegarde de l'image


% G�n�ration de la g�om�trie.
data.geometry = build_geometry(data);

% G�n�ration de l'image.
geomplot(data.geometry);

% Sauvegarde de l'image.
saveimage(data.settings.output_path, 'type', 'png', 'width', 800, 'height', 600, 'unit', 'pixel');