#include "edgesextractionview.h"
#include <QPainter>

EdgesExtractionView::EdgesExtractionView(EdgesExtractionScene* scene, QWidget* parent):
        QGraphicsView(scene, parent), _scene(scene)
{
}
