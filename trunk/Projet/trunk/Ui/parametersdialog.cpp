#include "parametersdialog.h"

#include "../Data/layer.h"
#include "layerparameters.h"

#include <QtCore/QDebug>
#include <QFileDialog>

using namespace Data;

ParametersDialog::ParametersDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    layerTabWidget->clear();

    setConnections();
    _retranslateUi();
}

void ParametersDialog::setNbLayers(int nb)
{
    qDebug() << QString("ParametersDialog::setNbLayers(%1)").arg(nb);
    int count = layerTabWidget->count();
    if (nb > count) {
        while(nb-- > count) {
            layerTabWidget->addTab(new LayerParameters, tr("layer %1").arg(nb));
            qDebug() << QString("add LayerParameters %1").arg(nb);
        }
    } else if (nb < count) {
        while(nb++ < count) {
            layerTabWidget->removeTab(nb);
            qDebug() << QString("remove LayerParameters %1").arg(nb);
        }
    }
    _retranslateUi();
}

void ParametersDialog::setConnections()
{
}

void ParametersDialog::_retranslateUi()
{
    for(int i=0; i<layerTabWidget->count(); ++i)
        layerTabWidget->setTabText(i, tr("layer %1").arg(i));
    retranslateUi(this);
}

void ParametersDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        _retranslateUi();
        break;
    default:
        break;
    }
}
