#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QTime>

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
    void changeColor();
    void clean();
    void finishedLine(bool a);
    void lineInterrupted();
    void pointsOnScene(bool a);

signals:
    void changeColor(int, int, QColor);

protected:
    QCheckBox* _keepBezierCheckBox;
    QCheckBox* _magnetCheckBox;
    PaintingScene* _scene;
    QPushButton* _redoButton;
    QPushButton* _toolButtonAddControl;
    QPushButton* _toolButtonAddPoint;
    QPushButton* _toolButtonAlignTangents;
    QPushButton* _toolButtonClean;
    QPushButton* _toolButtonPoint;
    QPushButton* _toolButtonRemoveControl;
    QPushButton* _toolButtonSimplifyView;
    QPushButton* _undoButton;
    QVBoxLayout* _vlayout;

};

#endif // TOOLBAR_H
