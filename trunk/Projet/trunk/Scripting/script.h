#ifndef SCRIPT_H
#define SCRIPT_H

#include "../Data/monofinfile.h"
#include "scriptwriter.h"

#include <qtextstream.h>
#include <vector>

namespace Scripting {

	/**
	  * Classe abstraite responsable de la g�n�ration d'un script.
	  */
	class Script {
	private:
		Monofin& monofin;
		QTextStream& script;

	public:
		/**
		  * Constructeur par d�faut.
		  * @param monofin L'instance � partir de laquelle le script doit �tre cosntruit.
		  * @param script Le flux de donn�es dans lequel le script est �crit.
		  */
		Script(Monofin& monofin, QTextStream& script);

		/**
		  * M�thode qui lance la g�n�ration du script.
		  */
		void build() const;

		/**
		  * Cette m�thode est appel�e par build au moment de la g�n�ration du script.
		  * Une classe h�ritant de Script doit impl�menter cette m�thode pour initialiser la liste ordonn�e
		  * des ScriptWriter qui seront utilis�s pour g�n�rer le script.
		  * @param writers Liste des instances de ScriptWriter � utiliser pour la g�n�ration du script.
		  */
		virtual void initWriters(std::vector<ScriptWriter*>* writers) const = 0;
	};

}

#endif // SCRIPT_H
