#ifndef PARAMETERSDIALOG_H
#define PARAMETERSDIALOG_H

#include "../ui_parametersDialog.h"

class StrateFile;

class ParametersDialog : public QDialog, public Ui::ParametersDialog {
    Q_OBJECT
    Q_DISABLE_COPY(ParametersDialog)
public:
    explicit ParametersDialog(QWidget *parent = 0);

protected:
    virtual void changeEvent(QEvent *e);

private:
    QList<StrateFile*> m_strates;
};

#endif // PARAMETERSDIALOG_H
