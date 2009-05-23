#ifndef COMSOLSCRIPT_H
#define COMSOLSCRIPT_H

#include "comsolscript.h"

namespace Scripting {

	/**
	  * Class representing the default COMSOL script.
	  * The one that run the simulation.
	  */
	class DefaultScript : public ComsolScript
	{
	public:

		/**
		  * Default constructor.
		  * @param solve_problem
		  *		Flag indicating if the script must solve the problem (run the simulation).
		  * @param mesh_sizes
		  *		An integer to describe the mesh sizes to use during the mesh process.
		  *		Only revelant if solve_problem is set to true.
		  *		The value range is between 1 and 9 with a default of 5.
		  *			1 : fine mesh sizes
		  *			X : more and more coerced
		  *			9 : coerce mesh sizes
		  */
		DefaultScript(bool solve_problem, int mesh_sizes, QString output_path);

		/** Implementation. */
		virtual void write(QTextStream& script, Data::ProjectFile& data) const;

	private:

		/**
		  * Writes the settings of the script.
		  */
		void writeMonofinSettings(QTextStream& script, Data::ProjectFile& data) const;

		/**
		  * Writes the entry point called of the script.
		  */
		void writeMain(QTextStream& script, Data::ProjectFile& data) const;

		/* Settings */

		/** @see DefaultScript */
		bool solve_problem;

		/** @see DefaultScript */
		int mesh_sizes;

		/** @see DefaultScript */
		QString output_path;
	};

}
#endif
