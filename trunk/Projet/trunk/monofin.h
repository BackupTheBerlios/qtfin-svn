#ifndef MONOFIN_H
#define MONOFIN_H

#include <QtCore/QPointer>
#include <QtGui/QWidget>
#include <QtGui/QMenu>

#include "Drawing/layerview.h"
#include "Drawing/paintingscene.h"
#include "EdgeDetection/graphic.h"

class QAction;
class QActionGroup;
class QToolBar;
class QVBoxLayout;

class Geometry3DViewer;
class GenerateComsolFile;
class LayerView;
class PaintingScene;
class ParametersDialog;

class Monofin : public QWidget
{
    Q_OBJECT
public:
    Monofin(Data::ProjectFile *projectFile = new Data::ProjectFile(), QWidget *parent = 0);
    virtual ~Monofin();

    QString currentFile() const { return _curFile; }
    int getNbLayers();
    bool isEmpty() const { return _isEmpty; }
    void newFile();
    bool newFileFromImage();
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
    void activateMagnet(bool a);
    void activateModifyBackgroundPicture(bool a);
    void activateRemoveControlPoint(bool a);
    void addBackgroundPicture();
    void alignTangents();
    void cleanPoints();
    void configurate();
    void decreaseWindowSize();
    void increaseWindowSize();
    void keepBezierCurve(bool a);
    void launch();
    void preview3D();
    void redo();
    void removeBackgroundPicture();
    void removeSelectedPoints();
    void simplifyView(bool a);
    void switchToBlack();
    void switchToRed();
    void switchToWhite();
    void undo();
    void zoomIn();
    void zoomInOnBackgroundPicture();
    void zoomOut();
    void zoomOutOnBackgroundPicture();

signals:
    void currentFileChanged();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *closeEvent);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

private slots:
    void monofinWasModified();

    void activeAddControl(bool a);
    void activeAddPoint(bool a);
    void activeModifyBackgroundPicture(bool a);
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
    void on_actionInsertLayer_triggered();
    void on_actionRemoveLayer_triggered();

    void setToolBarArea(Qt::ToolBarArea tba) { _toolBarArea = tba; }

private:
    void createToolBar();
    void initialize();
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
    QPointer<Graphic> _graphicView;
    QPointer<LayerView> _layerView;
    ParametersDialog *_paramDiag;
    Data::ProjectFile *_projectFile;
    QPointer<PaintingScene> _scene;

    Geometry3DViewer *_viewer;
    GenerateComsolFile *_generator;

    QAction *_actionUndo;
    QAction *_actionRedo;
    QAction *_actionAddControl;
    QAction *_actionAddPoint;
    QAction *_actionRemovePoints;
    QAction *_actionCreatePolygon;
    QAction *_actionCleanPolygon;
    QAction *_actionRemoveControl;
    QAction *_actionAlignTangents;
    QAction *_actionMagnet;
    QAction *_actionSwitchColors;
    QMenu *_menuColors;
    QAction *_actionSwitchToBlack;
    QAction *_actionSwitchToWhite;
    QAction *_actionSwitchToRed;
    QAction *_actionSimplifyView;
    QAction *_actionIncreaseWindowSize;
    QAction *_actionDecreaseWindowSize;
    QAction *_actionInsertLayer;
    QAction *_actionRemoveLayer;
    QAction *_actionAddBackgroundPicture;
    QAction *_actionTransformBackgroundPicture;
    QAction *_actionRemoveBackgroundPicture;
    QAction *_enlargeBackgroundPictureSize;
    QAction *_reduceBackgroundPictureSize;
    QActionGroup *_actionGroupDraw;
    QToolBar *_toolBar;
    Qt::ToolBarArea _toolBarArea;
};

#endif // MONOFIN_H
