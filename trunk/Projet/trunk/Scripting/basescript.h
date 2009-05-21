#ifndef SCRIPT_H
#define SCRIPT_H

#include "../Data/monofinfile.h"
#include <qtextstream>

namespace Scripting {

	/** Abstract class that represent a script. */
	class BaseScript {
	public:
		/**
		  * Default constructor.
		  * @param monofin A data instance.
		  * @param script A stream instance.
		  */
		BaseScript(Data::MonofinFile& monofin, QTextStream& script);

		/** Start the script generation. */
		virtual void build() const = 0;

	protected:
		/** Data from which generate the script. */
		Data::MonofinFile& monofin;

		/** QTextStream in which the script is written. */
		QTextStream& script;

		/** Append a comment token in the stream before returning it. */
		static QTextStream& comment(QTextStream& stream) { return stream << "% "; }

		/** Append an escaped line token in the stream before returning it. */
		static QTextStream& eendl(QTextStream& stream) { return stream << " ..." << endl; }
	};

}

#endif
