#include <QtGui/QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGraphicsView>
#include "paintingscene.h"
#include "paintingview.h"
#include "toolbar.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*MainWindow w;
    w.show();*/

    QWidget* wid = new QWidget();

    QHBoxLayout* hlayout = new QHBoxLayout;
    wid->setLayout(hlayout);

    PaintingScene scene(800, 600, wid);

    ToolBar bar(&scene);
    hlayout->addWidget(&bar);

    PaintingView view(&scene);
    hlayout->addWidget(&view);

    view.show();



    wid->show();
    return a.exec();
}
