#ifndef COMSOLSCRIPT_H
#define COMSOLSCRIPT_H

#include "comsolscript.h"

namespace Scripting {

	/**
	  * Class representing the default COMSOL script.
	  * The one that run the simulation.
	  */
	class DefaultScript : public ComsolScript
	{
	public:

		/**
		  * Default constructor.
		  * @param solve_problem
		  *		Flag indicating if the script must solve the problem (run the simulation).
		  * @param mesh_sizes
		  *		An integer to describe the mesh sizes to use during the mesh process.
		  *		Only revelant if solve_problem is set to true.
		  *		The value range is between 1 and 9 with a default of 5.
		  *			1 : fine mesh sizes
		  *			X : more and more coerced
		  *			9 : coerce mesh sizes
		  */
		DefaultScript(bool solve_problem, int mesh_sizes, QString output_path);

		/** Implementation. */
		virtual void write(QTextStream& script, Data::ProjectFile& data) const;

	private:

		/**
		  * Writes the settings of the script.
		  */
		void writeMonofinSettings(QTextStream& script, Data::ProjectFile& data) const;

		/**
		  * Writes the entry point called of the script.
		  */
		void writeMain(QTextStream& script, Data::ProjectFile& data) const;

		/* Settings */

		/** @see DefaultScript */
		bool solve_problem;

		/** @see DefaultScript */
		int mesh_sizes;

		/** @see DefaultScript */
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
//  %		solve_problem : booléen indiquant si le problème doit être résolu
//  %		mesh_sizes		: si le problème est à résoudre, indique la finesse du maillage (1 : fin, 9 : grossier, 5 : valeur par défaut)
//  %		output_path 	: chemin du fichier COMSOL (au format mph) que le script va générer
//  monofin.settings = struct( ...
//    'solve_problem', false, ...
//    'mesh_sizes', 5, ...
//    'output_path', 'C:\Monofin\Projet\trunk\debug\scripts\toto');
//
//  % Appel du point d'entrée du script.
//  default_main(monofin);
