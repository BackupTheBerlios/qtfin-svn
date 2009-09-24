#include "scriptmanager.h"

#include <QObject>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QStringList>
#include <QMessageBox>

#include <QString>
#include <QTextStream>
#include <QFile>
#include <QIODevice>

using namespace Scripting;

#ifdef Q_OS_WIN32

// On Windows if a batch (.bat) process spawned new process, for instance
// java in comsolapi.bat, to kill the initial process won't kill the java process.
// So on Windows we are unable to cancel a script execution.
// To solve the problem, we use the workaround proposed here: http://lists.trolltech.com/qt-interest/2005-10/thread00706-0.html

// from <windows.h>, it should work from Windows 2000 to newer
// HANDLE replaced by int
// DWORD replaced by unsigned long
// Structure containing the PID of a process.
typedef struct _PROCESS_INFORMATION {
  int hProcess;
  int hThread;
  unsigned long dwProcessId;
  unsigned long dwThreadId;
} PROCESS_INFORMATION;

#endif

ScriptManager::ScriptManager(Data::ProjectFile& data, QObject* parent)
	: QObject(parent), data(data)
{
	process = new QProcess(this);

	// connect the signals
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
}

void ScriptManager::disconnect() {
	QObject::disconnect(
			process, SIGNAL(finished(int, QProcess::ExitStatus)),
			this, SLOT(_finished(int, QProcess::ExitStatus)));
	QObject::disconnect(
			process, SIGNAL(error(QProcess::ProcessError)),
			this, SLOT(_error(QProcess::ProcessError)));
}

bool ScriptManager::execute(ComsolScript& script)
{
	if (process->state() != QProcess::NotRunning)
		return false;

	// first, delete the last execution log
	deleteLog();

#ifdef Q_OS_WIN32
	process->start(
			"comsolapi.bat",
			QStringList() << "ComsolEvaluator");
#else
	process->start(
			"comsolapi",
			QStringList() << "ComsolEvaluator");
#endif

	// trying to open the process in order to send the script
	if (process->open(QIODevice::ReadWrite)) {
		// retrieve the process output stream
		QTextStream stream(process);
		// write down the script in the stream
		script.write(stream, data);
		// close the stream in order to let the process execute the script
		process->closeWriteChannel();
		return true;
	} else {
		return false;
	}
}

void ScriptManager::kill() {
	if (process->state() == QProcess::NotRunning)
		return;

	// disconnect the signals
	disconnect();

#ifdef Q_OS_WIN32
	// to properly kill the process tree (java is a leaf in this tree),
	// we execute a TASKKILL instruction. We just need to retrieve the Windows PID
	// of the QProcess. We use the PROCESS_INFORMATION structure to do so.
	PROCESS_INFORMATION *pinfo = (PROCESS_INFORMATION*)process->pid();
	QProcess p(this);
	p.execute("TASKKILL",
			  QStringList() << "/PID"
							<< QString("%1").arg(pinfo->dwProcessId)
							<< "/T"
							<< "/F");
#endif

	// kill the process
	process->kill();
	// wait for the kill to be completed
	process->waitForFinished();
	// reconnect the signals
	connect();
}

void ScriptManager::_finished(int exitCode, QProcess::ExitStatus exitStatus) {
	bool successed = exitStatus == QProcess::NormalExit && exitCode == 0;
	emit ended(successed);
}

void ScriptManager::_error(QProcess::ProcessError error) {
	emit ended(false);
}

/* STATIC members */

const QString ScriptManager::COMSOL_EVALUATOR_LOG = "ComsolEvaluator.log";

QString ScriptManager::getScriptDirectory(QString filename) {
	return QString("%1/scripts/%2")
			.arg(QDir::currentPath())
			.arg(filename);
}

QString ScriptManager::getCurrentLog() {
	QString log = QString::null;

	// writer to write append content to the QString log
	QTextStream writer(&log, QIODevice::WriteOnly | QIODevice::Text);

	// the log file
	QFile file(Scripting::ScriptManager::getScriptDirectory(COMSOL_EVALUATOR_LOG));

	// trying to open the log file
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream reader(&file);
		QString line;
		while (!(line = reader.readLine()).isNull()) {
			writer << line;
			writer << endl;
		}
		file.close();
	}

	return log;
}

void ScriptManager::deleteLog() {
	// try to delete the current log file
	QString log = getScriptDirectory(COMSOL_EVALUATOR_LOG);
	if (QFile::exists(log))
		QFile::remove(log);
}
