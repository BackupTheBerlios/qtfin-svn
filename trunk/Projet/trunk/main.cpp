#include <QtCore/QTimer>
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

    QTimer *timer = new QTimer;
    QSplashScreen *splash = new QSplashScreen;
    splash->setAttribute(Qt::WA_DeleteOnClose);
    MainWindow* mainWin = new MainWindow;
    mainWin->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(timer, SIGNAL(timeout()), mainWin, SLOT(show()));
    QObject::connect(timer, SIGNAL(timeout()), splash, SLOT(close()));

    splash->setPixmap(QPixmap(":/images/splash"));
    splash->show();

    timer->start(2000);

    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
    splash->showMessage(QObject::tr("Setting up the main window..."),
                        topRight, Qt::white);

    qDebug("entering the main event loop");
    return app.exec();;
}
