#ifndef COMSOLSCRIPT_H
#define COMSOLSCRIPT_H

#include "comsolscript.h"

namespace Scripting {

	class DefaultScript : public ComsolScript
	{
	public:
		DefaultScript(bool solve_problem, int mesh_sizes, QString output_path);
		virtual void write(QTextStream& script, Data::ProjectFile& data) const;

	private:
		void writeMonofinSettings(QTextStream& script, Data::ProjectFile& data) const;
		void writeMain(QTextStream& script, Data::ProjectFile& data) const;

		/* Settings */

		bool solve_problem;
		int mesh_sizes;
		QString output_path;
	};

}
#endif
