#ifndef VIEWERSCRIPTMANAGER_H
#define VIEWERSCRIPTMANAGER_H

#include "scriptmanager.h"

#include "viewerscript.h"

namespace Scripting {

	/**
	  * Implementation of ScriptManager for the viewer script.
	  */
	class ViewerScriptManager : public ScriptManager
	{
	public:
		ViewerScriptManager(Data::ProjectFile& data);

	public slots:
		bool execute(QString output_path);
	};

}

#endif
