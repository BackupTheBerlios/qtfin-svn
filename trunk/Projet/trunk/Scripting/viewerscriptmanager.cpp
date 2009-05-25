#include "viewerscriptmanager.h"
#include "viewerscript.h"

using namespace Scripting;

ViewerScriptManager::ViewerScriptManager(Data::ProjectFile& data)
		: Scripting::ScriptManager(data) {
}

bool ViewerScriptManager::execute(QString output_path) {
	ViewerScript* script = new ViewerScript(output_path);
	bool r = ScriptManager::execute(*script);
	delete script;
	return r;
}
