#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <QObject>

#include "comsolscript.h"
#include "../Data/projectfile.h"

#include <QString>
#include <QFile>
#include <QProcess>

namespace Scripting {

	/**
	  * Object responsible for writing and executing a COMSOL Script.
	  */
	class ScriptManager : public QObject
	{
		Q_OBJECT

	public:
		/**
		  * Default constructuor.
		  * @param script
		  *		The instance of ComsolScript to work with.
		  * @param data
		  *		The data with wich the script will be build.
		  */
		ScriptManager(ComsolScript* script, Data::ProjectFile& data);

		/**
		  * A local directory where the external COMSOL Scripts needed are stored.
		  * Temporary scripts will be put in this same directory.
		  */
		const static QString ScriptDirectory;

		/**
		  * Write the script into a temporary file in the ScriptDirectory.
		  * @return True if success, False otherwise.
		  */
		bool write();

	public slots:
		/**
		  * Slot starting the execution of the script.
		  */
		void execute();

	signals:
		/**
		  * Signal received as soon as the execution of the script started.
		  */
		void started();

		/**
		  * Signal always received after a call to execute, indicating the
		  * result of the script execution.
		  * @param successed
		  *		Flag equals to True if the execution successed, False otherwise.
		  */
		void ended(bool successed);

	private slots:

		/* Internal slots to interact with a QProcess. */

		void _started();
		void _finished(int exitCode, QProcess::ExitStatus exitStatus);
		void _error(QProcess::ProcessError error);

	private:
		/** The instance of ComsolScript to work with. */
		ComsolScript* script;

		/** The data to build the script uppon with. */
		Data::ProjectFile& data;

		/** The process responsible for the script execution. */
		QProcess* process;
	};

}

#endif
