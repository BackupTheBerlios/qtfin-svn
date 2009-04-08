#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QTranslator>

class Monofin;
class PaintingScene;
class ParametersDialog;
class QtWindowListMenu;
class QAction;
class QActionGroup;
class QDockWidget;
class QMenu;
class QMenuBar;
class QStatusBar;
class QToolBar;
class QMdiArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

protected slots:
    void activeAddControl(bool a);
    void activeAddPoint(bool a);
    void activateRemoveControl(bool a);
    void beginLine(bool a);
    void clean();
    void configurate();
    void finishedLine(bool a);

protected:
    virtual void changeEvent(QEvent *e);

private slots:
    void about();
    void monofinModified();
    void newFile();
    void open();
    void openRecentFile();
    bool save();
    bool saveAs();
    void switchLanguage(QAction *action);

    void updateMenus();
    void updateRecentFileActions();
    void updateToolBars();

    void on_actionAddControl_toggled(bool a);
    void on_actionAddPoint_toggled(bool a);
    void on_actionCreatePolygon_toggled(bool a);
    void on_actionRemoveControl_toggled(bool a);
    void on_actionAlignTangents_triggered();
    void on_actionCleanPolygon_triggered();

private:
    Monofin *activeMonofin();
    void createActions();
    void createDocks();
    void createLanguageMenu();
    void createMenus();
    Monofin *createMonofin();
    void createStatusBar();
    void createToolBars();
    bool loadFile(const QString &fileName);
    void readSettings();
    void retranslateUi();
    void setConnections();
    QString strippedName(const QString &fullFileName);
    void writeSettings();

private:
    quint16 _majorVersion;
    quint16 _minorVersion;

    QAction *_actionExit;
    QAction *_actionNew;
    QAction *_actionOpen;
    QAction *_actionSave;
    QAction *_actionSaveAs;
    QAction *_actionAddControl;
    QAction *_actionAddPoint;
    QAction *_actionCreatePolygon;
    QAction *_actionCleanPolygon;
    QAction *_actionRemoveControl;
    QAction *_actionAlignTangents;
    QAction *_actionConfigurate;
    QAction *_actionLaunch;
    QAction *_actionShowGrid;
    QAction *_actionProperties;
    QAction *_actionAbout;
    QAction *_actionAboutQt;
    QMenuBar *_menuBar;
    QMenu *_menuFile;
    QMenu *_menuDraw;
    QMenu *_menuSimulation;
    QtWindowListMenu *_menuWin;
    QMenu *_menuLanguage;
    QMenu *_menuHelp;
    QToolBar *_mainToolBar;
    QStatusBar *_statusBar;
    QDockWidget *_dockFormLibrary;
    QWidget *_dockWidgetContents;
    QActionGroup *_actionGroupLanguage;
    QMdiArea *_mdiArea;

    QString _currentLanguage;
    QString _qmPath;
    QTranslator _appTranslator;
    QTranslator _qtTranslator;

    QStringList _recentFiles;
    enum { MaxRecentFiles = 5};
    QAction *_recentFileActions[MaxRecentFiles];
    QAction *_separatorAction;

    ParametersDialog *_paramDiag;
};

#endif // MAINWINDOW_H
