#include "startupDialog.h"

StartupDialog::StartupDialog(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
}

StartupDialog::~StartupDialog()
{

}

// TODO: to be removed
QWidget* StartupDialog::getButton1()
{
	return ui.buttonNewProject;
}

// TODO: to be removed
QWidget* StartupDialog::getButton2()
{
	return ui.buttonOpenProject;
}
