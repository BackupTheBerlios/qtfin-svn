#include "scriptwriter.h"

using namespace Scripting;

std::ostream& ScriptWriter::comment(std::ostream& stream) {
	stream << "% ";
	return stream;
}

std::ostream& ScriptWriter::eendl(std::ostream& stream) {
	stream << " ..." << std::endl;
	return stream;
}
