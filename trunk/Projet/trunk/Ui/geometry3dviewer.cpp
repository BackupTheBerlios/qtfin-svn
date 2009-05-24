#include "geometry3dviewer.h"

#include "../Scripting/scriptmanager.h"

#include <QGridLayout>
#include <QImage>
#include <QDir>

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

	// vsm
	vsm = new Scripting::ViewerScriptManager(
			QDir::toNativeSeparators(QString("%1/temp.png").arg(Scripting::ScriptManager::ScriptDirectory)),
			data);

	QObject::connect(vsm, SIGNAL(ended(bool)), this, SLOT(scriptExecutionEnded(bool)));
}

void Geometry3DViewer::show() {
	QFrame::show();
	lState->setText(tr("Retrieving the geometry preview, please wait..."));
	// starting the script execution
	if (!vsm->execute()) {
		emit(scriptExecutionEnded(false));
	}
}

Geometry3DViewer::~Geometry3DViewer() {
	delete lState;
	delete vsm;
}

void Geometry3DViewer::scriptExecutionEnded(bool successed) {
	if (!successed) {
		lState->setText(tr("An error occured, unable to retrieve the geometry preview."));
	} else {
		QImage image(vsm->getOutputPath());
		if (image.isNull()) {
			lState->setText(tr("Unable to display the geometry preview image."));
		} else {
			lState->setPixmap(QPixmap::fromImage(image));
		}
	}
}
