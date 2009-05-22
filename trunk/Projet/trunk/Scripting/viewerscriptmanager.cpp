#include "viewerscriptmanager.h"
#include "viewerscript.h"

using namespace Scripting;

ViewerScriptManager::ViewerScriptManager(QString output_path, Data::ProjectFile& data)
		: Scripting::ScriptManager(new Scripting::ViewerScript(output_path), data) {
}
