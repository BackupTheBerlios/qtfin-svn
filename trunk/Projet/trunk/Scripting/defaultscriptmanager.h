#ifndef DEFAULTSCRIPTMANAGER_H
#define DEFAULTSCRIPTMANAGER_H

#include "scriptmanager.h"
#include "../Data/projectfile.h"

namespace Scripting {

	class DefaultScriptManager : public ScriptManager
	{
	public:
		DefaultScriptManager(bool solve_problem, int mesh_sizes, QString output_path, Data::ProjectFile& data);
	};

}

#endif
