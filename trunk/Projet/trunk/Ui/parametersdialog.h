#ifndef PARAMETERSDIALOG_H
#define PARAMETERSDIALOG_H

#include "../ui_parametersDialog.h"

namespace Data {
    class Layer;
}

class ParametersDialog : public QDialog, public Ui::ParametersDialog {
    Q_OBJECT
    Q_DISABLE_COPY(ParametersDialog)
public:
    explicit ParametersDialog(QWidget *parent = 0);

    void setNbLayers(int nb);

protected:
    virtual void changeEvent(QEvent *e);

private:
    void setConnections();
    void _retranslateUi();

private:
    QList<Data::Layer*> m_strates;
};

#endif // PARAMETERSDIALOG_H
