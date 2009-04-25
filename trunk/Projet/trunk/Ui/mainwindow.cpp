#include "mainwindow.h"

#include "monofin.h"
#include "parametersdialog.h"
#include "startupdialog.h"
#include "lib/qtwindowlistmenu.h"

#include <QtCore/QDir>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QFileDialog>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QMessageBox>
#include <QtGui/QMdiArea>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>

// PUBLIC

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    if (objectName().isEmpty())
        setObjectName(QString::fromUtf8("MainWindow"));

    // set the current version
    _majorVersion = 0;
    _minorVersion = 1;

    // set the user interface
    _mdiArea = new QMdiArea(this);
    _mdiArea->setViewMode(QMdiArea::TabbedView);
    _mdiArea->setTabShape(QTabWidget::Triangular);
    setCentralWidget(_mdiArea);
    connect(_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(updateMenus()));
    connect(_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(updateToolBars()));
    connect(_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),
            this, SLOT(updateWindowTitle()));

    _paramDiag = new ParametersDialog(this);

    _qmPath = qApp->applicationDirPath() + "/translations";
    _appTranslator.load("monofin_" + QLocale::system().name(),
                        _qmPath);
    _qtTranslator.load("qt_" + QLocale::system().name(),
                       _qmPath);
    _currentLanguage = _appTranslator.translate("MainWindow", "English");
    qApp->installTranslator(&_qtTranslator);
    qApp->installTranslator(&_appTranslator);

    createActions();
    createMenus();
    createDocks();
    createStatusBar();
    createToolBars();
    setConnections();

    readSettings();

    retranslateUi();

    setWindowTitle(tr("Monofin"));
    setWindowIcon(QPixmap(":/images/icon.png"));
}

#include "layerparameters.h"
void MainWindow::configurate()
{
    if(_paramDiag->exec()) {
        QTabWidget *qtw = _paramDiag->layerTabWidget;
        for(int i = 0; i<qtw->count(); ++i) {
            LayerParameters * ll = static_cast<LayerParameters*> (qtw->widget(i));
            qDebug("%f", ll->youngDoubleSpinBox->value());
        }
    }
}

// PROTECTED
void MainWindow::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi();
        break;
    default:
        break;
    }
}

// PRIVATE SLOTS

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Monofin"),
                       tr("<h2>Monofin %1.%2</h2>"
                          "<p>Copyright &copy; 2009 Barbec_Guys</p>")
                       .arg(_majorVersion).arg(_minorVersion));
}

void MainWindow::newFile()
{
    QMdiSubWindow *msw = createMonofin();
    Monofin *monofin = static_cast<Monofin *>(msw->widget());
    monofin->newFile();
    monofin->show();
}

void MainWindow::open()
{
    QMdiSubWindow *msw = createMonofin();
    Monofin *monofin = static_cast<Monofin *>(msw->widget());
    if(monofin->open()) {
        updateToolBars();
        monofin->show();
    }
    else {
        msw->close();
    }
}

void MainWindow::openRecentFile()
{
    if(activeMonofin() && !activeMonofin()->okToContinue())
        return;
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        loadFile(action->data().toString());
}

bool MainWindow::save()
{
    if (activeMonofin())
        return activeMonofin()->save();
    else
        return false;
}

bool MainWindow::saveAs()
{
    if (activeMonofin())
        return activeMonofin()->saveAs();
    else
        return false;
}

void MainWindow::switchLanguage(QAction *action)
{
    QString locale = action->data().toString();
    _appTranslator.load("monofin_" + locale, _qmPath);
    _qtTranslator.load("qt_" + locale, _qmPath);
    _currentLanguage = _appTranslator.translate("MainWindow", "English");
    retranslateUi();
}

void MainWindow::updateMenus()
{
    bool hasMonofin = (activeMonofin() != 0);
    _actionSave->setEnabled(hasMonofin);
    _actionSaveAs->setEnabled(hasMonofin);
}

