#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QPointer>
#include <QtCore/QTranslator>
#include <QtGui/QMainWindow>
#include <QtGui/QGridLayout>
#include <QtGui/QPushButton>
#include <QtGui/QInputDialog>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>

#include "Data/projectfile.h"
#include "Ui/graphic.h"
#include "formitem.h"
class Monofin;
class PaintingScene;
class StartupDialog;
class QtWindowListMenu;
class QAction;
class QActionGroup;
class QDockWidget;
class QMdiArea;
class QMdiSubWindow;
class QMenu;
class QMenuBar;
class QStatusBar;
class QToolBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

signals:
    void updateMonofinScene();

protected slots:
    void configurate();

protected:
    virtual void changeEvent(QEvent *e);
    virtual void closeEvent(QCloseEvent *event);

private slots:
    void about();
    void addFormToLibrary();
    void launch();
    void loadForm(QListWidgetItem *item);    void preview3D();
    void newEmptyProject();
    void newFile();
    void newProjectFromImage();
    void open();
    void openRecentFile();
    void removeForm();    bool save();
    bool saveAs();
    void showGrid(bool a);
    void switchLanguage(QAction *action);

    void updateMenus();
    void updateRecentFileActions();
    void updateToolBars();
    void updateWindowTitle();

private:
    Monofin *activeMonofin();
    void createActions();
    void createDocks();
    void createLanguageMenu();
    void createMenus();
    QMdiSubWindow *createMonofin(Data::ProjectFile *projectFile = new Data::ProjectFile());
    void createStatusBar();
    void createToolBars();
    bool loadFile(const QString &fileName);
    void readSettings();
    void retranslateUi();
    void setConnections();
    QString strippedName(const QString &fullFileName);
    void updateLibrary();
    void writeSettings();

private:
    quint16 _majorVersion;
    quint16 _minorVersion;

    QAction *_actionExit;
    QAction *_actionNew;
    QAction *_actionOpen;
    QAction *_actionSave;
    QAction *_actionSaveAs;
    QAction *_actionConfigurate;
    QAction *_actionLaunch;
    QAction *_action3DPreview;
    QAction *_actionShowGrid;
    QAction *_actionProperties;
    QAction *_actionAbout;
    QAction *_actionAboutQt;
    QMenuBar *_menuBar;
    QMenu *_menuFile;
    QMenu *_menuDraw;
    QMenu *_menuDrawExt;
    QMenu *_menuSimulation;
    QtWindowListMenu *_menuWin;
    QMenu *_menuView;
    QMenu *_menuLanguage;
    QMenu *_menuHelp;

    QToolBar *_mainToolBar;
    QToolBar *_viewToolBar;
    QStatusBar *_statusBar;

    QDockWidget *_dockFormLibrary;
    QWidget *_dockWidgetContents;
    QPushButton *_buttonAddToFormLibrary;
    QPushButton *_buttonRemoveForm;    QGridLayout *_layoutLibrary;
    QListWidget *_listWidgetForms;
    QString _libraryPath;

    QMdiArea *_mdiArea;

    QString _currentLanguage;
    QString _qmPath;
    QTranslator _appTranslator;
    QActionGroup *_actionGroupLanguage;

    QStringList _recentFiles;
    enum { MaxRecentFiles = 5};
    QAction *_recentFileActions[MaxRecentFiles];
    QAction *_separatorAction;

    StartupDialog *_startupDialog;
    QPointer<Graphic> _graphicView;

};

#endif // MAINWINDOW_H
