#include "viewerscript.h"

#include "scripthelper.h"

#include <QDir>

using namespace Scripting;

ViewerScript::ViewerScript(QString output_path)
		: output_path(output_path) {
}

void ViewerScript::write(QTextStream& script, Data::ProjectFile& data) const
{
	ScriptHelper::writePath(script, data);

	ScriptHelper::writeMonofinDeclaration(script, data);
	ScriptHelper::writeMonofinLength(script, data);
	ScriptHelper::writeMonofinSegments(script, data);
	ScriptHelper::writeMonofinLayers(script, data);

	writeMonofinSettings(script, data);

	writeMain(script, data);
	ScriptHelper::writeEOS(script);
}

void ViewerScript::writeMonofinSettings(QTextStream& script, Data::ProjectFile& data) const
{
	script
			<< QString("monofin.settings = struct('output_path', '%1');")
			   .arg(QDir::toNativeSeparators(output_path))
			<< endl;
}

void ViewerScript::writeMain(QTextStream& script, Data::ProjectFile& data) const
{
	script
			<< "main_viewer(monofin);"
			<< endl;
}
