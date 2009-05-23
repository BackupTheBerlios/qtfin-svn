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

		/**
		  * Default constructor.
		  * @param name
		  *		The name of the script. This will be the filename given to the temporary file on disk.
		  */
		ComsolScript(QString name);

		/** Destructor. */
		virtual ~ComsolScript();

		/**
		  * Get the name of the script.
		  */
		QString getName() const { return name; }

		/**
		  * Abstract method used to write a script in a QTextStream.
		  * @param script
		  *		The stream in which to write down the script.
		  * @param data
		  *		The instance of Data::ProjectFile for which to write the script.
		  *		It should contain all the needed informations necessary to build the script.
		  */
		virtual void write(QTextStream& script, Data::ProjectFile& data) const = 0;
	private:

		/** The script name. */
		QString name;
	};

}

#endif
