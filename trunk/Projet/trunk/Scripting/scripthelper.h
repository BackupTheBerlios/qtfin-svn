#ifndef SCRIPTHELPER_H
#define SCRIPTHELPER_H

#include "../Data/projectfile.h"

#include <QTextStream>

namespace Scripting {

	class ScriptHelper {
	public:
		/* Constants */

		const static float DEFAULT_MONOFIN_LENGTH = 25; // cm
		const static float DEFAULT_LAYER_THICKNESS = 1; // cm
		const static float DEFAULT_LAYER_E = 200000;	// MPa
		const static float DEFAULT_LAYER_NU = 0.33;		// ratio
		const static float DEFAULT_LAYER_RHO = 7850;	// Kg/m^3

		/**
		  * Write the bloc path of the script. It just indicate COMSOL where to find the
		  * external scripts needed.
		  */
		static void writePath(QTextStream& script, Data::ProjectFile& data);

		/**
		  * Write the declaration of a structure called 'monofin'. Everything we write if
		  * mainly about this structure.
		  */
		static void writeMonofinDeclaration(QTextStream& script, Data::ProjectFile& data);

		/**
		  * Write the length in meter of the monofin.
		  */
		static void writeMonofinLength(QTextStream& script, Data::ProjectFile& data);

		/**
		  * Write the segments of the monofin. The segments are Bezier curves.
		  */
		static void writeMonofinSegments(QTextStream& script, Data::ProjectFile& data);

		/**
		  * Write the information about the layers of the monofin.
		  */
		static void writeMonofinLayers(QTextStream& script, Data::ProjectFile& data);

		/**
		  * Write the End Of Script token.
		  */
		static void writeEOS(QTextStream& script);
	};

}

#endif
