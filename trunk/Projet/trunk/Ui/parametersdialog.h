#ifndef PARAMETERSDIALOG_H
#define PARAMETERSDIALOG_H

#include "../ui_parametersDialog.h"
#include "layerparameters.h"

class ParametersDialog : public QDialog, public Ui::ParametersDialog {
    Q_OBJECT
    Q_DISABLE_COPY(ParametersDialog)
public:
    explicit ParametersDialog(QWidget *parent = 0);

    void setNbLayers(int nb);
    inline void updateLayerDensity(int layer, qreal density);
    inline void updateLayerPoissonRatio(int layer, qreal poisson);
    inline void updateLayerYoungModulus(int layer, qreal modulus);

protected:
    virtual void changeEvent(QEvent *e);

private:
    void setConnections();
    void _retranslateUi();
};

inline void ParametersDialog::updateLayerDensity(int layer, qreal density)
{
    if (layer < layerTabWidget->count()) {
        static_cast<LayerParameters*>(layerTabWidget->widget(layer))->updateDensity(density);
    }
}

inline void ParametersDialog::updateLayerPoissonRatio(int layer, qreal poisson)
{
    if (layer < layerTabWidget->count()) {
        static_cast<LayerParameters*>(layerTabWidget->widget(layer))->updatePoissonRatio(poisson);
    }
}

inline void ParametersDialog::updateLayerYoungModulus(int layer, qreal modulus)
{
    if (layer < layerTabWidget->count()) {
        static_cast<LayerParameters*>(layerTabWidget->widget(layer))->updateYoungModulus(modulus);
    }
}

#endif // PARAMETERSDIALOG_H
