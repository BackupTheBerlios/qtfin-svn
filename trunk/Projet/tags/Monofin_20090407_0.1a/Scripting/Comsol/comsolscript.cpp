#include "comsolscript.h"
#include "dummywriter.h"

using namespace Scripting;
using namespace Scripting::Comsol;

ComsolScript::ComsolScript(Data::MonofinFile& monofin, QTextStream& script) :
		Script(monofin, script) {
}

void ComsolScript::initWriters(std::vector<ScriptWriter*>* writers) const {
	writers->push_back(new DummyWriter());
}