void MainWindow::updateRecentFileActions()
{
    if (activeMonofin()) {
        QString curFile = activeMonofin()->currentFile();
        _recentFiles.removeAll(curFile);
        _recentFiles.prepend(curFile);
    }

    QMutableStringListIterator i(_recentFiles);
    while(i.hasNext()) {
        QString recentFile = i.next();
        qDebug("recentFiles[] := %s", recentFile.toStdString().c_str());
        if (!QFile::exists(recentFile))
            i.remove();
    }

    for (int j=0; j<MaxRecentFiles; ++j) {
        if (j<_recentFiles.count()) {
            QString text = tr("&%1 %2")
                           .arg(j+1)
                           .arg(strippedName(_recentFiles[j]));
            _recentFileActions[j]->setText(text);
            _recentFileActions[j]->setData(_recentFiles[j]);
            _recentFileActions[j]->setVisible(true);
        } else
            _recentFileActions[j]->setVisible(false);
    }
    _separatorAction->setVisible(!_recentFiles.isEmpty());
}

void MainWindow::updateToolBars()
{
    if(_mainToolBar) {
        removeToolBar(_mainToolBar);
    }
    if (activeMonofin()) {
        _mainToolBar = activeMonofin()->toolBar();
        addToolBar(activeMonofin()->toolBarArea(), _mainToolBar);
        _mainToolBar->setVisible(true);
    }
}

void MainWindow::updateWindowTitle()
{
    if (activeMonofin()) {
        setWindowTitle(tr("%1[*] - %2").arg(strippedName(activeMonofin()->currentFile())).arg(tr("Monofin")));
    }
    else
        setWindowTitle(tr("Monofin"));
}


// PRIVATE
Monofin *MainWindow::activeMonofin()
{
    if (QMdiSubWindow *activeSubWindow = _mdiArea->activeSubWindow())
        return static_cast<Monofin *>(activeSubWindow->widget());
    else
        return 0;
}

