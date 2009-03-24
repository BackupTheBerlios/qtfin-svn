#include "parametersdialog.h"

#include "../Data/stratefile.h"
#include "layerparameters.h"

ParametersDialog::ParametersDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    for(int i=0; i<4; ++i)
        m_strates.append(new StrateFile);

    layerTabWidget->clear();
    for(int i=0; i<m_strates.length(); ++i)
        layerTabWidget->addTab(new LayerParameters, QString("layer %1").arg(i));
}

void ParametersDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
