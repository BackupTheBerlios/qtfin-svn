#include "geometry3dviewer.h"

#include "../Scripting/scriptmanager.h"
#include "../Scripting/viewerscript.h"

#include <QGridLayout>
#include <QImage>
#include <QDir>
#include <QCloseEvent>
#include <QMessageBox>

Geometry3DViewer::Geometry3DViewer(Data::ProjectFile& data, QWidget *parent)
{
	// setting up the frame
	setWindowTitle(tr("3D Geometry Preview"));
	setFixedWidth(800);
	setFixedHeight(600);

	// layout
	setLayout(new QGridLayout(this));

	// lState
	lState = new QLabel(this);
	lState->setAlignment(Qt::AlignCenter);
	lState->setScaledContents(true);
	layout()->addWidget(lState);

	// ScriptManager
	smanager = new Scripting::ScriptManager(data, this);
	QObject::connect(smanager, SIGNAL(ended(bool)), this, SLOT(on_smanager_ended(bool)));
}

void Geometry3DViewer::show() {
	QFrame::show();

	lState->setText(tr("Retrieving the geometry preview, please wait..."));

	if (!smanager->execute(script)) {
		lState->setText(tr("ERROR: Unable the create the process responsible for the script execution."));
	}
}

void Geometry3DViewer::closeEvent(QCloseEvent *event) {
	smanager->kill();
	event->accept();
}

void Geometry3DViewer::on_smanager_ended(bool successed) {
	if (!successed) {
		// we retrieve the log and then show a message to the user

		QString log = Scripting::ScriptManager::getCurrentLog();

		QMessageBox msg(this);
		msg.setIcon(QMessageBox::Critical);
		if (!log.isNull()) {
			msg.setText(tr("Error while executing the COMSOL script, please see the LOG file for further informations."));
			msg.setDetailedText(log);
		} else {
			msg.setText("Error that cannot be logged while executing the COMSOL script.");
		}
		msg.exec();
		close();

	} else {
		QImage image(QDir::toNativeSeparators(script.getOutputPath()));
		if (image.isNull()) {
			lState->setText(tr("ERROR: Unable to display the geometry preview image."));
		} else {
			lState->setPixmap(QPixmap::fromImage(image));
		}
	}
}
