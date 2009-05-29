#include "layerview.h"
#include "Data/projectfile.h"

#include <QtCore/QDebug>
#include <QtGui/QApplication>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>

#include <QtDebug>

using namespace Data;

LayerView::LayerView(ProjectFile *structure, QWidget *parent)
    : QWidget(parent), _struct(structure), _totalHeight(0), _maxLength(0)
{
    setObjectName("LayerWiew");
    setAttribute(Qt::WA_DeleteOnClose);

    _layout = new QVBoxLayout(this);
    _layout->setObjectName("LayerView::_layout");
    _layout->setSpacing(0);

    // a mieux definir ou a changer dynamiquement selon le nombre de strates
    setMaximumHeight(400);

    // get data from the structure
    int i;
    for (i=0; i<_struct->getHowManyLayers(); ++i) {
        qreal height = _struct->getLayerHeight(i);
        qreal length = _struct->getLayerLength(i);
        _totalHeight += height; // obsolete
        _maxLength = qMax(length, _maxLength);
        createLayerEditionRow(i, height, length);
    }
    if (i==0)
        addLayerItem(0, 4, 100);

    setLayout(_layout);
}

LayerView::~LayerView()
{
    qDebug("LayerView::~LayerView()");
}

int LayerView::nbLayers()
{
    qDebug("LayerView::nbLayers()");
    return _layers.count();
}

QSize LayerView::sizeHint() const
{
    return _layout->sizeHint();
}

void LayerView::addLayerItem(int rank, qreal height, qreal length)
{
    qDebug() << QString("LayerView::addLayerItem(rank: %1, height: %2, length: %3").arg(rank).arg(height).arg(length);
    if (rank < 0 || height < 0 || length < 0)
        return;

    _totalHeight += height; // obsolete
    _maxLength = qMax(length, _maxLength);
    // add a layer into the data structure
    _struct->startHistory(MonofinLayer);
    _struct->addLayer(rank, length, height);
    _struct->stopHistory(MonofinLayer);
    createLayerEditionRow(rank, height, length);
}

void LayerView::removeLayerItem(int rank)
{
    qDebug() << QString("LayerView::removeLayerItem(%1)").arg(rank);
    _totalHeight -= _layers.at(rank)->layerHeight(); // obsolete

    // remove the layeritem
    LayerRowItem *item = _layers.at(rank);
    _layout->removeWidget(item);
    _layers.remove(rank);
    if(item)
        item->deleteLater();
    qDebug("nb layers : %d", _layers.count());

    // remove the layer from the data structure
    _struct->startHistory(MonofinLayer);
    _struct->removeLayer(rank);
    _struct->stopHistory(MonofinLayer);

    _maxLength = getBiggerLength();
    updateLayersHeight();
    updateLayersLength();
}

// PROTECTED

void LayerView::updateLayerHeight(qreal height, qreal old)
{
    qDebug() << QString("LayerView::updateLayersHeight(%1, %2)").arg(height).arg(old);
    LayerRowItem *sender = static_cast<LayerRowItem*>(QObject::sender());
    int rank;
    if(sender) {
        rank = _layers.indexOf(sender);

        _totalHeight -= old; // obsolete
        _struct->startHistory(MonofinLayer);
        _struct->setLayerHeight(rank, height);
        _struct->stopHistory(MonofinLayer);

        // update values
        height = _struct->getLayerHeight(rank);
        sender->setHeight(height);
        _totalHeight += height; // obsolete
        _layout->setStretch(rank, qRound(height));
    }
}


void LayerView::updateLayerLength(qreal length, qreal old)
{
    qDebug() << QString("LayerView::updateLayersLength(%1, %2)").arg(length).arg(old);
    LayerRowItem *sender = static_cast<LayerRowItem*>(QObject::sender());
    int rank;
    if(sender) {
        rank = _layers.indexOf(sender);

        _struct->startHistory(MonofinLayer);
        _struct->setLayerLength(rank, length);
        _struct->stopHistory(MonofinLayer);

        // update values
        length = _struct->getLayerLength(rank);
        sender->setLength(length);
        _maxLength = getBiggerLength();
        updateLayersLength();
    }
}

// PRIVATE

void LayerView::createLayerEditionRow(int row, qreal height, qreal length)
{
    qDebug() << "LayerView::createLayerEditionRow(" << row << ")";
    LayerRowItem *layerRowItem = new LayerRowItem(height, length, length/_maxLength);

    connect(layerRowItem, SIGNAL(layerHeightUpdated(qreal,qreal)), this, SLOT(updateLayerHeight(qreal,qreal)));
    connect(layerRowItem, SIGNAL(layerLengthUpdated(qreal,qreal)), this, SLOT(updateLayerLength(qreal,qreal)));

    _layers.insert(row, layerRowItem);
    _layout->insertWidget(row, layerRowItem, qRound(height));

    layerRowItem->show();

    updateLayersHeight();
    updateLayersLength();
}

void LayerView::updateLayersLength()
{
    foreach(LayerRowItem *item, _layers) {
        item->setLengthRatio(item->layerLength()/_maxLength);
    }
}

qreal LayerView::getBiggerLength() const
{
    qreal max = 0;
    foreach(LayerRowItem *item, _layers) {
        max = qMax(max, item->layerLength());
    }
    return max;
}
