#include "scriptmanager.h"

#include <QObject>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QStringList>

using namespace Scripting;

/* Constant initialization. */
const QString ScriptManager::ScriptDirectory = QString("%1/scripts").arg(QDir::currentPath());

ScriptManager::ScriptManager(ComsolScript* script, Data::ProjectFile& data)
	: script(script), data(data)
{
	process = new QProcess(this);

	// signals connection
	QObject::connect(
			process, SIGNAL(started()),
			this, SLOT(_started()));
	QObject::connect(
			process, SIGNAL(finished(int, QProcess::ExitStatus)),
			this, SLOT(_finished(int, QProcess::ExitStatus)));
	QObject::connect(
			process, SIGNAL(error(QProcess::ProcessError)),
			this, SLOT(_error(QProcess::ProcessError)));
	QObject::connect(
			process, SIGNAL(readChannelFinished()),
			this, SLOT(_debug_output()));

	// setting up the working directory
	process->setWorkingDirectory(ScriptDirectory);
}

ScriptManager::~ScriptManager() {
	delete process;
}

bool ScriptManager::execute()
{
	if (process->state() != QProcess::NotRunning)
		return false;

	qDebug("ScriptManager::execute()");

	// calling JAVA ComsolEvaluator to execute the script
	// the exit code of the process will give us information about the success of the execution
#ifdef WIN
	// on Windows, we need to precise .bat extension...
	process->start(
			"comsolapi.bat",
			QStringList() << "ComsolEvaluator");
#else
	process->start(
			"comsolapi",
			QStringList() << "ComsolEvaluator");
#endif

	// trying to open the process in order
	if (process->open(QIODevice::ReadWrite)) {
		// retrieve the process output stream
		QTextStream stream(process);
		// write down the script in the stream
		script->write(stream, data);
		// close the stream in order to let the process execute the script
		process->closeWriteChannel();
		return true;
	} else {
		return false;
	}
}

void ScriptManager::_started() {
	// the process started, we emit the 'started' signal
	qDebug("started");
	emit started();
}

void ScriptManager::_finished(int exitCode, QProcess::ExitStatus exitStatus) {
	// the process finished, we check the result and emit the 'ended' signal
	bool successed = exitStatus == QProcess::NormalExit && exitCode == 0;
	qDebug("ended(%d)", successed);
	emit ended(successed);
}

void ScriptManager::_error(QProcess::ProcessError error) {
	// an error occured, we just emit the 'ended' signal
	bool successed = false;
	qDebug("ended(%d)", successed);
	emit ended(successed);
}

void ScriptManager::_debug_output() {
	while (process->canReadLine()) {
		qDebug(process->readLine());
	}
}
