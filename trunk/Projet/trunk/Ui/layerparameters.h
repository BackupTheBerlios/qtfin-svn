#ifndef LAYERPARAMETERS_H
#define LAYERPARAMETERS_H

#include "../ui_layerParameters.h"

class LayerParameters : public QWidget, public Ui::LayerParameters {
    Q_OBJECT
    Q_DISABLE_COPY(LayerParameters)
public:
    explicit LayerParameters(QWidget *parent = 0);

public slots:
    void updateDensity(qreal density) { densityDoubleSpinBox->setValue(density); }
    void updatePoissonRatio(qreal poisson) { poissonDoubleSpinBox->setValue(poisson); }
    void updateYoungModulus(qreal modulus) { youngDoubleSpinBox->setValue(modulus); }

protected:
    virtual void changeEvent(QEvent *e);
};

#endif // LAYERPARAMETERS_H
