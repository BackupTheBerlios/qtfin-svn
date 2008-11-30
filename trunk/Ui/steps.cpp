#include <QtGui/QFileDialog>
#include "steps.h"

Steps::Steps(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
}

Steps::~Steps()
{

}

void Steps::showme()
{
	this->show();
}

void Steps::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Ouvrir un fichier"), "/", "*.fin");
	showme();
}
