#ifndef LAYERVIEW_H
#define LAYERVIEW_H

#include "layerrowitem.h"

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
    qreal getBiggerLength() const;

    void addLayerItem(int rank, qreal height, qreal length);
    void removeLayerItem(int rank);

protected slots:
    void updateLayerHeight(qreal height, qreal old);
    void updateLayerLength(qreal length, qreal old);

private:
    void createLayerEditionRow(int row, qreal height, qreal length);
    void updateLayersHeight() {}
    void updateLayersLength();

private:
    Data::ProjectFile *_struct;
    QVector<LayerRowItem*> _layers;
    QVBoxLayout *_layout;

    qreal _totalHeight;
    qreal _maxLength;
};

#endif // LAYERVIEW_H
