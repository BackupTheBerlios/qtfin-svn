#ifndef LAYERROWITEM_H
#define LAYERROWITEM_H

#include <QtGui/QDoubleSpinBox>
#include <QtGui/QWidget>

#include "layeritem.h"

class QHBoxLayout;
class QLabel;

class LayerRowItem : public QWidget
{
    Q_OBJECT
public:
    LayerRowItem(qreal height, qreal length, qreal lengthRatio, QWidget *parent=0);

    qreal layerHeight() const { return _heightDoubleSpinBox->value(); }
    qreal layerLength() const { return _lengthDoubleSpinBox->value(); }
    void setHeight(qreal height) { updateHeight(height); }
    void setLength(qreal length) { updateLength(length); }
    void setLengthRatio(qreal lengthRatio) { updateLengthRatio(lengthRatio); }

signals:
    void layerHeightUpdated(qreal,qreal);
    void layerLengthUpdated(qreal,qreal);

private slots:
    void updateHeight(double height);
    void updateLength(double length);
    void updateLengthRatio(double lengthRatio);

private:
    qreal _height;
    qreal _length;
    qreal _lengthRatio;
    LayerItem *_layerItem;

    QDoubleSpinBox *_heightDoubleSpinBox;
    QDoubleSpinBox *_lengthDoubleSpinBox;
    QLabel *_heightLabel;
    QLabel *_lengthLabel;
    QHBoxLayout *_hbLayout;
};

#endif // LAYERROWITEM_H
