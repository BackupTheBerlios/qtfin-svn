#include <QtGui/QMessageBox>
#include "mainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
	QObject::connect(ui.actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	QObject::connect(ui.action_About_Monofin, SIGNAL(triggered()), this, SLOT(about()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::about()
{
    QMessageBox::information(this, tr("About Monofin 0.1"),
                      tr("Monofin 0.1\n\n"
                      "Copyright 2008 INSA de Rennes, Inc."));
}

void MainWindow::showme()
{
	this->show();
}

