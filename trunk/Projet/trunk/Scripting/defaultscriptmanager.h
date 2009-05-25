#ifndef DEFAULTSCRIPTMANAGER_H
#define DEFAULTSCRIPTMANAGER_H

#include "scriptmanager.h"

#include "../Data/projectfile.h"

namespace Scripting {

	/**
	  * Implementation of ScriptManager for the default script.
	  */
	class DefaultScriptManager : public ScriptManager
	{
	public:
		/**
		  * Default constructor.
		  * @see DefaultScript
		  */
		DefaultScriptManager(Data::ProjectFile& data);

	public slots:
		bool execute(bool solve_problem, int mesh_sizes, QString output_path);
	};

}

#endif
