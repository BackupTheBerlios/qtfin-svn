#ifndef SCRIPT_H
#define SCRIPT_H

#include "../Data/monofinfile.h"
#include "scriptwriter.h"

#include <ostream>
#include <vector>

namespace Scripting {

	class Script
	{
	private:
		Monofin& monofin;
		std::ostream& script;

	public:
		Script(Monofin& monofin, std::ostream& script);
		virtual void initWriters(std::vector<ScriptWriter*>* writers) const = 0;
		void build() const;
	};

}

#endif // SCRIPT_H
