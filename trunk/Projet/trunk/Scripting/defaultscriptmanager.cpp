#include "defaultscriptmanager.h"

#include "defaultscript.h"

using namespace Scripting;

DefaultScriptManager::DefaultScriptManager(Data::ProjectFile& data)
		: ScriptManager(data) {
}

bool DefaultScriptManager::execute(bool solve_problem, int mesh_sizes, QString output_path) {
	DefaultScript* script = new DefaultScript(solve_problem, mesh_sizes, output_path);
	bool r = ScriptManager::execute(*script);
	delete script;
	return r;
}
