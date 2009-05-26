#include "layerindexdialog.h"

LayerIndexDialog::LayerIndexDialog(QWidget *parent) :
    QDialog(parent){
    setupUi(this);
}

void LayerIndexDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
