#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <QApplication>
#include <QtGui/QWidget>
#include "ui_graphic.h"

#include "edgesextractionscene.h"
#include "edgesextractionview.h"
#include "pixmapitem.h"
#include "rotatecircle.h"
#include "algosnake.h"




class Graphic : public QWidget {
    Q_OBJECT

public:
    Graphic(QWidget *parent = 0);
    ~Graphic();


private:
    Ui::Form _graphic;
    EdgesExtractionScene* _graphicsScene;
    EdgesExtractionView* _graphicsView;
    AlgoSnake* _algo;


public slots:
    void setPixmap();
    void rotate(double);
    void scaleChanged(double);
    void positionChanged();
    void startAlgo();

};

#endif // GRAPHIC_H
