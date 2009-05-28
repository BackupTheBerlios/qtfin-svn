#ifndef GENERATECOMSOLFILEDIALOG_H
#define GENERATECOMSOLFILEDIALOG_H

#include <QtGui/QDialog>

#include "../Data/projectfile.h"
#include "../Scripting/scriptmanager.h"
#include "../Scripting/defaultscript.h"

namespace Ui {
	class GenerateComsolFileDialog;
}

class GenerateComsolFileDialog : public QDialog {
	Q_OBJECT
	Q_DISABLE_COPY(GenerateComsolFileDialog)
public:
	explicit GenerateComsolFileDialog(Data::ProjectFile& data, QWidget *parent = 0);
	virtual ~GenerateComsolFileDialog();

protected:
	virtual void changeEvent(QEvent *e);

private:
	Ui::GenerateComsolFileDialog *m_ui;

	// data
	Data::ProjectFile& data;
	// scriptmanager
	Scripting::ScriptManager* smanager;
	// script
	Scripting::DefaultScript script;

	void setDialogState(bool running);

	virtual void closeEvent(QCloseEvent *event);

private slots:
	void on_cbSolveProblem_stateChanged(int );
 void on_bGenerate_clicked();
	void on_smanager_ended(bool successed);

};

#endif // GENERATECOMSOLFILEDIALOG_H
