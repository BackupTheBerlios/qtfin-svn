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

    enum { empty=2,
           image=3,
           open=4
       };

protected:
    virtual void changeEvent(QEvent *e);
    virtual bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void imageProject() { done(image); }
    void newEmptyProject() { done(empty); }
    void openProject() { done(open); }
    void retranslateUi();

private:
    Ui::StartupDialogClass *ui;
};

#endif // STARTUPDIALOG_H
