#ifndef GENERATECOMSOLFILE_H
#define GENERATECOMSOLFILE_H

#include "../Data/projectfile.h"
#include "../Scripting/defaultscriptmanager.h"

#include <QtGui/QFrame>
#include <QCloseEvent>

namespace Ui {
	class GenerateComsolFile;
}

class GenerateComsolFile : public QFrame {
	Q_OBJECT
	Q_DISABLE_COPY(GenerateComsolFile)
public:
	explicit GenerateComsolFile(Data::ProjectFile& data, QWidget *parent = 0);
	virtual ~GenerateComsolFile();

protected:
	virtual void changeEvent(QEvent *e);

private:
	Ui::GenerateComsolFile *m_ui;
	Data::ProjectFile& data;
	Scripting::DefaultScriptManager* sm;

	void setFormState(bool running);
	virtual void closeEvent(QCloseEvent *event);

private slots:
	void on_bGenerate_clicked();

	void scriptExecutionEnded(bool successed);
};

#endif // GENERATECOMSOLFILE_H
