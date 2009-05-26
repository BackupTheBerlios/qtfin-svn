#include "scriptmanager.h"

#include <QObject>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QStringList>

using namespace Scripting;

QString ScriptManager::getScriptDirectory(QString filename) {
	return QString("%1/scripts/%2")
			.arg(QDir::currentPath())
			.arg(filename);
}

ScriptManager::ScriptManager(Data::ProjectFile& data)
	: data(data)
{
	process = new QProcess(this);
	//process->setStandardOutputFile("output.txt");

	// signals connection
	connect();

	// setting up the working directory
	process->setWorkingDirectory(getScriptDirectory());
}

void ScriptManager::connect() {
	QObject::connect(
			process, SIGNAL(finished(int, QProcess::ExitStatus)),
			this, SLOT(_finished(int, QProcess::ExitStatus)));
	QObject::connect(
			process, SIGNAL(error(QProcess::ProcessError)),
			this, SLOT(_error(QProcess::ProcessError)));
	QObject::connect(
			process, SIGNAL(readChannelFinished()),
			this, SLOT(_debug_output()));
}

void ScriptManager::disconnect() {
	QObject::disconnect(
			process, SIGNAL(finished(int, QProcess::ExitStatus)),
			this, SLOT(_finished(int, QProcess::ExitStatus)));
	QObject::disconnect(
			process, SIGNAL(error(QProcess::ProcessError)),
			this, SLOT(_error(QProcess::ProcessError)));
	QObject::disconnect(
			process, SIGNAL(readChannelFinished()),
			this, SLOT(_debug_output()));
}

ScriptManager::~ScriptManager() {
	delete process;
}

bool ScriptManager::execute(ComsolScript& script)
{
	if (process->state() != QProcess::NotRunning)
		return false;

	qDebug("ScriptManager::execute()");

	// calling JAVA ComsolEvaluator to execute the script
	// the exit code of the process will give us information about the success of the execution
#ifdef Q_OS_WIN32
	qDebug("WIN config");

	// on Windows, we need to precise .bat extension...
	process->start(
			"comsolapi.bat",
			QStringList() << "ComsolEvaluator");
#else
	qDebug("!WIN config");

	process->start(
			"comsolapi",
			QStringList() << "ComsolEvaluator");
#endif

	// trying to open the process in order
	if (process->open(QIODevice::ReadWrite)) {
		// retrieve the process output stream
		QTextStream stream(process);
		// write down the script in the stream
		script.write(stream, data);
		// close the stream in order to let the process execute the script
		process->closeWriteChannel();
		return true;
	} else {
		qDebug("ScriptManager::execute() return false");
		return false;
	}
}

void ScriptManager::kill() {
	// just kill the process if it's running
	if (process->state() != QProcess::NotRunning) {
		qDebug("ScriptManager::kill()");
		disconnect();
		process->kill();
		process->waitForFinished();
		connect();
	}
}

void ScriptManager::_finished(int exitCode, QProcess::ExitStatus exitStatus) {
	// the process finished, we check the result and emit the 'ended' signal
//	if (exitStatus == QProcess::CrashExit)
//		return; // le slot _error a capturé un QProcess:Crashed
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
