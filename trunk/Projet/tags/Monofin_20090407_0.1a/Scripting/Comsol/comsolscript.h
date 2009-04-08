#ifndef COMSOLSCRIPT_H
#define COMSOLSCRIPT_H

#include "../script.h"

namespace Scripting {
	namespace Comsol {

		/**
		  * Définition du script COMSOL "classique".
		  */
		class ComsolScript : public Script {
		public:
			// Redéfinitions
            ComsolScript(Data::MonofinFile& monofin, QTextStream& script);
			void initWriters(std::vector<ScriptWriter*>* writers) const;
		};

	}
}

#endif // COMSOLSCRIPT_H
