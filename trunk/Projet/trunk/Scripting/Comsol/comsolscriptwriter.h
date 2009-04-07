#ifndef COMSOLSCRIPTWRITER_H
#define COMSOLSCRIPTWRITER_H

#include "../scriptwriter.h"
#include <qtextstream.h>

namespace Scripting {
	namespace Comsol {

		/**
		  * Classe responsable de l'écriture d'une portion de code dans le langage COMSOL Script.
		  * La classe définies des fonctions utiles à l'écriture des scripts.
		  */
		class ComsolScriptWriter : public ScriptWriter {
		public:
			/**
			  * Imprime le début d'un commentaire COMSOL Script dans un flux de données.
			  */
			static QTextStream& comment(QTextStream& stream);

			/**
			  * Imprime un échappement de ligne COMSOL Script dans un flux de données.
			  */
			static QTextStream& eendl(QTextStream& stream);
		};

	}
}

#endif // COMSOLSCRIPTWRITER_H
