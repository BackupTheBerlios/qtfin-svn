#include "comsolscript.h"

#include <QDir>
#include <QString>

using namespace Scripting;

ComsolScript::ComsolScript(Data::MonofinFile& monofin, QTextStream& script)
	: BaseScript(monofin, script) {
}

void ComsolScript::build() const {
	QString path = QString("%1/Scripts").arg(QDir::currentPath());
	script << comment << "Déclaration du dossier contenant les scripts utilisés." << endl;
	script << QString("path('%1');").arg(QDir::toNativeSeparators(path)) << endl;
	script << endl;

	script << comment << "Initialisation d'une variable." << endl;
	script << "clear monofin;" << endl;
	script << endl;

	script << comment << "Longueur de l'axe central de la palme en mètre." << endl;
	script << QString("monofin.length = %1;").arg(0) << endl; // TODO: récupérer la longueur dans la sdd
	script << endl;

	writeSegments();

	writeLayers();

	writeSettings();

	script << comment << "Appel du point d'entrée du script." << endl;
	script << "main(monofin);" << endl;
}

void ComsolScript::writeSegments() const {
	script << comment << "Courbes de Bézier cubiques (P0, P1, P1, P2)." << endl;
	script << comment << "    x : [P0.x P1.x P2.x], vecteur des X des points P0, P1 et P2." << endl;
	script << comment << "    y : [P0.y P1.y P2.y], vecteur des Y des points P0, P1 et P2." << endl;
	script << comment << endl;
	script << comment << "Remarque" << endl;
	script << comment << "    L'unité dans laquelle sont exprimées les coordonnées est irrévélante." << endl;
	script << comment << "    Elle proviennent de l'interface graphique MonoFin et seront ramenées à des mètres" << endl;
	script << comment << "    en respectant la longeur spécifiée dans monofin.length." << endl;
	script << endl;

	// TODO : dumper à partir de la sdd
}

void ComsolScript::writeLayers() const {
	script << comment << "Strates constituant la palme. La première (resp. dernière) strate correspond au dessous (resp. dessus) de la palme." << endl;
	script << comment << "    thickness : épaisseur de la strate exprimée en mètre" << endl;
	script << comment << "    length    : longueur de la strate en % de monofin.length" << endl;
	script << comment << "    E         : module de Young en Pa" << endl;
	script << comment << "    nu        : coefficient de Poisson" << endl;
	script << comment << "    rho       : densité en kg/m^3" << endl;
	script << endl;

	// TODO : dumper à partir de la sdd
}

void ComsolScript::writeSettings() const {
	script << comment << "Paramètres de la palme." << endl;
	script << comment << "    solve_problem : booléen indiquant si le problème doit être résolu" << endl;
	script << comment << "    mesh_sizes    : si le problème est à résoudre, indique la finesse du maillage (1 : fin, 9 : grossier, 5 : valeur par défaut)" << endl;
	script << comment << "    output_path   : chemin du fichier COMSOL (au format mph) que le script va générer" << endl;
	script << endl;
}
