#include "comsolscriptwriter.h"

using namespace Scripting::Comsol;

QTextStream& ComsolScriptWriter::comment(QTextStream& stream) {
	stream << "% ";
	return stream;
}

QTextStream& ComsolScriptWriter::eendl(QTextStream& stream) {
	stream << " ..." << endl;
	return stream;
}
