#include <QPen>
#include <math.h>
#include "edgesextractionscene.h"

EdgesExtractionScene::EdgesExtractionScene(QWidget* parent, qreal width, qreal height):
        QGraphicsScene(parent), _pixItem(NULL)
{
    this->setSceneRect(QRectF(0,0,width,height));

    _symetryAxe = height / 2;
    _heal = width / 10;

    QGraphicsLineItem* ligne1 = this->addLine(0, _symetryAxe, width, _symetryAxe);
    ligne1->setPen(QPen(QBrush("red"), 3));
    ligne1->setZValue(3);

    QGraphicsRectItem* rect1 = addRect(0, 0, _heal, height);
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

    qreal radius = sqrt(pixmap.width() * pixmap.width() +
                        pixmap.height() * pixmap.height()) / 2;
    SCircle* scircle = new SCircle(pixmap.rect().center().x(),
                                   pixmap.rect().center().y(),
                                   radius, _pixItem);

    scircle->addSPoint(100);
    _pixItem->setSCircle(scircle);

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

void EdgesExtractionScene::itemScaleChanged(){
    emit this->scaleChanged();
}
