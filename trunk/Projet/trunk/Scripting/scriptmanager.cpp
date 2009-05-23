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
	: script(script), data(data), process(NULL) {
}

bool ScriptManager::write() {
	// tempory path for the script
	QString filename = QDir::toNativeSeparators(
			QString("%1/%2.m")
				.arg(ScriptDirectory)
				.arg(script->getName()));

	// if the scripts already exists, we removed it
	if (QFile::exists(filename)) {
		if (!QFile::remove(filename))
			// unable to remove the last script
			return false;
	}

	// initializing the stream
	QFile file(filename);
	QTextStream stream(&file);

	// QIODevice::Text, is interoperable regarding the eof token
	// on Windows it uses \n\r and on Linux \n...
	if (!file.open(QIODevice::Text | QIODevice::WriteOnly)) {
		// cannot open the stream
		return false;
	}

	// writing the script
	script->write(stream, data);

	// closing the file
	file.close();

	// everything went well
	return true;
}

void ScriptManager::execute() {
	// sort of semaphore, we don't want to do anything
	// while the process is not destroyed
	if (process != NULL)
		return;

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

	// setting up the working directory
	process->setWorkingDirectory(ScriptDirectory);

	// calling COMSOL
	// WARNING
	//	Here, we suppose that the COMSOL directory is in the PATH environment variable
	//	of the user. It's better of us and for the user as we do not need to store the
	//	directory where to find the executables.
#ifdef WIN
	// on Windows we need to call 'comsolbatch' and then to give the script name
	process->start(
			"comsolbatch",
			QStringList() << script->getName());
#else
	// on Linux and Mac we just have to call 'comsol' and passing
	// 'batch' as a first argument and the the script name
	process->start(
			"comsol",
			QStringList() << "batch" << script->getName());
#endif

	//process->waitForStarted();
}

void ScriptManager::_started() {
	// the process started, we emit the 'started' signal
	emit started();
}

void ScriptManager::_finished(int exitCode, QProcess::ExitStatus exitStatus) {
	// the process finished, we check the result and emit the 'ended' signal
	emit ended(exitStatus == QProcess::NormalExit && exitCode == 0 ? true : false);
	delete process;
	process = NULL;
}

void ScriptManager::_error(QProcess::ProcessError error) {
	// an error occured, we just emit the 'ended' signal
	emit ended(false);
	delete process;
	process = NULL;
}
