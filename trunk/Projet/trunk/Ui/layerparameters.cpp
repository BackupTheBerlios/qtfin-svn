#include "layerparameters.h"

LayerParameters::LayerParameters(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

void LayerParameters::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
