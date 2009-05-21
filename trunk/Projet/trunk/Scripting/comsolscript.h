#ifndef COMSOLSCRIPT_H
#define COMSOLSCRIPT_H

#include "basescript.h"

namespace Scripting {

	class ComsolScript : public BaseScript
	{
	public:
		ComsolScript(Data::MonofinFile& monofin, QTextStream& script);
		virtual void build() const;

	private:
		void writeSegments() const;
		void writeLayers() const;
		void writeSettings() const;
	};

}
#endif
