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
     * @return the "SCircle" created by the pixmapItem
     */
    SCircle* scircle();

    /**
     * getter
     * @return the scale of the "pixmapItem"
     */
    qreal getScale();

    /**
     * getter
     * @return the angle of rotation of the pixmap
     */
    qreal rotationAngle();

    /**
     * setter
     * @param scircle : the "SCircle" of the pixmap
     */
    void setSCircle(SCircle* scircle);

    /**
     * getter
     * @return true if the mouse is in the bounding rectangle of the item else return false
     */
    bool isEnter();

    /**
     * getter
     * @return true if the pixmapItem is used for an algo
     */
    bool isForAlgo();

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

    bool _enter; // boolean that is true when the mouse is in the bounding rectangle of the item

    bool _isForAlgo;

    /**
     * different overloaded functions that handle the mouse events
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event );
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
    virtual void wheelEvent ( QGraphicsSceneWheelEvent * event );
};

inline qreal PixmapItem::rotationAngle(){
        return _rotateAngl;
}

inline SCircle* PixmapItem::scircle(){
        return _scircle;
}

inline void PixmapItem::setSCircle(SCircle* scircle){
    _scircle = scircle;
    _isForAlgo = true;
}

inline bool PixmapItem::isEnter(){
    return _enter;
}

inline bool PixmapItem::isForAlgo(){
    return _isForAlgo;
}

inline qreal PixmapItem::getScale(){
    return _scale;
}


#endif // PIXMAPITEM_H
