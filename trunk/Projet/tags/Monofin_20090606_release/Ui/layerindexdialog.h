#ifndef LAYERINDEXDIALOG_H
#define LAYERINDEXDIALOG_H

#include "ui_layerindexdialog.h"

class LayerIndexDialog : public QDialog, private Ui::LayerIndexDialog {
    Q_OBJECT
    Q_DISABLE_COPY(LayerIndexDialog)
public:
    explicit LayerIndexDialog(QWidget *parent = 0);
    void setNbLayer(int nb) {
        if(nb > 1) rankSpinBox->setMaximum(nb-1);
        else rankSpinBox->setMaximum(0);
    }
    int getLayerRank() { return rankSpinBox->value(); }

protected:
    virtual void changeEvent(QEvent *e);
};

#endif // LAYERINDEXDIALOG_H
