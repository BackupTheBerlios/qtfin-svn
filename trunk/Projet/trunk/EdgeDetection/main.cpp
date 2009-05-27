#include <QtGui>
#include <QApplication>
#include <iostream>
#include "SPoint.h"
#include "graphic.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Graphic g;
    //g.setSize(3048, 2000);
    g.show();

    return a.exec();
}
