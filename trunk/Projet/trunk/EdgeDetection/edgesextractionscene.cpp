#include <QPen>
#include "edgesextractionscene.h"

EdgesExtractionScene::EdgesExtractionScene(QWidget* parent):
        QGraphicsScene(parent), _pixItem(NULL)
{
    this->setSceneRect(QRectF(0,0,1000,700));

    /*_pixItem = new PixmapItem(QPixmap("Images/test"));
    _pixItem = new PixmapItem();
    _pixItem->setZValue(1);
    this->addItem(_pixItem);
    _pixItem->center();

    _rotCircle = new RotateCircle(_pixItem);
    _rotCircle->setZValue(2);
    this->addItem(_rotCircle);*/

    QGraphicsLineItem* ligne1 = this->addLine(0, 350, 1000, 350);
    ligne1->setPen(QPen(QBrush("red"), 3));
    ligne1->setZValue(3);

    QGraphicsRectItem* rect1 = addRect(0, 0, 100, 700);
    rect1->setPen(QPen(QBrush("red"), 2));
    rect1->setBrush(QBrush("red",Qt::Dense4Pattern));
    rect1->setZValue(4);

    QGraphicsRectItem* rect2 = addRect(this->sceneRect());
    rect2->setPen(QPen(QBrush("black"), 3));
    rect2->setZValue(5);

}

void EdgesExtractionScene::setPixmap(QPixmap pixmap){
    if(_pixItem != NULL){
        this->removeItem(_pixItem);
        this->removeItem(_rotCircle);
    }

    _pixItem = new PixmapItem(pixmap);
    _pixItem->setZValue(1);
    this->addItem(_pixItem);
    _pixItem->center();

    _rotCircle = new RotateCircle(_pixItem);
    _rotCircle->setZValue(2);
    this->addItem(_rotCircle);

    this->update();
}

void EdgesExtractionScene::rotateCircleMoved(qreal angle){
    emit this->rotateAngleChanged(angle);
}

void EdgesExtractionScene::itemPositionChanged(){
    emit this->positionChanged();
}
