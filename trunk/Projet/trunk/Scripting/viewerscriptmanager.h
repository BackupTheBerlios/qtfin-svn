#ifndef VIEWERSCRIPTMANAGER_H
#define VIEWERSCRIPTMANAGER_H

#include "scriptmanager.h"

namespace Scripting {

	class ViewerScriptManager : public ScriptManager
	{
	public:
		ViewerScriptManager(QString output_path, Data::ProjectFile& data);
	};

}

#endif
