#include "mainwindow.h"

#include "monofin.h"
#include "startupdialog.h"
#include "lib/qtwindowlistmenu.h"
#include "Drawing/paintingscene.h"
#include "Ui/graphic.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QRect>
#include <QtCore/QSettings>
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

    _startupDialog = new StartupDialog(this);

#ifdef Q_OS_MAC
    _qmPath = qApp->applicationDirPath() + ":/translations";
    _libraryPath = qApp->applicationDirPath() + ":/library";
#else
    _qmPath = qApp->applicationDirPath() + "/translations";
    _libraryPath = qApp->applicationDirPath() + "/library";
#endif // Q_OS_MAC

    _appTranslator.load("monofin_" + QLocale::system().name(),
                        _qmPath);
    _currentLanguage = _appTranslator.translate("MainWindow", "English");
    qApp->installTranslator(&_appTranslator);

    createActions();
    createMenus();
    createDocks();
    updateLibrary();
    createStatusBar();
    createToolBars();
    setConnections();

    readSettings();

    retranslateUi();

    setWindowTitle(tr("Monofin"));
    setWindowIcon(QIcon(":/icons/general/logo.png"));
}

#include "layerparameters.h"
void MainWindow::configurate()
{
    if (activeMonofin())
        activeMonofin()->configurate();
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

void MainWindow::closeEvent(QCloseEvent *event)
{
    _mdiArea->closeAllSubWindows();
    if (activeMonofin())
        event->ignore();
    else {
        event->accept();
        writeSettings();
    }
}

// PRIVATE SLOTS

void MainWindow::about()
{
    QMessageBox::about(this, tr("About %1").arg(QApplication::applicationName()),
                       tr("<h2>Monofin %1</h2>"
                          "<p>Copyright &copy; 2009 %2</p>"
                          "<p>"
                          "Chaudet Yoann <br/>"
                          "Garcia Paul <br/>"
                          "Gautier Quentin <br/>"
                          "Le Squer Nicolas <br/>"
                          "Musset Nicolas <br/>"
                          "Villoing Xavier <br/>"
                          "</p>")
                       .arg(QApplication::applicationVersion()).arg(QApplication::organizationName()));
}

void MainWindow::addFormToLibrary()
{
    if(this->activeMonofin() != NULL){
        if(!this->activeMonofin()->isEmpty()){
            QString name = QInputDialog::getText(this, tr("Choose form name"), tr("Enter a name for the form:"));
            QString path(_libraryPath);
            path.append("/");
            path.append(name);
            path.append(".finf");
            if(!name.isEmpty()){
                QDir dir(_libraryPath);
                if(!dir.exists()){
                    qDebug("Directory not existing");
                    dir.mkpath(_libraryPath);
                }
                if(QFile::exists(path)){
                    QMessageBox msg;
                    msg.setText(tr("File already existing"));
                    msg.setIcon(QMessageBox::Question);                    msg.setInformativeText(("Overwrite the file ?"));                    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);                    msg.setDefaultButton(QMessageBox::No);                    int ret = msg.exec();                    if(ret == QMessageBox::Yes){                        this->activeMonofin()->saveForm(path);                    }                }else{                    this->activeMonofin()->saveForm(path);                }            }
        }
    }
    this->updateLibrary();}

void MainWindow::launch()
{
    if (activeMonofin()) {
        activeMonofin()->launch();
    }
}

void MainWindow::loadForm(QListWidgetItem *item){
    if(this->activeMonofin() == NULL){
        this->newEmptyProject();
    }
    if(this->activeMonofin() != NULL){
        if(item->type() == FormItem::Type){
            this->activeMonofin()->loadForm(((FormItem*)item)->path());
        }
    }
}

void MainWindow::preview3D()
{
    if (activeMonofin()) {
        activeMonofin()->preview3D();
    }
}

void MainWindow::newEmptyProject()
{
    QMdiSubWindow *msw = createMonofin();
    Monofin *monofin = static_cast<Monofin *>(msw->widget());
    monofin->newFile();
    monofin->show();
}

void MainWindow::newFile()
{
    int res = 0;
    if (res = _startupDialog->exec()) {
        switch (res) {
        case StartupDialog::empty:
            newEmptyProject();
            break;
        case StartupDialog::image:
            newProjectFromImage();
            break;
        case StartupDialog::open:
            open();
            break;
        default:

            break;
        }    
    }
}

