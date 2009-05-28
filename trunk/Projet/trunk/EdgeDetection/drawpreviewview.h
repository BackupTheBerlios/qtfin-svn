#ifndef DRAWPREVIEWVIEW_H
#define DRAWPREVIEWVIEW_H

#include <QGraphicsview>
#include "../Data/ProjectFile.h"

class DrawPreviewView : public QGraphicsView
{
public:
    DrawPreviewView(QWidget* parent, Data::ProjectFile* preview, qreal width, qreal height, qreal scale);

protected:
    Data::ProjectFile* _preview;
    qreal _scale;

    virtual void drawBackground(QPainter *painter, const QRectF &rect);

};

#endif // DRAWPREVIEWVIEW_H
