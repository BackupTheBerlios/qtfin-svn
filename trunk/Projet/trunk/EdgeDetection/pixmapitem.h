#ifndef PIXMAPITEM_H
#define PIXMAPITEM_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QWidget>
#include <QPainter>
#include <QRectF>
#include <QGraphicsSceneMouseEvent>
#include <QWheelEvent>
#include <QPointF>
#include "scircle.h"

class SCircle;

class PixmapItem : public QGraphicsPixmapItem
{
public:

    /**
     * constructor
     * create an item wich contains the image (.png or .bmp) and an "SCircle"
     * the item has to be include into a scene
     * @param pix : the image
     */
    PixmapItem(QPixmap pix = QPixmap());

    /**
     * destructor
     */
    ~PixmapItem();

    /**
     * getter
     * @return the angle of rotation of the item
     */
    qreal rotateAngle();

    /**
     * getter
     * @return the "SCircle" created by the pixmapItem
     */
    SCircle* scircle();

    /**
     * setter
     * @param scircle : the "SCircle" of the pixmap
     */
    void setSCircle(SCircle* scircle);

    /**
     * overloards the function paint of the class QGraphicsItem
     * it is called by the "graphicsview" to paint the item
     */
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    /**
     * translate the item in the scene
     * @param x : the translation along the axe X
     * @param y : the translation along the axe Y
     */
    void translate2(qreal x, qreal y);

    /**
     * set the value of the angle of rotation of the item
     * the angle is between -180 and 180
     * @param angle : the angle of rotation, if the angle is not between -180 and 180, the function change the angle
     */
    void rotate2(qreal angle);

    /**
     * change the scale of the item
     * @param scale : the scale of the item
     */
    void scaled(qreal scale);

    /**
     * center the item in the midle of the scene
     */
    void center();

public slots:


protected:
    SCircle* _scircle; // the "scircle" arround the item

    qreal X; // the widht of the image
    qreal Y; // the heigt of the image
    qreal dimInitWidth; // the intial width of the image
    qreal dimInitHeight; // the initial heigt of the image


    QPointF _posMousePress; // the position of the mouse when it is pressed
    QPointF _posMouseMove; // the position of the mouse

    qreal _rotateAngl; // the angle of rotation of the image
    qreal _scale; // the scale of rotation of the image

    bool enter; // boolean that is true when the mouse is in the bounding rectangle of the item

    bool _isForAlgo;

    /**
     * different overloaded functions that handle the mouse events
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event );
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
};

inline qreal PixmapItem::rotateAngle(){
        return _rotateAngl;
}

inline SCircle* PixmapItem::scircle(){
        return _scircle;
}

inline void PixmapItem::setSCircle(SCircle* scircle){
    _scircle = scircle;
    _isForAlgo = true;
}

#endif // PIXMAPITEM_H
