#include "Ui/mainWindow.h"
#include "Ui/startupDialog.h"
#include "Ui/steps.h"


#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
	// declaration de l'application Qt
    QApplication a(argc, argv);

    // declaration des widgets
    StartupDialog sd;
    MainWindow mw;
    Steps st;

	QObject::connect(sd.getButton1(), SIGNAL(clicked()), &mw, SLOT(showme()));
	QObject::connect(sd.getButton2(), SIGNAL(clicked()), &st, SLOT(openFile()));

	sd.show();

    return a.exec();
}
