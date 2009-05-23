#ifndef VIEWERSCRIPT_H
#define VIEWERSCRIPT_H

#include "comsolscript.h"

namespace Scripting {

	/**
	  * Class representing the viewer COMSOL script.
	  * The one that generate a picture of the 3D geometry.
	  */
	class ViewerScript : public ComsolScript
	{
	public:

		/**
		  * Default constructor.
		  * @param output_path
		  *		The output_path of the generated image with a 'png' extension.
		  */
		ViewerScript(QString output_path);

		/** Implementation */
		virtual void write(QTextStream& script, Data::ProjectFile& data) const;

		/**
		  * Return the output_path.
		  */
		QString getOutputPath() { return output_path; }
	private:

		/**
		  * Write the settings of the script.
		  */
		void ViewerScript::writeMonofinSettings(QTextStream& script, Data::ProjectFile& data) const;

		/**
		  * Writes the entry point called of the script.
		  */
		void writeMain(QTextStream& script, Data::ProjectFile& data) const;

		/* Settings */

		/** The generated image path. */
		QString output_path;
	};

}

#endif // VIEWERSCRIPT_H
