#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <QObject>

#include "comsolscript.h"
#include "../Data/projectfile.h"

#include <QObject>
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
		  * @param data
		  *		The data with wich the script will be build.
		  */
		ScriptManager(Data::ProjectFile& data, QObject* parent = 0);

		/**
		  * Get a flag indicating if the process responsible for the script execution is running.
		  * When the process is running, any call to execute will return false.
		  */
		bool isRunning() const { return process->state() == QProcess::Running; }

		/* STATIC members */

		/**
		  * Get the directory where the external COMSOL Scripts are.
		  * @param filename
		  *		An optional filename that will be append to the returned path.
		  */
		static QString getScriptDirectory(QString filename = "");

		/** The filename of the ComsolEvaluator.java log file. */
		const static QString COMSOL_EVALUATOR_LOG;

		/**
		  * Return the content of the ComsolEvaluator.log file created after a script execution.
		  * If the file does not existe, QString::null is returned.
		  */
		static QString getCurrentLog();

		/** Delete the current ComsolEvaluator.java log file, if it exists. */
		static void deleteLog();

	public slots:

		/**
		  * Slot starting the execution of the script.
		  * As soon as the method return true, the signals started() and ended(bool) will
		  * be emited.
		  * @param script
		  *		The instance of ComsolScript to execute.
		  * @return
		  *		True if the script is executing, False otherwise.
		  */
		bool execute(ComsolScript& script);

		/**
		  * Slot killing the process in case we want to cancel the script execution.
		  */
		void kill();

	signals:
		/**
		  * Signal always received after a call to execute, indicating the
		  * result of the script execution.
		  * @param successed
		  *		Flag equals to True if the execution successed, False otherwise.
		  */
		void ended(bool successed);

	private slots:

		/* Internal slots to interact with a QProcess. */

		void _finished(int exitCode, QProcess::ExitStatus exitStatus);
		void _error(QProcess::ProcessError error);

	private:
		/** The data to build the script uppon with. */
		Data::ProjectFile& data;

		/** The process responsible for the script execution. */
		QProcess* process;

		/* QProcess's signals connection */

		void disconnect();
		void connect();
	};

}

#endif
