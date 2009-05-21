#include "basescript.h"

using namespace Scripting;

BaseScript::BaseScript(Data::MonofinFile& monofin, QTextStream& script)
	: monofin(monofin), script(script) {
}