void MainWindow::newProjectFromImage()
{
    qDebug("MainWindow::newProjectFromImage()");
    Data::ProjectFile *projectFile = new Data::ProjectFile;
    if (_graphicView.isNull()) {
        qDebug("graphicview not existing, creating a new one.");
        _graphicView = new Graphic(this, projectFile, 1024, 768);
    } else {
        qDebug("existing graphicview.");
        _graphicView->setProjectFile(projectFile);
    }
    //projectFile->startHistory(Data::MonofinSurface);
    //projectFile->clearSurface();
    //projectFile->stopHistory(Data::MonofinSurface);

    QMdiSubWindow *msw = createMonofin(projectFile);
    Monofin *monofin = static_cast<Monofin *>(msw->widget());
    monofin->show();
    monofin->newFileFromImage();
    connect(_graphicView, SIGNAL(kept()), this, SIGNAL(updateMonofinScene()));

    _graphicView->show();
}

void MainWindow::open()
{
    qDebug("MainWindow::open()");
    QMdiSubWindow *msw = createMonofin();
    Monofin *monofin = static_cast<Monofin *>(msw->widget());
    monofin->show();
    if(monofin->open()) {
        updateToolBars();
    }
    else {
        msw->close();
    }
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        loadFile(action->data().toString());
}

void MainWindow::removeForm(){    QListWidgetItem* item = _listWidgetForms->currentItem();    if(item != NULL){        if(item->type() == FormItem::Type){            QString path(((FormItem*)item)->path());            QFile file(path);            if(file.exists()){                QMessageBox msg;                msg.setText(tr("Remove file"));                msg.setIcon(QMessageBox::Question);                msg.setInformativeText(("Are you sure ?"));                msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);                msg.setDefaultButton(QMessageBox::No);                int ret = msg.exec();                if(ret == QMessageBox::Yes){                    file.remove();                    this->updateLibrary();                }            }        }    }}bool MainWindow::save()
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

void MainWindow::setFullScreenMode(bool enabled)
{
    if (enabled)
        setWindowState(windowState() ^ Qt::WindowFullScreen);
    else
        setWindowState(windowState() & ~Qt::WindowFullScreen);
}

void MainWindow::showGrid(bool a)
{
    if(activeMonofin() != 0){
        this->activeMonofin()->showGrid(a);
    }
}

void MainWindow::switchLanguage(QAction *action)
{
    QString locale = action->data().toString();
    _appTranslator.load("monofin_" + locale, _qmPath);
    _currentLanguage = _appTranslator.translate("MainWindow", "English");
    retranslateUi();
}


