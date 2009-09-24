#ifndef SCRIPT_H
#define SCRIPT_H

#include "../Data/projectfile.h"

#include <QTextStream>
#include <QString>

namespace Scripting {

	/**
	  * Abstract class representing a COMSOL Script.
	  * Classes that inherited from ComsolScript are able to write down their code
	  * in a QTextStream.
	  */
	class ComsolScript {
	public:

		/** Default constructor. */
		ComsolScript();

		/** Destructor. */
		virtual ~ComsolScript();

		/**
		  * Abstract method used to write a script in a QTextStream.
		  * @param script
		  *		The stream in which to write the script.
		  * @param data
		  *		The instance of Data::ProjectFile to use in order to write the script.
		  *		It should contain all the needed informations to build the script.
		  */
		virtual void write(QTextStream& script, Data::ProjectFile& data) const = 0;
	};

}

#endif
