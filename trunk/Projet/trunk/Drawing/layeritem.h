#ifndef LAYERITEM_H
#define LAYERITEM_H

#include <QtGui/QPixmap>
#include <QtGui/QWidget>

class QPainter;

class LayerItem : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal heightRatio READ heightRatio WRITE setHeightRatio);
    Q_PROPERTY(qreal lengthRatio READ lengthRatio WRITE setLengthRatio);
public:
    LayerItem(Qt::BrushStyle brushStyle = Qt::DiagCrossPattern, qreal heightRatio = 1.0, qreal lengthRatio = 1.0/*, QObject *parent = 0*/);
    virtual ~LayerItem();

    qreal heightRatio() { return _heightRatio; }
    qreal lengthRatio() { return _lengthRatio; }
    void setHeightRatio(qreal heightRatio) { _heightRatio = heightRatio; }
    void setLengthRatio(qreal lengthRatio) { _lengthRatio = lengthRatio; }
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent* event);

private:
    void drawLayer(QPainter *painter);
    void refreshPixmap();
    void retranslateUi();

private:
    enum { Margin = 20 };

    Qt::BrushStyle _brushStyle;
    qreal _heightRatio;
    qreal _lengthRatio;
    QPixmap _pixmap;
};

#endif // LAYERITEM_H