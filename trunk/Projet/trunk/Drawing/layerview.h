#ifndef LAYERVIEW_H
#define LAYERVIEW_H

#include "layeritem.h"

#include <QtCore/QVector>
#include <QtGui/QWidget>

namespace Data {
    class ProjectFile;
}

class QDoubleSpinBox;
class QVBoxLayout;

class LayerView : public QWidget
{
    Q_OBJECT
public:
    LayerView(Data::ProjectFile *structure, QWidget *parent = 0);
    virtual ~LayerView();

    int nbLayers();
    QSize sizeHint() const;

    void addLayerItem(int rank, qreal height, qreal length);
    void removeLayerItem(int rank);

protected slots:
    void updateLayerHeight();
    void updateLayerLength();

private:
    void createLayerEditionRow(int row, qreal height, qreal length);

private:
    Data::ProjectFile *_struct;
    QVector<QDoubleSpinBox*> _heightDoubleSpinBoxes;
    QVector<QDoubleSpinBox*> _lengthDoubleSpinBoxes;
    QVector<LayerItem*> _layers;
    QVBoxLayout *_layout;

    qreal _totalHeight;
    qreal _totalLength;
};

#endif // LAYERVIEW_H
