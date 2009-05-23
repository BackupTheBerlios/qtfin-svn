#ifndef VIEWERSCRIPTMANAGER_H
#define VIEWERSCRIPTMANAGER_H

#include "scriptmanager.h"

namespace Scripting {

	/**
	  * Implementation of ScriptManager for the viewer script.
	  */
	class ViewerScriptManager : public ScriptManager
	{
	public:
		ViewerScriptManager(
				// settings
				QString output_path,
				// data
				Data::ProjectFile& data);
	};

}

#endif
