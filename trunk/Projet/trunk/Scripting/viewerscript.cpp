#include "viewerscript.h"

#include "scripthelper.h"

#include <QDir>

using namespace Scripting;

ViewerScript::ViewerScript(QString output_path)
		: ComsolScript("temp.viewer"),
		  output_path(output_path) {
}

void ViewerScript::write(QTextStream& script, Data::ProjectFile& data) const {
	ScriptHelper::writePath(script, data);

	ScriptHelper::writeMonofinDeclaration(script, data);
	ScriptHelper::writeMonofinLength(script, data);
	ScriptHelper::writeMonofinSegments(script, data);
	ScriptHelper::writeMonofinLayers(script, data);

	writeMonofinSettings(script, data);

	writeMain(script, data);
}

void ViewerScript::writeMonofinSettings(QTextStream& script, Data::ProjectFile& data) const {
	script << "% Paramètres de la palme." << endl;
	script << "%     output_path   : chemin de l'image (au format png) à générer" << endl;

	script << "monofin.settings = struct( ..." << endl;
	script << QString("    'output_path', '%1');").arg(QDir::toNativeSeparators(output_path)) << endl;

	script << endl;
}

void ViewerScript::writeMain(QTextStream& script, Data::ProjectFile& data) const {
	script << "% Appel du point d'entrée du script." << endl;
	script << "viewer_main(monofin);" << endl;
}
