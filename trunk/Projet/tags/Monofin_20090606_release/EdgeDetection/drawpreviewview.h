#ifndef DRAWPREVIEWVIEW_H
#define DRAWPREVIEWVIEW_H

#include <QGraphicsView>
#include "../Data/projectfile.h"

class DrawPreviewView : public QGraphicsView
{
public:
    /**
     * constructor
     * create a view wich displays a preview of the Bézier curves extracted by the algoedgesextraction
     * @param parent : the parent of the view
     * @param preview : the structure of the form to preview
     * @param width : the width of the scene
     * @param height : the height of the scene
     */
    DrawPreviewView(QWidget* parent, Data::ProjectFile* preview, qreal width, qreal height);

    /**
     * rescale the view along the size of the scene
     */
    void reScale();

    /**
     * setter
     * change the structure to preview
     * @param preview : the structure of the form to preview
     * @param width : the width of the scene
     * @param height : the height of the scene
     */
    void setProjectFile(Data::ProjectFile *monofin, qreal width = 0, qreal height = 0);

protected:
    Data::ProjectFile* _preview; //the structure to preview

    qreal _initWidth; //the initial width of the view
    qreal _initHeight;//the initial height of the view
    qreal _sceneWidth; //the width of the scene
    qreal _sceneHeight; //the height of the scene
    qreal _scale; // the scale of the view

    /**
     * see the Qt Reference Documentation
     */
    virtual void drawBackground(QPainter *painter, const QRectF &rect);

    /**
     * see the Qt Reference Documentation
     */
    void resizeEvent(QResizeEvent *event);

};

#endif // DRAWPREVIEWVIEW_H