void MainWindow::updateMenus()
{
    bool hasMonofin = (activeMonofin() != 0);
    _actionSave->setEnabled(hasMonofin);
    _actionSaveAs->setEnabled(hasMonofin);

    // MENU DRAW && VIEW
    if (hasMonofin) {
        _menuDraw->clear();
        _menuView->clear();
        QList<QToolBar*> l = activeMonofin()->toolBar();
        if(l.size() >= 1){
            _menuDraw->addActions(l.first()->actions());
            if(l.size() >= 2){
                _menuView->addActions(l.at(1)->actions());
            }
        }
        _menuDraw->addAction(_actionShowGrid);
        _menuDraw->addSeparator();
        _menuDraw->addAction(_action3DPreview);

        _menuDraw->setEnabled(true);
        _menuView->setEnabled(true);
    }
    else{
        _menuDraw->setEnabled(false);
        _menuView->setEnabled(false);
    }
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

    if(_viewToolBar) {
        removeToolBar(_viewToolBar);
    }

    if (activeMonofin()) {
        QList<QToolBar*> l = activeMonofin()->toolBar();
        if(l.size() >= 1){
            _mainToolBar = l.first();
            addToolBar(activeMonofin()->toolBarDrawArea(), _mainToolBar);
            _mainToolBar->setVisible(true);
            if(l.size() >= 2){
                _viewToolBar = l.at(1);
                addToolBar(activeMonofin()->toolBarViewArea(), _viewToolBar);
                _viewToolBar->setVisible(true);
            }else{
                qDebug("No view toolbar to show");
            }
        }else{
            qDebug("No toolBar to show");
        }

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
    _action3DPreview = new QAction(this);
    _action3DPreview->setObjectName(QString::fromUtf8("action3DPreview"));

    // MENU SIMULATION
    _actionConfigurate = new QAction(this);
    _actionConfigurate->setObjectName(QString::fromUtf8("actionConfigurate"));
    _actionLaunch = new QAction(this);
    _actionLaunch->setObjectName(QString::fromUtf8("actionLaunch"));

    // MENU WINDOW
    _actionFullScreen = new QAction(this);
    _actionFullScreen->setObjectName(QString::fromUtf8("actionFullScreen"));
    _actionFullScreen->setCheckable(true);
    _actionFullScreen->setChecked(false);

    // MENU HELP
    _actionAbout = new QAction(this);
    _actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
    _actionAboutQt = new QAction(this);
    _actionAboutQt->setObjectName(QString::fromUtf8("actionAboutQt"));

    //FORM LIBRARY
    _buttonAddToFormLibrary = new QPushButton(this);
    _buttonAddToFormLibrary->setObjectName(QString::fromUtf8("actionAddToFormLibrary"));
    _buttonRemoveForm = new QPushButton(this);    _buttonRemoveForm->setObjectName(QString::fromUtf8("buttonRemoveForm"));}

void MainWindow::createDocks()
{
    _dockFormLibrary = new QDockWidget(this);
    _dockFormLibrary->setObjectName(QString::fromUtf8("dockFormLibrary"));
    _dockFormLibrary->setFloating(false);
    _dockFormLibrary->setFeatures(QDockWidget::AllDockWidgetFeatures);
    _dockFormLibrary->setAllowedAreas(Qt::AllDockWidgetAreas);

    _listWidgetForms = new QListWidget();

    _layoutLibrary = new QGridLayout();
    _layoutLibrary->setObjectName(QString::fromUtf8("layoutLibrary"));
    _layoutLibrary->addWidget(_buttonAddToFormLibrary, 1,0);
    _layoutLibrary->addWidget(_buttonRemoveForm, 1, 1);
    _layoutLibrary->addWidget(_listWidgetForms, 0, 0, 1, 0);

    _dockWidgetContents = new QWidget();
    _dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
    _dockWidgetContents->setLayout(_layoutLibrary);

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
    _menuDraw->setDisabled(true);

    // MENU SIMULATION
    _menuSimulation = new QMenu(_menuBar);
    _menuSimulation->setObjectName(QString::fromUtf8("menuSimulation"));
    _menuSimulation->addAction(_actionConfigurate);
    _menuSimulation->addAction(_actionLaunch);

    // MENU WINDOW
    _menuWin = new QtWindowListMenu(_menuBar);
    _menuWin->attachToMdiArea(_mdiArea);
    _menuWin->addAction(_actionFullScreen);
    _menuWin->addSeparator();

    //MENU VIEW
    _menuView = new QMenu(_menuBar);
    _menuView->setObjectName(QString::fromUtf8("menuView"));
    _menuView->setDisabled(true);

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
    _menuBar->addAction(_menuView->menuAction());
    _menuBar->addAction(_menuSimulation->menuAction());
    _menuBar->addAction(_menuWin->menuAction());
    _menuBar->addAction(_menuLanguage->menuAction());
    _menuBar->addAction(_menuHelp->menuAction());
}

QMdiSubWindow *MainWindow::createMonofin(Data::ProjectFile *projectFile)
{
    qDebug("MainWindow::createMonofin()");
    Monofin *monofin = new Monofin(projectFile);
    connect(monofin, SIGNAL(currentFileChanged()), this, SLOT(updateRecentFileActions()));
    connect(this, SIGNAL(updateMonofinScene()), monofin, SLOT(updateScene()));

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
    _viewToolBar = NULL;
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
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());

    QRect rect = settings.value("geometry", QRect(100, 100, 900, 700)).toRect();
    move(rect.topLeft());
    resize(rect.size());

    _recentFiles = settings.value("recentFiles").toStringList();
    updateRecentFileActions();

    setFullScreenMode(settings.value("fullScreen").toBool());

    settings.setValue("geometry", geometry());
    settings.setValue("recentFiles", _recentFiles);
    settings.setValue("fullScreen", _actionFullScreen->isChecked());
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
    _actionShowGrid->setShortcut(QApplication::translate("Monofin", "Ctrl+Shift+G", 0, QApplication::UnicodeUTF8));
    _actionConfigurate->setText(QApplication::translate("Monofin", "&Configurate...", 0, QApplication::UnicodeUTF8));
    _action3DPreview->setText(QApplication::translate("MainWindow", "&3D Preview", 0, QApplication::UnicodeUTF8));
    _actionLaunch->setText(QApplication::translate("Monofin", "&Launch", 0, QApplication::UnicodeUTF8));
    _actionLaunch->setShortcut(QApplication::translate("Monofin", "Ctrl+L", 0, QApplication::UnicodeUTF8));
    _actionFullScreen->setText(QApplication::translate("MainWindow", "&Fullscreen", 0, QApplication::UnicodeUTF8));
    _actionFullScreen->setShortcut(QApplication::translate("MainWindow", "F12", 0, QApplication::UnicodeUTF8));
    _actionProperties->setText(QApplication::translate("MainWindow", "&Properties...", 0, QApplication::UnicodeUTF8));
    _actionAbout->setText(QApplication::translate("MainWindow", "&About", 0, QApplication::UnicodeUTF8));
    _actionAboutQt->setText(QApplication::translate("MainWindow", "About &Qt", 0, QApplication::UnicodeUTF8));
    _buttonAddToFormLibrary->setText(QApplication::translate("MainWindow", "&Add form", 0, QApplication::UnicodeUTF8));
    _buttonRemoveForm->setText(QApplication::translate("MainWindow", "&Remove form", 0, QApplication::UnicodeUTF8));
    // menus
    _menuFile->setTitle(QApplication::translate("MainWindow", "&File", 0, QApplication::UnicodeUTF8));
    _menuDraw->setTitle(QApplication::translate("MainWindow", "&Draw", 0, QApplication::UnicodeUTF8));
    _menuSimulation->setTitle(QApplication::translate("MainWindow", "&Simulation", 0, QApplication::UnicodeUTF8));
    _menuView->setTitle(QApplication::translate("MainWindow", "&View", 0, QApplication::UnicodeUTF8));
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
    connect(_action3DPreview, SIGNAL(triggered()), this, SLOT(preview3D()));
    connect(_actionLaunch, SIGNAL(triggered()), this, SLOT(launch()));
    connect(_actionFullScreen, SIGNAL(toggled(bool)), this, SLOT(setFullScreenMode(bool)));
    connect(_actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(_actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(_actionShowGrid, SIGNAL(toggled(bool)), this, SLOT(showGrid(bool)));
    connect(_buttonAddToFormLibrary, SIGNAL(clicked()), this, SLOT(addFormToLibrary()));
    connect(_listWidgetForms, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(loadForm(QListWidgetItem*)));    connect(_buttonRemoveForm, SIGNAL(clicked()), this, SLOT(removeForm()));
    QMetaObject::connectSlotsByName(this);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::updateLibrary()
{
    _listWidgetForms->clear();
    /*foreach(QListWidgetItem* i, _listItems){
        _listItems.removeOne(i);
        delete i;
    }
    _listItems.clear();*/

    QDir dir(_libraryPath);
    QStringList fileNames =
            dir.entryList(QStringList("*.finf"));

    for (int i=0; i<fileNames.size(); ++i) {

        QString locale;
        locale.append(_libraryPath);
        locale.append("/");
        locale.append(fileNames[i]);
        QListWidgetItem* item = new FormItem(
                QIcon(QPixmap::fromImage(Data::ProjectFile::getImage(locale))),
                fileNames[i].left(fileNames[i].size()-5),
                locale);

        if(Data::ProjectFile::getImage(locale).isNull())
            qDebug("image null");
        qDebug() << locale;

        _listWidgetForms->setIconSize(QSize(64,64));
        _listWidgetForms->addItem(item);

    }

}

void MainWindow::writeSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());

    setWindowState(windowState() & ~Qt::WindowFullScreen);
    settings.setValue("geometry", geometry());
    settings.setValue("recentFiles", _recentFiles);
    settings.setValue("fullScreen", _actionFullScreen->isChecked());
}
