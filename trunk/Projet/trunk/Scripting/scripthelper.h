#ifndef SCRIPTHELPER_H
#define SCRIPTHELPER_H

#include "../Data/projectfile.h"
#include "scriptmanager.h"
#include <QTextStream>
#include <QDir>

namespace Scripting {

	class ScriptHelper {
	public:
		static void writePath(QTextStream& script, Data::ProjectFile& data);
		static void writeMonofinDeclaration(QTextStream& script, Data::ProjectFile& data);
		static void writeMonofinLength(QTextStream& script, Data::ProjectFile& data);
		static void writeMonofinSegments(QTextStream& script, Data::ProjectFile& data);
		static void writeMonofinLayers(QTextStream& script, Data::ProjectFile& data);
	};

}

#endif
