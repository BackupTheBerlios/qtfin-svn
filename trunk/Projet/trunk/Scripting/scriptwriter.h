#ifndef SCRIPTWRITER_H
#define SCRIPTWRITER_H

#include "../Data/monofinfile.h"
#include <qtextstream.h>
#include <string>

namespace Scripting {

	/**
	  * Classe abstraite responsable de l'�criture d'une partie d'un script.
	  */
	class ScriptWriter {
	public:
		/**
		  * M�thode appel�e par une classe h�ritant de Script au moment de la g�n�ration
		  * d'une portion du script.
		  */
        virtual void write(Data::MonofinFile& monofin, QTextStream& script) = 0;
	};

}

#endif // SCRIPTWRITER_H
