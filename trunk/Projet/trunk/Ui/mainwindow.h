#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

namespace Ui
{
    class MainWindowClass;
}

class PaintingScene;
class ParametersDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void activeAddControl(bool a);
    void activeAddPoint(bool a);
    void activateRemoveControl(bool a);
    void beginLine(bool a);
    void clean();
    void finishedLine(bool a);

    void configurate();

private:
    void setConnections();

private:
    Ui::MainWindowClass *ui;
    PaintingScene *_scene;
    ParametersDialog *_paramDiag;
};

#endif // MAINWINDOW_H
