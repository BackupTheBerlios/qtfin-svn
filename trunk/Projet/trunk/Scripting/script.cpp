#include "script.h"

using namespace Scripting;

Script::Script(Data::MonofinFile& monofin, QTextStream& script)
	: monofin(monofin), script(script) {
}

void Script::build() const {
	// récupération des writers
	std::vector<ScriptWriter*> *writers = new std::vector<ScriptWriter*>();
	initWriters(writers);

	// création du script
	for(std::vector<ScriptWriter*>::iterator it = writers->begin(); it != writers->end(); ++it) {
		(*it)->write(monofin, script);
	}

	// destruction des writers
	delete writers;
}
