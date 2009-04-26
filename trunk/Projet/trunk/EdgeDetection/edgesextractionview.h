#ifndef EDGESEXTRACTIONVIEW_H
#define EDGESEXTRACTIONVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include "edgesextractionscene.h"

class EdgesExtractionView : public QGraphicsView
{
public:
    EdgesExtractionView(EdgesExtractionScene* scene, QWidget* parent = NULL);

protected:
    EdgesExtractionScene* _scene;

};

#endif
