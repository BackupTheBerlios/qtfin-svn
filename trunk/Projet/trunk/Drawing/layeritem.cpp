#include "layeritem.h"

#include <QtCore/QDebug>
#include <QtGui/QApplication>
#include <QtGui/QPainter>

LayerItem::LayerItem(Qt::BrushStyle brushStyle, qreal heightRatio, qreal lengthRatio/*, QObject *parent*/)
        : /*QObject(parent),*/ _brushStyle(brushStyle), _heightRatio(heightRatio), _lengthRatio(lengthRatio)
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    refreshPixmap();
}

LayerItem::~LayerItem()
{
    qDebug("LayerItem::~LayerItem()");
}

QSize LayerItem::sizeHint() const
{
    return QSize(2 * Margin, Margin);
}

void LayerItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, _pixmap);
}

void LayerItem::resizeEvent(QResizeEvent* event)
{
    refreshPixmap();
}

// PRIVATE

void LayerItem::drawLayer(QPainter *painter)
{
    QRectF rect(0, 0,
               width()*_lengthRatio - 1, height()*_heightRatio - 1);
    if (!rect.isValid())
        return;

    painter->setBrush(_brushStyle);
    painter->drawRect(rect.toRect());
}

void LayerItem::refreshPixmap()
{
    _pixmap = QPixmap(size());
    _pixmap.fill(this, 0, 0);

    QPainter painter(&_pixmap);
    painter.initFrom(this);
    drawLayer(&painter);
    update();
}
