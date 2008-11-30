#ifndef STEPS_H
#define STEPS_H

#include <QtGui/QMainWindow>
#include "ui_steps.h"

class Steps : public QMainWindow
{
    Q_OBJECT

public:
    Steps(QWidget *parent = 0);
    ~Steps();

    public slots:
    void openFile(void);
    void showme(void);

private:
    Ui::stepsClass ui;
};

#endif // STEPS_H
