#include "parametersdialog.h"

#include "../Data/layer.h"
#include "layerparameters.h"

#include <QFileDialog>

using namespace Data;

ParametersDialog::ParametersDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    // a refaire
    for(int i=0; i<4; ++i)
        m_strates.append(new Layer);

    layerTabWidget->clear();
    // a refaire
    for(int i=0; i<m_strates.count(); ++i)
        layerTabWidget->addTab(new LayerParameters, tr("layer %1").arg(i));

    setConnections();
    _retranslateUi();
}

void ParametersDialog::setConnections()
{
    connect(browseButton, SIGNAL(clicked()), this, SLOT(chooseFile()));
}

void ParametersDialog::_retranslateUi()
{
    for(int i=0; i<layerTabWidget->count(); ++i)
        layerTabWidget->setTabText(i, tr("layer %1").arg(i));
    retranslateUi(this);
}

void ParametersDialog::chooseFile()
{
    QString dir;
    if(QFile::exists(pathLineEdit->text()))
        dir = pathLineEdit->text();
    QString str = QFileDialog::getOpenFileName(this, QString(), dir);
    if(!str.isEmpty())
        pathLineEdit->setText(str);
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
