#ifndef EDGESEXTRACTIONVIEW_H
#define EDGESEXTRACTIONVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include "edgesextractionscene.h"

class EdgesExtractionView : public QGraphicsView
{
public:

    /**
     * constructor
     * the EdgesExtractionView object contains only one scene
     * there is no operations done on the view
     * @param scene : the scene contained in the "GraphicView"
     * @param parent : the parent of the "GraphicView"
     */
    EdgesExtractionView(EdgesExtractionScene* scene, QWidget* parent = NULL);

    /**
     * rescale the view
     */
    void reScale();

protected:
    
    EdgesExtractionScene* _scene; // the scene contained in the view    
    qreal _initHeight; //the initial height of the view
    qreal _initWidth; //the initial width of the view
    qreal _scale; //the scale of the view

    /**
     * see the Qt Reference Documentation
     */
    void resizeEvent(QResizeEvent *event);
    /**
     * see the Qt Reference Documentation
     */
    virtual void drawForeground(QPainter *painter, const QRectF &rect);

};

#endif
