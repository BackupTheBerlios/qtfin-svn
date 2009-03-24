#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>
#include "Ui/mainwindow.h"

#include "Ui/parametersdialog.h"
#include "Ui/layerParameters.h"

int main(int argc, char *argv[])
{
    /**
     * Application.
     */
    QApplication app(argc, argv);

    /*
     * Splash screen.
     */
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/images/splash.png"));
    splash->show();

    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;

    splash->showMessage(QObject::tr("Setting up the main window..."),
                        topRight, Qt::white);
    MainWindow mainWin;

    mainWin.show();
    splash->finish(&mainWin);
    delete splash;

    /** pour test :**/
    ParametersDialog pdiag;
    if(pdiag.exec()) {
        QTabWidget *qtw = pdiag.layerTabWidget;
        for(int i = 0; i<qtw->count(); ++i) {
            LayerParameters * ll = static_cast<LayerParameters*> (qtw->widget(i));
            qDebug("%f", ll->youngDoubleSpinBox->value());
        }
    }
    /** fin test **/

    return app.exec();
}