void MainWindow::createActions()
{
    // MENU FILE
    _actionNew = new QAction(this);
    _actionNew->setObjectName(QString::fromUtf8("actionNew"));
    _actionOpen = new QAction(this);
    _actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
    _actionSave = new QAction(this);
    _actionSave->setObjectName(QString::fromUtf8("actionSave"));
    _actionSaveAs = new QAction(this);
    _actionSaveAs->setObjectName(QString::fromUtf8("actionSaveAs"));
    _actionExit = new QAction(this);
    _actionExit->setObjectName(QString::fromUtf8("actionExit"));

    // recent files
    for (int i=0; i<MaxRecentFiles; ++i) {
        _recentFileActions[i] = new QAction(this);
        _recentFileActions[i]->setVisible(false);
        connect(_recentFileActions[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }

    // MENU DRAW
    _actionShowGrid = new QAction(this);
    _actionShowGrid->setObjectName(QString::fromUtf8("actionShowGrid"));
    _actionShowGrid->setCheckable(true);
    _actionShowGrid->setChecked(true);
    _actionProperties = new QAction(this);
    _actionProperties->setObjectName(QString::fromUtf8("actionProperties"));

    // MENU SIMULATION
    _actionConfigurate = new QAction(this);
    _actionConfigurate->setObjectName(QString::fromUtf8("actionConfigurate"));
    _actionLaunch = new QAction(this);
    _actionLaunch->setObjectName(QString::fromUtf8("actionLaunch"));

    // MENU HELP
    _actionAbout = new QAction(this);
    _actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
    _actionAboutQt = new QAction(this);
    _actionAboutQt->setObjectName(QString::fromUtf8("actionAboutQt"));
}

void MainWindow::createDocks()
{
    _dockFormLibrary = new QDockWidget(this);
    _dockFormLibrary->setObjectName(QString::fromUtf8("dockFormLibrary"));
    _dockFormLibrary->setFloating(false);
    _dockFormLibrary->setFeatures(QDockWidget::AllDockWidgetFeatures);
    _dockFormLibrary->setAllowedAreas(Qt::AllDockWidgetAreas);

    _dockWidgetContents = new QWidget();
    _dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
    _dockFormLibrary->setWidget(_dockWidgetContents);
    addDockWidget(Qt::RightDockWidgetArea, _dockFormLibrary);
}

void MainWindow::createLanguageMenu()
{
    _menuLanguage = new QMenu(_menuBar);

    _actionGroupLanguage = new QActionGroup(this);
    connect(_actionGroupLanguage, SIGNAL(triggered(QAction*)),
            this, SLOT(switchLanguage(QAction*)));

    QDir dir(_qmPath);
    QStringList fileNames =
            dir.entryList(QStringList("monofin_*.qm"));

    for (int i=0; i<fileNames.size(); ++i) {
        QString locale = fileNames[i];
        locale.remove(0, locale.indexOf('_')+1);
        locale.truncate(locale.indexOf('.'));

        QTranslator translator;
        translator.load(fileNames[i], _qmPath);
        QString language = translator.translate("MainWindow", "English");
        QAction *action = new QAction(tr("&%1 %2")
                                      .arg(i+1).arg(language), this);
        action->setCheckable(true);
        action->setData(locale);

        _menuLanguage->addAction(action);
        _actionGroupLanguage->addAction(action);

        if (language == _currentLanguage)
            action->setChecked(true);
    }
}

void MainWindow::createMenus()
{
    _menuBar = new QMenuBar(this);
    _menuBar->setObjectName(QString::fromUtf8("menuBar"));
    setMenuBar(_menuBar);

    // MENU FILE
    _menuFile = new QMenu(_menuBar);
    _menuFile->setObjectName(QString::fromUtf8("menuFile"));
    _menuFile->addAction(_actionNew);
    _menuFile->addAction(_actionOpen);
    _menuFile->addSeparator();
    _menuFile->addAction(_actionSave);
    _menuFile->addAction(_actionSaveAs);
    _menuFile->addSeparator();
    _menuFile->addAction(_actionExit);

    // recent files
    _separatorAction = _menuFile->insertSeparator(_actionExit);
    for (int i=0; i<MaxRecentFiles; ++i)
        _menuFile->insertAction(_actionExit, _recentFileActions[i]);
    _menuFile->insertSeparator(_actionExit);

    // MENU DRAW
    _menuDraw = new QMenu(_menuBar);
    _menuDraw->setObjectName(QString::fromUtf8("menuDraw"));
    _menuDraw->addAction(_actionShowGrid);
    _menuDraw->addAction(_actionProperties);

    // MENU SIMULATION
    _menuSimulation = new QMenu(_menuBar);
    _menuSimulation->setObjectName(QString::fromUtf8("menuSimulation"));
    _menuSimulation->addAction(_actionConfigurate);
    _menuSimulation->addAction(_actionLaunch);

    // MENU WINDOW
    _menuWin = new QtWindowListMenu(_menuBar);
    _menuWin->attachToMdiArea(_mdiArea);

    // MENU LANGUAGE
    createLanguageMenu();

    // MENU HELP
    _menuHelp = new QMenu(_menuBar);
    _menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
    _menuHelp->addAction(_actionAbout);
    _menuHelp->addAction(_actionAboutQt);


    // MENUBAR
    _menuBar->addAction(_menuFile->menuAction());
    _menuBar->addAction(_menuDraw->menuAction());
    _menuBar->addAction(_menuSimulation->menuAction());
    _menuBar->addAction(_menuWin->menuAction());
    _menuBar->addAction(_menuLanguage->menuAction());
    _menuBar->addAction(_menuHelp->menuAction());
}

QMdiSubWindow *MainWindow::createMonofin()
{
    Monofin *monofin = new Monofin;
    connect(monofin, SIGNAL(currentFileChanged()), this, SLOT(updateRecentFileActions()));

    QMdiSubWindow *msw = _mdiArea->addSubWindow(monofin);
    msw->setAttribute(Qt::WA_DeleteOnClose);
    return msw;
}

void MainWindow::createStatusBar()
{
    _statusBar = new QStatusBar(this);
    _statusBar->setObjectName(QString::fromUtf8("statusBar"));
    setStatusBar(_statusBar);
}

void MainWindow::createToolBars()
{
    _mainToolBar = NULL;
}


bool MainWindow::loadFile(const QString &fileName)
{
    QMdiSubWindow *msw = createMonofin();
    Monofin *monofin = static_cast<Monofin *>(msw->widget());
    if(monofin->openFile(fileName)) {
        monofin->show();
        updateToolBars();
        return true;
    } else {
        msw->close();
        return false;
    }
}

void MainWindow::readSettings()
{
}

void MainWindow::retranslateUi()
{
    // actions
    _actionExit->setText(QApplication::translate("MainWindow", "E&xit", 0, QApplication::UnicodeUTF8));
    _actionExit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
    _actionNew->setText(QApplication::translate("MainWindow", "&New...", 0, QApplication::UnicodeUTF8));
    _actionNew->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
    _actionOpen->setText(QApplication::translate("MainWindow", "&Open...", 0, QApplication::UnicodeUTF8));
    _actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
    _actionSave->setText(QApplication::translate("MainWindow", "&Save", 0, QApplication::UnicodeUTF8));
    _actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
    _actionSaveAs->setText(QApplication::translate("MainWindow", "&Save As...", 0, QApplication::UnicodeUTF8));
    _actionShowGrid->setText(QApplication::translate("Monofin", "&Show Grid", 0, QApplication::UnicodeUTF8));
    _actionShowGrid->setShortcut(QApplication::translate("Monofin", "Ctrl+G", 0, QApplication::UnicodeUTF8));
    _actionConfigurate->setText(QApplication::translate("Monofin", "&Configurate...", 0, QApplication::UnicodeUTF8));
    _actionConfigurate->setShortcut(QApplication::translate("Monofin", "Ctrl+S", 0, QApplication::UnicodeUTF8));
    _actionLaunch->setText(QApplication::translate("Monofin", "&Launch", 0, QApplication::UnicodeUTF8));
    _actionLaunch->setShortcut(QApplication::translate("Monofin", "Ctrl+L", 0, QApplication::UnicodeUTF8));
    _actionProperties->setText(QApplication::translate("MainWindow", "&Properties...", 0, QApplication::UnicodeUTF8));
    _actionAbout->setText(QApplication::translate("MainWindow", "&About", 0, QApplication::UnicodeUTF8));
    _actionAboutQt->setText(QApplication::translate("MainWindow", "About &Qt", 0, QApplication::UnicodeUTF8));

    // menus
    _menuFile->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
    _menuDraw->setTitle(QApplication::translate("MainWindow", "&Draw", 0, QApplication::UnicodeUTF8));
    _menuSimulation->setTitle(QApplication::translate("MainWindow", "&Simulation", 0, QApplication::UnicodeUTF8));
    _menuLanguage->setTitle(QApplication::translate("MainWindow", "&Language", 0, QApplication::UnicodeUTF8));
    _menuHelp->setTitle(QApplication::translate("MainWindow", "&Help", 0, QApplication::UnicodeUTF8));

    // docks
    _dockFormLibrary->setWindowTitle(QApplication::translate("MainWindow", "Form library", 0, QApplication::UnicodeUTF8));
}

void MainWindow::setConnections()
{
    // MENU
    connect(_actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(_actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    connect(_actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(_actionSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(_actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(_actionConfigurate, SIGNAL(triggered()), this, SLOT(configurate()));
    connect(_actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(_actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    QMetaObject::connectSlotsByName(this);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::writeSettings()
{
}
