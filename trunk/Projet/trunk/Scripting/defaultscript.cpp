#include "defaultscript.h"

#include "scripthelper.h"

#include <QDir>

using namespace Scripting;

DefaultScript::DefaultScript(bool solve_problem, int mesh_sizes, QString output_path)
	: solve_problem(solve_problem), mesh_sizes(mesh_sizes), output_path(output_path) {
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

void DefaultScript::writeMonofinSettings(QTextStream& script, Data::ProjectFile& data) const
{
	QString settings = QString("monofin.settings = struct('solve_problem', %1, 'mesh_sizes', %2, 'output_path', '%3');");
	settings = settings
			   .arg(solve_problem ? "true" : "false")
			   .arg(mesh_sizes)
			   .arg(QDir::toNativeSeparators(output_path));

	script << settings << endl;
	script << endl;
}

void DefaultScript::writeMain(QTextStream& script, Data::ProjectFile& data) const
{
	script << "main_default(monofin);" << endl;
}
