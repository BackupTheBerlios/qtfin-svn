#include "defaultscriptmanager.h"

#include "defaultscript.h"

using namespace Scripting;

DefaultScriptManager::DefaultScriptManager(
		bool solve_problem,
		int mesh_sizes,
		QString output_path,
		Data::ProjectFile& data)
		: Scripting::ScriptManager(new Scripting::DefaultScript(solve_problem, mesh_sizes, output_path),
		  data) {
}
