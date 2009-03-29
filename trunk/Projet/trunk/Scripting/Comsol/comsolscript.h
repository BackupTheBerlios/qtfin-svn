#ifndef COMSOLSCRIPT_H
#define COMSOLSCRIPT_H

#include "../script.h"

namespace Scripting {
	namespace Comsol {

		class ComsolScript : public Script
		{
		public:
			ComsolScript(Monofin& monofin, std::ostream& script);
			void initWriters(std::vector<ScriptWriter*>* writers) const;
		};

	}
}

#endif // COMSOLSCRIPT_H
