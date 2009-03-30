#ifndef DUMMYWRITER_H
#define DUMMYWRITER_H

#include "comsolscriptwriter.h"

namespace Scripting {
	namespace Comsol {

		class DummyWriter : public ComsolScriptWriter {
		public:
			// Red�finition
			virtual void write(Monofin& monofin, QTextStream& script);
		};

	}
}

#endif // DUMMYWRITER_H
