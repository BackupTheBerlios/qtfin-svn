#ifndef INSERTLAYERDIALOG_H
#define INSERTLAYERDIALOG_H

#include "ui_insertlayerdialog.h"

class InsertLayerDialog : public QDialog, private Ui::insertLayerDialog {
    Q_OBJECT
    Q_DISABLE_COPY(InsertLayerDialog)
public:
    explicit InsertLayerDialog(QWidget *parent = 0);
    void setNbLayer(int nb) {
        if(nb > 1) rankSpinBox->setMaximum(nb-1);
        else rankSpinBox->setMaximum(0);
    }
    int getLayerRank() { return rankSpinBox->value(); }
    qreal getLayerHeight() { return heightDoubleSpinBox->value(); }
    qreal getLayerLength() { return lengthDoubleSpinBox->value(); }
    int before() { return insertBeforeCheckBox->isChecked()?0:1; }

protected:
    virtual void changeEvent(QEvent *e);
};

#endif // INSERTLAYERDIALOG_H
