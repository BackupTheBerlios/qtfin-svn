#ifndef SCRIPT_H
#define SCRIPT_H

#include "../Data/monofinfile.h"
#include "scriptwriter.h"

#include <qtextstream.h>
#include <vector>

namespace Scripting {

	/**
	  * Classe abstraite responsable de la génération d'un script.
	  */
	class Script {
	private:
		Monofin& monofin;
		QTextStream& script;

	public:
		/**
		  * Constructeur par défaut.
		  * @param monofin L'instance à partir de laquelle le script doit être cosntruit.
		  * @param script Le flux de données dans lequel le script est écrit.
		  */
		Script(Monofin& monofin, QTextStream& script);

		/**
		  * Méthode qui lance la génération du script.
		  */
		void build() const;

		/**
		  * Cette méthode est appelée par build au moment de la génération du script.
		  * Une classe héritant de Script doit implémenter cette méthode pour initialiser la liste ordonnée
		  * des ScriptWriter qui seront utilisés pour générer le script.
		  * @param writers Liste des instances de ScriptWriter à utiliser pour la génération du script.
		  */
		virtual void initWriters(std::vector<ScriptWriter*>* writers) const = 0;
	};

}

#endif // SCRIPT_H
