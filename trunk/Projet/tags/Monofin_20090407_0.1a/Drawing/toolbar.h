#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class PaintingScene;

class ToolBar: public QWidget{

    Q_OBJECT

public:
    ToolBar(PaintingScene* scene, QWidget* parent = 0);

public slots:
    void activeAddControl(bool a);
    void activeAddPoint(bool a);
    void activateRemoveControl(bool a);
    void beginLine(bool a);
    void clean();
    void finishedLine(bool a);
    void lineInterrupted();

protected:
    PaintingScene* _scene;
    QPushButton* _toolButtonAddControl;
    QPushButton* _toolButtonAddPoint;
    QPushButton* _toolButtonAlignTangents;
    QPushButton* _toolButtonClean;
    QPushButton* _toolButtonPoint;
    QPushButton* _toolButtonRemoveControl;
    QPushButton* _toolButtonSimplifyView;
    QVBoxLayout* _vlayout;

};

#endif // TOOLBAR_H
