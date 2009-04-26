#ifndef EDGESEXTRACTIONSCENE_H
#define EDGESEXTRACTIONSCENE_H

#include <QGraphicsScene>
#include <QWidget>
#include <QObject>
#include <QPixmap>
#include "pixmapitem.h"
#include "rotatecircle.h"

class EdgesExtractionScene : public QGraphicsScene
{
   Q_OBJECT

public:
    EdgesExtractionScene(QWidget* parent);

    void setPixmap(QPixmap);

    void rotateCircleMoved(qreal);
    void itemPositionChanged();

    PixmapItem* pixItem(){
        return _pixItem;
    }

    RotateCircle* rotCircle(){
        return _rotCircle;
    }

signals:
    void rotateAngleChanged(double);
    void positionChanged();

private:
    PixmapItem* _pixItem;
    RotateCircle* _rotCircle;
};

#endif // EDGESEXTRACTIONSCENE_H
