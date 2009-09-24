#include "insertlayerdialog.h"

InsertLayerDialog::InsertLayerDialog(QWidget *parent) :
    QDialog(parent){
    setupUi(this);
}

void InsertLayerDialog::changeEvent(QEvent *e)
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
