#ifndef SCRIPTWRITER_H
#define SCRIPTWRITER_H

#include "../Data/monofinfile.h"
#include <ostream>
#include <string>

namespace Scripting {

	class ScriptWriter
	{
	public:
		virtual void write(Monofin& monofin, std::ostream& script) = 0;
		static std::ostream& comment(std::ostream& stream);
		static std::ostream& eendl(std::ostream& stream);
	};

}

#endif // SCRIPTWRITER_H
