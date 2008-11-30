#ifndef PHYSICALPARAMETERS_H
#define PHYSICALPARAMETERS_H

#include <QtGui/QWidget>
#include "ui_physicalParameters.h"

class PhysicalParameters : public QWidget
{
    Q_OBJECT

public:
    PhysicalParameters(QWidget *parent = 0);
    ~PhysicalParameters();

private:
    Ui::physicalParametersClass ui;
};

#endif // PHYSICALPARAMETERS_H
