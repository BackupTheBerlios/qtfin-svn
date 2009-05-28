#ifndef DRAWPREVIEWVIEW_H
#define DRAWPREVIEWVIEW_H

#include <QGraphicsview>
#include "../Data/ProjectFile.h"

class DrawPreviewView : public QGraphicsView
{
public:
    DrawPreviewView(QWidget* parent, Data::ProjectFile* preview, qreal width, qreal height);

    void reScale();

protected:
    Data::ProjectFile* _preview;

    qreal _initWidth;
    qreal _initHeight;

    qreal _sceneWidth;
    qreal _sceneHeight;

    qreal _scale;

    virtual void drawBackground(QPainter *painter, const QRectF &rect);

    void resizeEvent(QResizeEvent *event);

};

#endif // DRAWPREVIEWVIEW_H
