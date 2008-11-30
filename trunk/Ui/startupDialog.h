#ifndef STARTUPDIALOG_H
#define STARTUPDIALOG_H

#include <QtGui/QDialog>
#include "ui_startupDialog.h"

class StartupDialog : public QDialog
{
    Q_OBJECT

public:
    StartupDialog(QWidget *parent = 0);
    ~StartupDialog();

    // TODO: remove these dirty functions
    QWidget* getButton1();
    QWidget* getButton2();

private:
    Ui::StartupDialogClass ui;
};

#endif // STARTUPDIALOG_H
