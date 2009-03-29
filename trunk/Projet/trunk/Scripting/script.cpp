#include "script.h"

using namespace Scripting;

Script::Script(Monofin& monofin, std::ostream& script)
	: monofin(monofin), script(script) {
}

void Script::build() const {
	// r�cup�ration des writers
	std::vector<ScriptWriter*> *writers = new std::vector<ScriptWriter*>();
	initWriters(writers);

	// cr�ation du script
	for(std::vector<ScriptWriter*>::iterator it = writers->begin(); it != writers->end(); ++it) {
		(*it)->write(monofin, script);
	}

	// destruction des writers
	delete writers;
}
