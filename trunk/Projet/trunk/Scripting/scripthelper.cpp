#include "scripthelper.h"

using namespace Scripting;

void ScriptHelper::writePath(QTextStream& script, Data::ProjectFile& data) {
	script << "% Déclaration du dossier contenant les scripts utilisés." << endl;
	script << QString("path('%1', path);").arg(QDir::toNativeSeparators(ScriptManager::ScriptDirectory)) << endl;
	script << endl;
}

void ScriptHelper::writeMonofinDeclaration(QTextStream& script, Data::ProjectFile& data) {
	script << "% Initialisation de la variable 'monofin'." << endl;
	script << "clear monofin;" << endl;
	script << endl;
}

// TODO: récupérer la longueur dans la sdd
void ScriptHelper::writeMonofinLength(QTextStream& script, Data::ProjectFile& data) {
	script << "% Longueur de l'axe central de la palme en mètre." << endl;
	script << QString("monofin.length = %1;").arg(0.025) << endl;
	script << endl;
}

void ScriptHelper::writeMonofinSegments(QTextStream& script, Data::ProjectFile& data) {
	script << "% Courbes de Bézier cubiques (P0, P1, P1, P2)." << endl;
	script << "%     x : [P0.x P1.x P2.x], vecteur des X des points P0, P1 et P2." << endl;
	script << "%     y : [P0.y P1.y P2.y], vecteur des Y des points P0, P1 et P2." << endl;
	script << "% " << endl;
	script << "% Remarque" << endl;
	script << "%     L'unité dans laquelle sont exprimées les coordonnées est irrévélante." << endl;
	script << "%     Elle proviennent de l'interface graphique MonoFin et seront ramenées à des mètres" << endl;
	script << "%     en respectant la longeur spécifiée dans monofin.length." << endl;

	script << "monofin.segments = [ ..." << endl;
	for (int i = 0; i < data.getAllSegmentKeys().count(); i++) {
		// keys
		int segKey = data.getAllSegmentKeys().at(i);
		int aKey, bKey, cKey;

		// coordinates
		float aX, aY, bX, bY, cX, cY;

		data.getSegment(segKey, aKey, cKey, bKey);
		data.getIntersectionPoint(aKey, aX, aY);
		data.getControlPoint(bKey, bX, bY);
		data.getIntersectionPoint(cKey, cX, cY);

		script <<
				QString("struct('x', [%1 %2 %3], 'y', [%4 %5 %6])")
					.arg(aX)
					.arg(bX)
					.arg(cX)
					.arg(aY)
					.arg(bY)
					.arg(cY);

		if (i < data.getAllSegmentKeys().count() - 1) {
			script << ", ..." << endl;
		}
	}

	script << "];" << endl << endl;
}

void ScriptHelper::writeMonofinLayers(QTextStream& script, Data::ProjectFile& data) {
	script << "% Strates constituant la palme. La première (resp. dernière) strate correspond au dessous (resp. dessus) de la palme." << endl;
	script << "%     thickness : épaisseur de la strate exprimée en mètre" << endl;
	script << "%     length    : longueur de la strate en % de monofin.length" << endl;
	script << "%     E         : module de Young en Pa" << endl;
	script << "%     nu        : coefficient de Poisson" << endl;
	script << "%     rho       : densité en kg/m^3" << endl;

	script << "monofin.layers = [ ..." << endl;

	for (int i = 0; i < data.getHowManyLayers(); i++) {
		script << QString(
				"    struct('thickness', %1, 'length', %2, 'E', %3, 'nu', %4, 'rho', %5)")
					.arg(data.getLayerHeight(i))
					.arg(data.getLayerLength(i))
					.arg(data.getLayerConfigYoung(i))
					.arg(data.getLayerConfigPoisson(i))
					.arg(data.getLayerConfigRho(i));

		if (i < data.getHowManyLayers() - 1) {
			script << ", ..." << endl;
		}
	}

	script << "];" << endl << endl;
}
