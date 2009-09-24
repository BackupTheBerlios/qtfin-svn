#include "generatecomsolfile.h"
#include "ui_generatecomsolfile.h"

#include <QDir>
#include <QFileDialog>
#include <QString>
#include <QDebug>
#include <QMessageBox>

GenerateComsolFile::GenerateComsolFile(Data::ProjectFile& data, QWidget *parent) :
	QFrame(parent),
	m_ui(new Ui::GenerateComsolFile),
	data(data)
{
	m_ui->setupUi(this);
	setFormState(false);
	sm = new Scripting::DefaultScriptManager(data);
	QObject::connect(sm, SIGNAL(ended(bool)), this, SLOT(scriptExecutionEnded(bool)));
}

GenerateComsolFile::~GenerateComsolFile()
{
	delete m_ui;
}

void GenerateComsolFile::setFormState(bool running) {
	m_ui->bGenerate->setText(running
							 ? tr("Cancel")
							 : tr("Generate..."));
	m_ui->progressBar->setVisible(running);
	m_ui->cbMeshSizes->setEnabled(!running);
	m_ui->cbSolveProblem->setEnabled(!running);
}

void GenerateComsolFile::changeEvent(QEvent *e)
{
	QFrame::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void GenerateComsolFile::on_bGenerate_clicked()
{
	if (!sm->isRunning()) {
		QString output_path = QFileDialog::getSaveFileName(
				this,
				tr("Save file"),
				QDir::currentPath(),
				tr("MPH files (*.mph)"));

		if (output_path.isEmpty())
			return;

		setFormState(true);

		if (!sm->execute(
				m_ui->cbSolveProblem->isChecked(),
				m_ui->cbMeshSizes->currentIndex() + 1,
				output_path)) {
			emit(scriptExecutionEnded(false));
		}
	} else {
		sm->kill();
		setFormState(false);
	}
}

void GenerateComsolFile::closeEvent(QCloseEvent *event) {
	if (sm->isRunning())
		event->ignore();
	else
		event->accept();
}

void GenerateComsolFile::scriptExecutionEnded(bool successed) {
	setFormState(false);

	QMessageBox msg(this);
	msg.setStandardButtons(QMessageBox::Ok);
	msg.setIcon(successed
				? QMessageBox::Information
				: QMessageBox::Critical);
	msg.setText(successed
				? tr("File created with success!")
				: tr("An error occured during the execution of the script."));
	msg.exec();

	if (successed)
		close();
}
