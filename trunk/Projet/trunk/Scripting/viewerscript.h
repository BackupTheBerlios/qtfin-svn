#ifndef VIEWERSCRIPT_H
#define VIEWERSCRIPT_H

#include "comsolscript.h"

namespace Scripting {

	class ViewerScript : public ComsolScript
	{
	public:
		ViewerScript(QString output_path);
		virtual void write(QTextStream& script, Data::ProjectFile& data) const;

	private:
		void ViewerScript::writeMonofinSettings(QTextStream& script, Data::ProjectFile& data) const;
		void writeMain(QTextStream& script, Data::ProjectFile& data) const;

		/* Settings */

		QString output_path;
	};

}

#endif // VIEWERSCRIPT_H
