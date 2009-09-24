#include "layerrowitem.h"

#include <QtCore/QDebug>
#include <QtGui/QApplication>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>

LayerRowItem::LayerRowItem(qreal height, qreal length, qreal lengthRatio, QWidget *parent)
    : QWidget(parent), _height(height), _length(length), _lengthRatio(lengthRatio)
{
    _heightDoubleSpinBox = new QDoubleSpinBox(this);
    _heightDoubleSpinBox->setValue(height);
    _heightDoubleSpinBox->setSuffix(QApplication::translate("LayerRowItem", " cm", "centimeters", QApplication::UnicodeUTF8));
    _lengthDoubleSpinBox = new QDoubleSpinBox(this);
    _lengthDoubleSpinBox->setMaximum(9999.99);
    _lengthDoubleSpinBox->setValue(length);
    _lengthDoubleSpinBox->setSuffix(QApplication::translate("LayerRowItem", " cm", "centimeters", QApplication::UnicodeUTF8));
    _heightLabel = new QLabel("height");
    _lengthLabel = new QLabel("length");
    _hbLayout = new QHBoxLayout(this);
    _hbLayout->setContentsMargins(2, 2, 2, 2);
    _layerItem = new LayerItem(Qt::LinearGradientPattern, 1.0, lengthRatio);

    connect(_heightDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateHeight(double)));
    connect(_lengthDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateLength(double)));

    _hbLayout->addWidget(_heightLabel, 0, Qt::AlignRight);
    _hbLayout->addWidget(_heightDoubleSpinBox, 0, Qt::AlignLeft);
    _hbLayout->addSpacing(20);
    _hbLayout->addWidget(_lengthLabel, 0, Qt::AlignRight);
    _hbLayout->addWidget(_lengthDoubleSpinBox, 0, Qt::AlignLeft);
    _hbLayout->addSpacing(20);
    _hbLayout->addWidget(_layerItem);
}

void LayerRowItem::updateHeight(double height)
{
    qDebug() << QString("LayerRowItem::updateHeight(%1)").arg(height);
    // update spinboxes
    if (height != _heightDoubleSpinBox->value())
        _heightDoubleSpinBox->setValue(height);

    if (_height != height) {
        // update local value
        double old = _height;
        _height = height;

        // signal
        qDebug() << QString("emit layerHeightUpdated(new:%1, old:%2)").arg(height).arg(old);
        emit layerHeightUpdated(height, old);
    }
}

void LayerRowItem::updateLength(double length)
{
    qDebug() << QString("LayerRowItem::updateLength(%1)").arg(length);
    // update spinboxes
    if (length != _lengthDoubleSpinBox->value())
        _lengthDoubleSpinBox->setValue(length);

    if (_length != length) {
        // update local value
        double old = _length;
        _length = length;

        // signal
        qDebug() << QString("emit layerLengthUpdated(new:%1, old:%2)").arg(length).arg(old);
        emit layerLengthUpdated(length, old);
    }
}

void LayerRowItem::updateLengthRatio(double lengthRatio)
{
    qDebug() << QString("LayerRowItem::updateLengthRatio(%1)").arg(lengthRatio);
    // update LayerItem
    if (lengthRatio != _layerItem->lengthRatio())
        _layerItem->setLengthRatio(lengthRatio);
}
