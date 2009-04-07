#ifndef COMSOLSCRIPTWRITER_H
#define COMSOLSCRIPTWRITER_H

#include "../scriptwriter.h"
#include <qtextstream.h>

namespace Scripting {
	namespace Comsol {

		/**
		  * Classe responsable de l'�criture d'une portion de code dans le langage COMSOL Script.
		  * La classe d�finies des fonctions utiles � l'�criture des scripts.
		  */
		class ComsolScriptWriter : public ScriptWriter {
		public:
			/**
			  * Imprime le d�but d'un commentaire COMSOL Script dans un flux de donn�es.
			  */
			static QTextStream& comment(QTextStream& stream);

			/**
			  * Imprime un �chappement de ligne COMSOL Script dans un flux de donn�es.
			  */
			static QTextStream& eendl(QTextStream& stream);
		};

	}
}

#endif // COMSOLSCRIPTWRITER_H
