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
        : QWidget(parent), _struct(structure), _totalHeight(0), _totalLength(0)
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
        _totalHeight += height;
        _totalLength += length;
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

    _totalHeight += height;
    _totalLength += length;
    // add a layer into the data structure
    _struct->startHistory(MonofinLayer);
    _struct->addLayer(rank, length, height);
    _struct->stopHistory(MonofinLayer);
    createLayerEditionRow(rank, height, length);
}

void LayerView::removeLayerItem(int rank)
{
    qDebug() << QString("LayerView::removeLayerItem(%1)").arg(rank);
    _totalHeight -= _heightDoubleSpinBoxes.at(rank)->value();
    _totalLength -= _lengthDoubleSpinBoxes.at(rank)->value();

    // remove the layer
    if (!_layout->takeAt(rank))
        qDebug("no item to be removed");
    _layers.remove(rank);
    _heightDoubleSpinBoxes.remove(rank);
    _lengthDoubleSpinBoxes.remove(rank);

    // remove the layer from the data structure
    _struct->startHistory(MonofinLayer);
    _struct->removeLayer(rank);
    _struct->stopHistory(MonofinLayer);
}

// PROTECTED

void LayerView::updateLayerHeight()
{
    QDoubleSpinBox *heightSB = static_cast<QDoubleSpinBox*>(QObject::sender());
    int rank;
    qreal height;
    if(heightSB) {
        height = heightSB->value();
        rank = _heightDoubleSpinBoxes.indexOf(heightSB);

        qDebug() << "LayerView::updateLayerHeight()" << "height:" << height;
        _totalHeight -= _struct->getLayerHeight(rank);
        _struct->startHistory(MonofinLayer);
        _struct->setLayerHeight(rank, height);
        _struct->stopHistory(MonofinLayer);

        // update values
        height = _struct->getLayerHeight(rank);
        heightSB->setValue(height);
        _totalHeight += height;
        _layout->setStretch(rank, qRound(height));

        // BUG HERE
    }
}


void LayerView::updateLayerLength()
{
    QDoubleSpinBox *lengthSB = static_cast<QDoubleSpinBox*>(QObject::sender());
    int rank;
    qreal length;
    if(lengthSB) {
        length = lengthSB->value();
        rank = _lengthDoubleSpinBoxes.indexOf(lengthSB);

        qDebug() << "LayerView::updateLayerLength()" << "length:" << length;
        _totalLength -= _struct->getLayerLength(rank);
        _struct->startHistory(MonofinLayer);
        _struct->setLayerLength(rank, length);
        _struct->stopHistory(MonofinLayer);

        // update values
        length = _struct->getLayerLength(rank);
        lengthSB->setValue(length);
        _totalLength += length;
        _layers.at(rank)->setLengthRatio(length/_totalLength);
        _layers.at(rank)->update();

        // BUG HERE
    }
}

// PRIVATE

void LayerView::createLayerEditionRow(int row, qreal height, qreal length)
{
    qDebug() << "LayerView::createLayerEditionRow(" << row << ")";
    QDoubleSpinBox *heightDoubleSpinBox = new QDoubleSpinBox;
    heightDoubleSpinBox->setValue(height);
    heightDoubleSpinBox->setSuffix(QApplication::translate("LayerView", " cm", "centimeters", QApplication::UnicodeUTF8));
    QDoubleSpinBox *lengthDoubleSPinBox = new QDoubleSpinBox;
    lengthDoubleSPinBox->setValue(length);
    lengthDoubleSPinBox->setMaximum(9999.99);
    lengthDoubleSPinBox->setSuffix(QApplication::translate("LayerView", " cm", "centimeters", QApplication::UnicodeUTF8));
    QLabel *heightLabel = new QLabel("height");
    QLabel *lengthLabel = new QLabel("length");
    QHBoxLayout *hbLayout = new QHBoxLayout;
    hbLayout->setObjectName(QString("LayerView::hbLayout%1").arg(row));
    LayerItem *layerItem = new LayerItem((Qt::BrushStyle)(row%4+Qt::CrossPattern), 1.0, length/_totalLength);

    connect(heightDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateLayerHeight()));
    connect(lengthDoubleSPinBox, SIGNAL(valueChanged(double)), this, SLOT(updateLayerLength()));

    _heightDoubleSpinBoxes.insert(row, heightDoubleSpinBox);
    _lengthDoubleSpinBoxes.insert(row, lengthDoubleSPinBox);
    _layers.insert(row, layerItem);

    hbLayout->addWidget(heightLabel, 0, Qt::AlignRight);
    hbLayout->addWidget(heightDoubleSpinBox, 0, Qt::AlignLeft);
    hbLayout->addSpacing(20);
    hbLayout->addWidget(lengthLabel, 0, Qt::AlignRight);
    hbLayout->addWidget(lengthDoubleSPinBox, 0, Qt::AlignLeft);
    hbLayout->addSpacing(20);
    hbLayout->addWidget(layerItem);

    _layout->insertLayout(row, hbLayout, qRound(height));
    layerItem->show();
}


// TIP
// utiliser les valeurs de strech vertical pour obtenir les bonnes proportions de hauteur
