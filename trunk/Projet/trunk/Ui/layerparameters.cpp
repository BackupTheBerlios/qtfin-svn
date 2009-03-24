#include "layerparameters.h"

LayerParameters::LayerParameters(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
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
