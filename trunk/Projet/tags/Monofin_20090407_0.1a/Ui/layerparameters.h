#ifndef LAYERPARAMETERS_H
#define LAYERPARAMETERS_H

#include "../ui_layerParameters.h"

class LayerParameters : public QWidget, public Ui::LayerParameters {
    Q_OBJECT
    Q_DISABLE_COPY(LayerParameters)
public:
    explicit LayerParameters(QWidget *parent = 0);

protected:
    virtual void changeEvent(QEvent *e);
};

#endif // LAYERPARAMETERS_H
