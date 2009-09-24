#include <QtGui/QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGraphicsView>
#include "paintingscene.h"
#include "paintingview.h"
#include "toolbar.h"
#include "../Data/projectfile.h"

using namespace Data;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget* wid = new QWidget();

    QHBoxLayout* hlayout = new QHBoxLayout;
    wid->setLayout(hlayout);

    /****
      En attendant d'avoir une structure qui fonctionne...
    ****/
    ProjectFile structure;

    PaintingScene scene(800, 600, &structure, wid);

    ToolBar bar(&scene);
    hlayout->addWidget(&bar);

    PaintingView view(&scene);
    hlayout->addWidget(&view);

    view.show();



    wid->show();
    return a.exec();
}
