function main(data);
% Point d'entr�e du script g�n�r� par l'interface graphique MonoFin.
%
% Param�tre(s)
%		data : la structure de donn�es g�n�r�es par l'interface graphique MonoFin
%
% Le script effectue les t�ches suivantes :
%		- g�n�ration de la g�om�trie de la palme en 3D
%		- param�trage d'une analyse modale (Eigenfrequency)
%		- �ventuellement le lancement du calcul des fr�quences propres
%		- la g�n�ration d'un fichier binaire COMSOL contenant le r�sultat de l'appel au script


% G�n�ration de la g�om�trie.
data.geometry = build_geometry(data);

% Construction de la structure FEM (Finite Element Method) contenant le param�trage
% de l'analyse modale.
data.fem = build_fem(data);

% G�n�ration du fichier binaire COMSOL de sortie.
fem = data.fem;
flsave(data.settings.output_path, fem);
clear fem
