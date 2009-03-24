#ifndef STARTUPDIALOG_H
#define STARTUPDIALOG_H

#include <QtGui/QDialog>

namespace Ui
{
    class StartupDialogClass;
}

class StartupDialog : public QDialog
{
    Q_OBJECT
public:
    StartupDialog(QWidget *parent = 0);
    ~StartupDialog();

signals:
    void newProject();
    void imageProject();
    void openProject();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::StartupDialogClass *ui;
};

#endif // STARTUPDIALOG_H
