#include "scriptmanager.h"

#include <QObject>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QStringList>

using namespace Scripting;

const QString ScriptManager::ScriptDirectory =
		QString("%1/scripts").arg(QDir::currentPath());

ScriptManager::ScriptManager(ComsolScript* script, Data::ProjectFile& data)
	: script(script) {

	process = NULL;

	QString filename = QDir::toNativeSeparators(
			QString("%1/%2.m")
				.arg(ScriptDirectory)
				.arg(script->getName()));

	if (QFile::exists(filename))
		QFile::remove(filename);

	QFile file(filename);
	QTextStream stream(&file);

	// QIODevice::Text, is interoperable regarding the eof token
	if (!file.open(QIODevice::Text | QIODevice::WriteOnly)) {
		// TODO: throw exception
		return;
	}
	script->write(stream, data);
	file.close();
}

void ScriptManager::execute() {
	if (process != NULL)
		return;

	process = new QProcess(this);

	QObject::connect(
			 process, SIGNAL(started()),
			 this, SLOT(_started()));
	QObject::connect(
			 process, SIGNAL(finished(int, QProcess::ExitStatus)),
			 this, SLOT(_finished(int, QProcess::ExitStatus)));
	QObject::connect(
			 process, SIGNAL(error(QProcess::ProcessError)),
			 this, SLOT(_error(QProcess::ProcessError)));

	process->setWorkingDirectory(ScriptDirectory);

#ifdef WIN
	process->start(
			"comsolbatch",
			QStringList() << script->getName());
#else
	process->start(
			"comsol",
			QStringList() << "batch" << script->getName());
#endif

	process->waitForStarted();
}

void ScriptManager::_started() {
	emit started();
}

void ScriptManager::_finished(int exitCode, QProcess::ExitStatus exitStatus) {
	emit ended(true);
	delete process;
}

void ScriptManager::_error(QProcess::ProcessError error) {
	emit ended(false);
	delete process;
}
