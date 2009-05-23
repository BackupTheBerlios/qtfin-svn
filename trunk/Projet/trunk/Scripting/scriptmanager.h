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

		/** Destructor */
		~ScriptManager();

		/**
		  * Get a flag indicating if the process responsible for the script execution is running.
		  * When the process is running, any call to execute will return false.
		  */
		bool isRunning() const { return process->state() == QProcess::Running; }

		/**
		  * A local directory where the external COMSOL Scripts needed are stored.
		  * Temporary scripts will be put in this same directory.
		  */
		const static QString ScriptDirectory;

	public slots:

		/**
		  * Slot starting the execution of the script.
		  * As soon as the method return true, the signals started() and ended(bool) will
		  * be emited.
		  * @return
		  *		True if the script is executing, False otherwise.
		  */
		bool execute();

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
		void _debug_output();

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
