#ifndef DUMMYWRITER_H
#define DUMMYWRITER_H

#include "../scriptwriter.h"

namespace Scripting {
	namespace Comsol {

		class DummyWriter : public ScriptWriter
		{
		public:
			virtual void write(Monofin& monofin, std::ostream& script);
		};

	}
}

#endif // DUMMYWRITER_H
