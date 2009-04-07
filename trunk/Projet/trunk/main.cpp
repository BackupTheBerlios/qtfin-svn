#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>
#include "Ui/mainwindow.h"

#include "Ui/parametersdialog.h"

int main(int argc, char *argv[])
{
    /**
     * Application.
     */
    QApplication app(argc, argv);
    qDebug("create QApplication");

    /*
     * Splash screen.
     */
    /*QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/images/splash.png"));
    splash->show();

    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;

    splash->showMessage(QObject::tr("Setting up the main window..."),
                        topRight, Qt::white);*/
    MainWindow mainWin;
    qDebug("create MainWindow");

    mainWin.show();
    qDebug("showing MainWindow");
    /*splash->finish(&mainWin);
    delete splash;*/

    qDebug("entering execution loop");
    return app.exec();
}
