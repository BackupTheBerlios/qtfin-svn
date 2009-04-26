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
    g.show();

    return a.exec();
}
