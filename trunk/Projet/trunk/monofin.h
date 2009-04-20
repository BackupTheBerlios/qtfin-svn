#ifndef MONOFIN_H
#define MONOFIN_H

#include <QtGui/QWidget>

class QAction;
class QActionGroup;
class QToolBar;
class QVBoxLayout;

namespace Data {
    class ProjectFile;
}
class PaintingScene;

class Monofin : public QWidget
{
    Q_OBJECT
public:
    Monofin(QWidget *parent = 0);

    QString currentFile() const { return _curFile; }
    bool isEmpty() const { return _isEmpty; }
    void newFile();
    bool okToContinue();
    bool open();
    bool openFile(const QString &fileName);
    bool save();
    bool saveAs();
    QSize sizeHint() const;
    QToolBar *toolBar() const { return _toolBar; }
    Qt::ToolBarArea toolBarArea() const { return _toolBarArea; }

public slots:
    void activateAddControl(bool a);
    void activateAddPoint(bool a);
    void activateCreateLine(bool a);
    void activateRemoveControlPoint(bool a);
    void alignTangents();
    void cleanPoints();
    void removeSelectedPoints();

signals:
    void currentFileChanged();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *closeEvent);

private slots:
    void monofinWasModified();

    void activeAddControl(bool a);
    void activeAddPoint(bool a);
    void activateRemoveControl(bool a);
    void beginLine(bool a);
    void clean();
    void finishedLine(bool a);
    void on_actionAddControl_toggled(bool a);
    void on_actionAddPoint_toggled(bool a);
    void on_actionCreatePolygon_toggled(bool a);
    void on_actionRemoveControl_toggled(bool a);
    void on_actionAlignTangents_triggered();
    void on_actionCleanPolygon_triggered();

    void setToolBarArea(Qt::ToolBarArea tba) { _toolBarArea = tba; }

private:
    void createToolBar();
    bool readFile(const QString &fileName);
    void retranslateUi();
    bool saveFile(const QString &fileName);
    void setConnections();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

private:
    QString _curFile;
    bool _isEmpty;
    bool _isUntitled;
    QVBoxLayout *_layout;
    Data::ProjectFile *_projectFile;
    PaintingScene *_scene;

    QAction *_actionAddControl;
    QAction *_actionAddPoint;
    QAction *_actionCreatePolygon;
    QAction *_actionCleanPolygon;
    QAction *_actionRemoveControl;
    QAction *_actionAlignTangents;
    QActionGroup *_actionGroupDraw;
    QToolBar *_toolBar;
    Qt::ToolBarArea _toolBarArea;
};

#endif // MONOFIN_H
