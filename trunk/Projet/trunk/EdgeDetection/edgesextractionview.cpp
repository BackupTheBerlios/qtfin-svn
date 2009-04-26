#include "edgesextractionview.h"

EdgesExtractionView::EdgesExtractionView(EdgesExtractionScene* scene, QWidget* parent):
        QGraphicsView(scene, parent), _scene(scene)
{
}
