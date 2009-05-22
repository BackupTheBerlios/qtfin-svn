#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include "comsolscript.h"
#include "../Data/projectfile.h"
#include <QString>
#include <QObject>
#include <QFile>
#include <QProcess>

namespace Scripting {

	class ScriptManager : public QObject
	{
		Q_OBJECT

	public:
		/** Default constructor. */
		ScriptManager(ComsolScript* script, Data::ProjectFile& data);

		/** A directory used to create and execute scripts. */
		const static QString ScriptDirectory;

	public slots:
		void execute();

	signals:
		void started();
		void ended(bool successed);

	private slots:
		void _started();
		void _finished(int exitCode, QProcess::ExitStatus exitStatus);
		void _error(QProcess::ProcessError error);

	private:		
		ComsolScript* script;
		QProcess* process;
	};

}

#endif
