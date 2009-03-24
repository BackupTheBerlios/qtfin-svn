#ifndef MONOFIN_H
#define MONOFIN_H

#include <QObject>

class Monofin : public QObject
{
    Q_OBJECT
public:
    Monofin();

public slots:
    void newEmptyProject();
    void newImageProject();
    void openProject();

};

#endif // MONOFIN_H
