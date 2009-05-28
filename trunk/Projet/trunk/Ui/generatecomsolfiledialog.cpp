#include "generatecomsolfiledialog.h"
#include "ui_generatecomsolfiledialog.h"

#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QCloseEvent>

GenerateComsolFileDialog::GenerateComsolFileDialog(Data::ProjectFile& data, QWidget *parent) :
	QDialog(parent),
	m_ui(new Ui::GenerateComsolFileDialog),
	data(data)
{
	m_ui->setupUi(this);

	smanager = new Scripting::ScriptManager(data, this);
	QObject::connect(smanager, SIGNAL(ended(bool)), this, SLOT(on_smanager_ended(bool)));

	// init the UI
	m_ui->cbMeshSizes->setCurrentIndex(script.getMeshSizes() - 1);
	m_ui->cbSolveProblem->setChecked(script.getSolveProblem());

	setDialogState(false);
}

GenerateComsolFileDialog::~GenerateComsolFileDialog()
{
	delete m_ui;
}

void GenerateComsolFileDialog::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void GenerateComsolFileDialog::setDialogState(bool running) {
	// change the dialog state when a script is running or not
	setCursor(running ? Qt::WaitCursor : Qt::ArrowCursor);
	m_ui->bGenerate->setEnabled(!running);
	m_ui->progressBar->setVisible(running);
	m_ui->cbMeshSizes->setEnabled(!running);
	m_ui->cbSolveProblem->setEnabled(!running);
}

void GenerateComsolFileDialog::closeEvent(QCloseEvent* event) {
	if (smanager->isRunning()) {
		if (QMessageBox::question(
				this,
				tr("Cancel the script execution?"),
				tr("Are you sure you want to cancel the script execution?"),
				QMessageBox::Yes,
				QMessageBox::No) == QMessageBox::Yes) {

			// cancel the script execution
			smanager->kill();
			// updating the dialog state
			setDialogState(false);
		} else {
			// simply ignore the event
			event->ignore();
			return;
		}
	}

	event->accept();
}

void GenerateComsolFileDialog::on_bGenerate_clicked()
{
	if (smanager->isRunning())
		return; // unreachable code due to setDialogState

	QString output_path = QFileDialog::getSaveFileName(
		this,
		tr("Save file"),
		QDir::currentPath(),
		tr("MPH files (*.mph)"));

	if (output_path.isEmpty())
		return;

	script.setOutputPath(output_path);
	script.setMeshSizes(m_ui->cbMeshSizes->currentIndex() + 1);
	script.setSolveProblem(m_ui->cbSolveProblem->isChecked());

	setDialogState(true);

	if (!smanager->execute(script)) {
		emit(on_smanager_ended(false));
	}
}

void GenerateComsolFileDialog::on_smanager_ended(bool successed) {
	setDialogState(false);

	if (successed) {
		// ask the user if he wants to launch COMSOL
		QMessageBox* msg = new QMessageBox(this);
		msg->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msg->setIcon(QMessageBox::Question);
		msg->setText(tr("Script executed with success. Would you like to open the generated file in COMSOL Multiphysics now?"));

		if (msg->exec() == QMessageBox::Yes) {

			QProcess comsol(this);
			if (!comsol.startDetached("comsol", QStringList() << "multiphysics" << "-open" << script.getOutputPath())) {
				QMessageBox::critical(
						this,
						tr("Unable to locate COMSOL Multiphysics"),
						tr("Please check that COMSOL Multiphysics is accessible from your PATH environment."));
			} else {
				// msg needs to be a pointer otherwise it will be killed
				close();
			}

		}
	} else {
		// show an error message with the log of the error
		QMessageBox msg(this);
		msg.setStandardButtons(QMessageBox::Ok);
		msg.setIcon(QMessageBox::Critical);

		// trying to get the detail
		QString log = Scripting::ScriptManager::getCurrentLog();

		if (!log.isNull()) {
			msg.setText(tr("Error while executing the COMSOL script, please see the LOG file for further informations."));
			msg.setDetailedText(log);
		} else {
			msg.setText("Error that cannot be logged while executing the COMSOL script.");
		}
		msg.exec();
		close();
	}
}

void GenerateComsolFileDialog::on_cbSolveProblem_stateChanged(int state)
{
	// it is non sense to ask for a mesh sizes where no mesh will be performed
	m_ui->cbMeshSizes->setEnabled(state == Qt::Checked);
}
