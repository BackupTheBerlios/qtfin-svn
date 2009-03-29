#include "parametersdialog.h"

#include "../Data/stratefile.h"
#include "layerparameters.h"

#include <QFileDialog>

ParametersDialog::ParametersDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    for(int i=0; i<4; ++i)
        m_strates.append(new StrateFile);

    layerTabWidget->clear();
    for(int i=0; i<m_strates.count(); ++i)
        layerTabWidget->addTab(new LayerParameters, QString("layer %1").arg(i));

    setConnections();
}

void ParametersDialog::setConnections()
{
    connect(browseButton, SIGNAL(clicked()), this, SLOT(chooseFile()));
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
        retranslateUi(this);
        break;
    default:
        break;
    }
}
