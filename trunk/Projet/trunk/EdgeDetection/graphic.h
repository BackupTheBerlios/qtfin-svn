#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <QApplication>
#include <QtGui/QWidget>
#include <QDialog>
#include "ui_graphic.h"

#include "edgesextractionscene.h"
#include "edgesextractionview.h"
#include "pixmapitem.h"
#include "rotatecircle.h"
#include "algosnake.h"
#include "drawpreview.h"
#include "../Drawing/paintingscene.h"




class Graphic : public QDialog {
    Q_OBJECT

public:
    /**
     * constructor
     * a graphic's object is a factory wich contains all of the elements to work on one image and to start the two algorithms
     * detecteting and extracting the edges of this image
     * it is also the graphic's users interface and put the slots and signals in relation
     * @param parent : the parent of the graphic
     */
    Graphic(QWidget *parent = 0, Data::ProjectFile* monofin = 0, qreal width = 0, qreal height = 0);
    ~Graphic();

    /**
     * setter
     * @param width : the width of the scene
     * @param height : the height of the scene
     */
    void setSize(qreal width, qreal height);


public slots:
    /**
     * displays an openWindow when the button "open image" is clicked
     * the user choose an image (.png or .bmp) and this image is integrated into the interface
     */
    void setPixmap();

    /**
     * rotates the images when the value of the spinBox "value" is changing
     * @param angle : the value of the spinBox and so the angle of rotation
     */
    void rotate(double angle);

    /**
     * changes the scale of the image when the value of spinBox "scale" is changing
     * @param scale : the value of the spinBox and so the sclae of the image (in %)
     */
    void setScale(double scale);

    /**
     * changes the value of the spinBox "positionX" and "positionY"
     * in function of the position of the image in the scene
     */
    void positionChanged();

    /**
     * changes the value of the spinBox scale
     * in function of the scale of the image in the scene
     */
    void scaleChanged();

    /**
     * set the position of the "Pixmapitem" to (x,y) in the scene's coordinate
     * @param x : the coordinate X
     */
    void setPixmapPositionX(int x);

     /**
     * set the position of the "Pixmapitem" to (x,y) in the scene's coordinate
     * @param y : the coordinate Y
     */
    void setPixmapPositionY(int y);

    /**
     * slot wich start the two algorithms (edges detection and extraction)
     */
    void startAlgo();

    /**
     * the draw detected by the algorithm is kept
     * close the graphic
     */
    void kept();

    /**
     * the draw detected by the algorithm is not kept
     * the graphic is still open
     */
    void doNotKept();


protected:
    Ui::Form _graphic; // the graphic's interface
    EdgesExtractionScene* _graphicsScene; // the scene wich contains all of the items (image, "rotateCircle",...)
    EdgesExtractionView* _graphicsView; // the view wich contains the scene (_graphicsScene)
    AlgoSnake* _algo; // object wich contains the two algorithms working on the opened image

    Data::ProjectFile* _monofin;
    DrawPreview* _preview;
};

#endif // GRAPHIC_H
