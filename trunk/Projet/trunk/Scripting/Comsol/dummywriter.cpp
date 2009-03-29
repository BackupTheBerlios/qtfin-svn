#include "dummywriter.h"

using namespace Scripting::Comsol;
using namespace std;

void DummyWriter::write(Monofin& monofin, std::ostream& script) {
	script << comment << "Voilà un commentaire" << endl;
	script << "appelDeFonction(" << eendl;
	script << "\tparam1," << eendl;
	script << "\tparam2," << eendl;
	script << "\tparam3);" << endl;
	script << comment << "Fin du faux script";
}
