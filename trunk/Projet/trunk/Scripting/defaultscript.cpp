#include "defaultscript.h"

#include "scripthelper.h"

#include <QDir>

using namespace Scripting;

DefaultScript::DefaultScript(bool solve_problem, int mesh_sizes, QString output_path)
	: ComsolScript("temp.default"),
	  solve_problem(solve_problem), mesh_sizes(mesh_sizes), output_path(output_path) {
}

void DefaultScript::write(QTextStream& script, Data::ProjectFile& data) const {
	ScriptHelper::writePath(script, data);

	ScriptHelper::writeMonofinDeclaration(script, data);
	ScriptHelper::writeMonofinLength(script, data);
	ScriptHelper::writeMonofinSegments(script, data);
	ScriptHelper::writeMonofinLayers(script, data);

	writeMonofinSettings(script, data);

	writeMain(script, data);
}

void DefaultScript::writeMonofinSettings(QTextStream& script, Data::ProjectFile& data) const {
	script << "% Paramètres de la palme." << endl;
	script << "%     solve_problem : booléen indiquant si le problème doit être résolu" << endl;
	script << "%     mesh_sizes    : si le problème est à résoudre, indique la finesse du maillage (1 : fin, 9 : grossier, 5 : valeur par défaut)" << endl;
	script << "%     output_path   : chemin du fichier COMSOL (au format mph) que le script va générer" << endl;

	script << "monofin.settings = struct( ..." << endl;
	script << QString("    'solve_problem', %1, ...").arg(solve_problem ? "true" : "false") << endl;
	script << QString("    'mesh_sizes', %1, ...").arg(mesh_sizes) << endl;
	script << QString("    'output_path', '%1');").arg(QDir::toNativeSeparators(output_path)) << endl;

	script << endl;
}

void DefaultScript::writeMain(QTextStream& script, Data::ProjectFile& data) const {
	script << "% Appel du point d'entrée du script." << endl;
	script << "default_main(monofin);" << endl;
}
