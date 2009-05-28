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

    void reScale();
    qreal getScale();

protected:
    //void resizeEvent(QResizeEvent *event);
    EdgesExtractionScene* _scene; // the scene contained in the view

    qreal _initHeight;
    qreal _initWidth;

};

#endif
