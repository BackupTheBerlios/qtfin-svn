#ifndef VIEWERSCRIPT_H
#define VIEWERSCRIPT_H

#include "comsolscript.h"

namespace Scripting {

	/**
	  * Class representing the viewer COMSOL script.
	  * The one that generate a picture of the 3D geometry.
	  */
	class ViewerScript : public ComsolScript
	{
	public:

		/**
		  * Default constructor.
		  * @param output_path
		  *		The output_path of the generated image with a 'png' extension.
		  */
		ViewerScript(QString output_path);

		/** Implementation */
		virtual void write(QTextStream& script, Data::ProjectFile& data) const;

		/**
		  * Return the output_path.
		  */
		QString getOutputPath() { return output_path; }
	private:

		/**
		  * Write the settings of the script.
		  */
		void ViewerScript::writeMonofinSettings(QTextStream& script, Data::ProjectFile& data) const;

		/**
		  * Writes the entry point called of the script.
		  */
		void writeMain(QTextStream& script, Data::ProjectFile& data) const;

		/* Settings */

		/** The generated image path. */
		QString output_path;
	};

}

#endif

//  % Déclaration du dossier contenant les scripts utilisés.
//  path('C:\Monofin\Projet\trunk\debug\scripts', path);
//
//  % Initialisation d'une variable.
//  clear monofin;
//
//  % Longueur de l'axe central de la palme en mètre.
//  monofin.length = 25e-2;
//
//  % Courbes de Bézier cubiques (P0, P1, P1, P2).
//  %		x : [P0.x P1.x P2.x], vecteur des X des points P0, P1 et P2.
//  %		y : [P0.y P1.y P2.y], vecteur des Y des points P0, P1 et P2.
//  %
//  % Remarque
//  % 	L'unité dans laquelle sont exprimées les coordonnées est irrévélante.
//  % 	Elle proviennent de l'interface graphique MonoFin et seront ramenées à des mètres
//  %		en respectant la longeur spécifiée dans monofin.length.
//  monofin.segments = [ ...
//    struct('x', [-10 -2 -1], 'y', [0 -65 -98]), ...
//    struct('x', [-1 130 404], 'y', [-98 -248 -280]), ...
//    struct('x', [404 332 400], 'y', [-280 -145 0])];
//
//  % Strates constituant la palme. La première (resp. dernière) strate correspond au dessous (resp. dessus) de la palme.
//  %		thickness : épaisseur de la strate exprimée en mètre
//  %		length 		: longueur de la strate en % de monofin.length
//  %		E 				: module de Young en Pa
//  %		nu 				: coefficient de Poisson
//  %		rho 			: densité en kg/m^3
//  monofin.layers = [ ...
//    struct('thickness', 1e-2, 'length', 1, 'E', 200000000000, 'nu', 0.33, 'rho', 5000), ...
//    struct('thickness', .5e-2, 'length', .9, 'E', 200000000000, 'nu', 0.33, 'rho', 7850), ...
//    struct('thickness', 1e-2, 'length', .8, 'E', 200000000000, 'nu', 0.33, 'rho', 7850)];
//
//  % Paramètres de la palme.
//  %		output_path 	: chemin de l'image (au format png) à générer
//  monofin.settings = struct( ...
//    'output_path', 'C:\Monofin\Projet\trunk\debug\scripts\toto');
//
//  % Appel du point d'entrée du script.
//  viewer_main(monofin);

