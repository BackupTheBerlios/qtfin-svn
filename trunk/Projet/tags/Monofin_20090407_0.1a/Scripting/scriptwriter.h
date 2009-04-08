#ifndef SCRIPTWRITER_H
#define SCRIPTWRITER_H

#include "../Data/monofinfile.h"
#include <qtextstream.h>
#include <string>

namespace Scripting {

	/**
	  * Classe abstraite responsable de l'écriture d'une partie d'un script.
	  */
	class ScriptWriter {
	public:
		/**
		  * Méthode appelée par une classe héritant de Script au moment de la génération
		  * d'une portion du script.
		  */
        virtual void write(Data::MonofinFile& monofin, QTextStream& script) = 0;
	};

}

#endif // SCRIPTWRITER_H
