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
    /**
     * constructor
     * @param parent : the parent of the scene
     */
    EdgesExtractionScene(QWidget* parent, qreal width, qreal height);

    /**
     * getter
     * @return the coordinate Y of the axe of symetry
     */
    qreal symetryAxe();

    /**
     * getter
     * @return the coordinate X of the heal
     */
    qreal heal();

    /**
     * setter
     * @param pixmap : the image (.png or .bmp) wich is integrated into the scene
     */
    void setPixmap(QPixmap pixmap);

    /**
     * function that rotate the image along the "rotate circle"
     * @param angle : the angle of rotation
     */
    void rotateCircleMoved(qreal angle);

    /**
     * function that emit a signal informing that the image is moving
     */
    void itemPositionChanged();

    /**
     * function that emit a signal informing that the image is rescaling
     */
    void itemScaleChanged();

    /**
     * getter
     * @return the image wich is in the scene
     */
    PixmapItem* pixItem();

    /**
     * getter
     * @return a pointer on the "rotate circle"
     */
    RotateCircle* rotCircle();

signals:
    /**
     * signal informing that the image is rotating
     * param angle : the angle of rotation
     */
    void rotateAngleChanged(double angle);

    /**
     * signal informing that the image is moving
     */
    void positionChanged();

    /**
     * signal informing that the image is rescaling
     */
    void scaleChanged();


protected:
    PixmapItem* _pixItem; // the image of the scene
    RotateCircle* _rotCircle; // an item allowing to rotate the image
    qreal _symetryAxe; //the position of the axe of symetry
    qreal _heal;//the position of the heal ("red zone")
};

inline PixmapItem* EdgesExtractionScene::pixItem(){
        return _pixItem;
}

inline RotateCircle* EdgesExtractionScene::rotCircle(){
        return _rotCircle;
}

inline qreal EdgesExtractionScene::symetryAxe(){
    return _symetryAxe;
}

inline qreal EdgesExtractionScene::heal(){
    return _heal;
}

#endif // EDGESEXTRACTIONSCENE_H
