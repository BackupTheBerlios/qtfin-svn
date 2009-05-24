#include "monofin.h"
#include "Data/projectfile.h"
#include "Drawing/paintingview.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>

/*!
    \class Monofin

    \brief The Monofin class...

    \sa LayerView, PaintingScene, PaintingView, ProjectFile
*/

/*!
    Constructs a monofin which is a child of \a parent.
*/
Monofin::Monofin(Data::ProjectFile *projectFile, QWidget *parent)
    : QWidget(parent), _projectFile(projectFile)
{
    setAttribute(Qt::WA_DeleteOnClose);
    _curFile = QString();
    _isEmpty = true;
    _isUntitled = true;
    _layout = new QVBoxLayout(this);
}
/*!
    Destroys the monofin.

    All this widget's children are deleted first. The application
    exits if this widget is the main widget.
*/
Monofin::~Monofin()
{
    qDebug("Monofin::~Monofin()");
}

// PUBLIC
/*!

*/
static int documentNumber = 1;

/*!
    Create a new file.
*/
void Monofin::newFile()
{
    setCurrentFile(QString());
    ++documentNumber;
    initialize();
}

/*!
    Indicate whether the monofin has been modified. In the latter case, ask the user to save the changes made.
*/
bool Monofin::okToContinue()
{
    if (isWindowModified()) {
        int r = QMessageBox::warning(this, tr("Monofin"),
                                     tr("The document has been modified.\n"
                                        "Do you want to save your changes?"),
                                     QMessageBox::Yes | QMessageBox::Default,
                                     QMessageBox::No,
                                     QMessageBox::Cancel | QMessageBox::Escape);
        if (r == QMessageBox::Yes)
            return save();
        else if (r == QMessageBox::Cancel)
            return false;
    }
    return true;
}

/*!
    Ask the user to open a file, using a QFileDialog.
    \sa QFileDialog
*/
bool Monofin::open()
{
    qDebug("Monofin::open()");
    if (okToContinue()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open Monofin"), ".",
                                                        tr("Monofin files (*.fin)"));
        if (!fileName.isEmpty())
            return openFile(fileName);
    }
    return false;
}

/*!
    Check whether the given \a fileName exists before opening it.
*/
bool Monofin::openFile(const QString &fileName)
{
    qDebug() << QString("Monofin::openFile(%1)").arg(fileName);
    if (QFile::exists(fileName))
        return readFile(fileName);
    else
        return false;
}

/*!
    Save the file.
*/
bool Monofin::save()
{
    qDebug("Monofin::save()");
    if (_isUntitled)
        return saveAs();
    else
        return saveFile(_curFile);
}

/*!
    Save the file, asking the user.
*/
bool Monofin::saveAs()
{
    qDebug("Monofin::saveAs()");
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Monofin"), ".",
                                                    tr("Monofin files (*.fin)"));
    if(fileName.isEmpty())
        return false;
    else
        return saveFile(fileName);
}

QSize Monofin::sizeHint() const
{
    return _layout->sizeHint();
}

// PUBLIC SLOTS

/*!
    \a a
*/
void Monofin::activateAddControl(bool a)
{
    _scene->activateAddControl(a);
}

/*!
    \a a
*/
void Monofin::activateAddPoint(bool a)
{
    _scene->activateAddPoint(a);
}

/*!
    \a a
*/
void Monofin::activateCreateLine(bool a)
{
    _scene->activateCreateLine(a);
}

/*!
    \a a
*/
void Monofin::activateRemoveControlPoint(bool a)
{
    _scene->activateRemoveControlPoint(a);
}

/*!

*/
void Monofin::alignTangents()
{
    _scene->alignTangents();
}

/*!

*/
void Monofin::cleanPoints()
{
    _scene->cleanPoints();
    _isEmpty = true;
}

/*!

*/
void Monofin::removeSelectedPoints()
{
    _scene->removeSelectedPoints();
}

// PROTECTED SLOTS

void Monofin::activeAddControl(bool a){
    qDebug("activeAddControl: %d", a);
    _actionAddPoint->setDisabled(a);
    _actionAlignTangents->setDisabled(a);
    _actionCleanPolygon->setDisabled(a);
    _actionRemoveControl->setDisabled(a);
}

void Monofin::activeAddPoint(bool a){
    qDebug("activeAddPoint: %d", a);
    _actionAddControl->setDisabled(a);
    _actionAlignTangents->setDisabled(a);
    _actionCleanPolygon->setDisabled(a);
    _actionRemoveControl->setDisabled(a);
}

void Monofin::activateRemoveControl(bool a){
    qDebug("activeRemoteControl: %d", a);
    _actionAddPoint->setDisabled(a);
    _actionAddControl->setDisabled(a);
    _actionAlignTangents->setDisabled(a);
    _actionCleanPolygon->setDisabled(a);
}

void Monofin::beginLine(bool a){
    _actionCreatePolygon->setDisabled(a);
    _actionGroupDraw->setDisabled(a);
}

void Monofin::clean(){
    _actionCreatePolygon->setEnabled(true);
    _actionGroupDraw->setDisabled(true);
}

void Monofin::finishedLine(bool a){
    _actionCreatePolygon->setDisabled(a);
    _actionGroupDraw->setEnabled(a);
}

// PROTECTED

/*!

*/
void Monofin::closeEvent(QCloseEvent *closeEvent)
{
    if (okToContinue())
        closeEvent->accept();
    else
        closeEvent->ignore();
}

void Monofin::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi();
        break;
    default:
        break;
    }
}

void Monofin::keyPressEvent(QKeyEvent *event)
{
    QCoreApplication::sendEvent(_scene.data(), event);
}

void Monofin::keyReleaseEvent(QKeyEvent *event)
{
    QCoreApplication::sendEvent(_scene.data(), event);
}

// PRIVATE SLOTS

/*!

*/
void Monofin::monofinWasModified()
{
    setWindowModified(true);
}

// TOOLBAR
void Monofin::on_actionAddControl_toggled(bool a)
{
    activateAddControl(a);
}

void Monofin::on_actionAddPoint_toggled(bool a)
{
    activateAddPoint(a);
}

void Monofin::on_actionCreatePolygon_toggled(bool a)
{
    activateCreateLine(a);
}

void Monofin::on_actionRemoveControl_toggled(bool a)
{
    activateRemoveControlPoint(a);
}

void Monofin::on_actionAlignTangents_triggered()
{
    alignTangents();
}

void Monofin::on_actionCleanPolygon_triggered()
{
    cleanPoints();
}

// PRIVATE

void Monofin::createToolBar()
{
    // ACTIONS
    _actionAddControl = new QAction(this);
    _actionAddControl->setObjectName(QString::fromUtf8("actionAddControl"));
    _actionAddControl->setCheckable(true);
    _actionAddPoint = new QAction(this);
    _actionAddPoint->setObjectName(QString::fromUtf8("actionAddPoint"));
    _actionAddPoint->setCheckable(true);
    _actionCreatePolygon = new QAction(this);
    _actionCreatePolygon->setObjectName(QString::fromUtf8("actionCreatePolygon"));
    _actionCreatePolygon->setCheckable(true);
    _actionCreatePolygon->setEnabled(true);
    _actionCleanPolygon = new QAction(this);
    _actionCleanPolygon->setObjectName(QString::fromUtf8("actionCleanPolygon"));
    _actionRemoveControl = new QAction(this);
    _actionRemoveControl->setObjectName(QString::fromUtf8("actionRemoveControl"));
    _actionRemoveControl->setCheckable(true);
    _actionAlignTangents = new QAction(this);
    _actionAlignTangents->setObjectName(QString::fromUtf8("actionAlignTangents"));
    _actionGroupDraw = new QActionGroup(this);
    _actionGroupDraw->setExclusive(false);
    _actionGroupDraw->addAction(_actionAddControl);
    _actionGroupDraw->addAction(_actionAddPoint);
    _actionGroupDraw->addAction(_actionCleanPolygon);
    _actionGroupDraw->addAction(_actionRemoveControl);
    _actionGroupDraw->addAction(_actionAlignTangents);

    // TOOLBAR
    _toolBarArea = Qt::LeftToolBarArea;
    _toolBar = new QToolBar();
    _toolBar->setObjectName(QString::fromUtf8("mainToolBar"));
    _toolBar->setFloatable(false);
    _toolBar->setAllowedAreas(Qt::LeftToolBarArea);
    _toolBar->addAction(_actionCreatePolygon);
    _toolBar->addAction(_actionCleanPolygon);
    _toolBar->addSeparator();
    _toolBar->addAction(_actionAddControl);
    _toolBar->addAction(_actionRemoveControl);
    _toolBar->addAction(_actionAddPoint);
    _toolBar->addSeparator();
    _toolBar->addAction(_actionAlignTangents);
}

void Monofin::initialize()
{
    if (!_layerView.isNull())
        _layerView->close();
    _layerView = new LayerView(_projectFile);
    if (!_scene.isNull())
        _scene->deleteLater();
    _scene = new PaintingScene(1024, 768, _projectFile, this);

    _layout->addWidget(new PaintingView(_scene.data()));
    _layout->addWidget(_layerView.data());
    createToolBar();
    retranslateUi();
    setLayout(_layout);
    setConnections();
    clean();
}

/*!

*/
bool Monofin::readFile(const QString &fileName)
{
    qDebug() << QString("Monofin::readFile(%1)").arg(fileName);
    _projectFile->loadProject(fileName);
    setCurrentFile(fileName);
    initialize();
    _scene->updateMonofinDrawing();
    return true;
}

void Monofin::retranslateUi()
{
    // TOOLBAR
    _toolBar->setWindowTitle(tr("Draw toolbar"));
    _actionAddControl->setText(tr("Add control point"));
    _actionAddPoint->setText(tr("Add point"));
    _actionCreatePolygon->setText(tr("Create polygon"));
    _actionCleanPolygon->setText(tr("Clean polygon"));
    _actionRemoveControl->setText(tr("Remove control point"));
    _actionAlignTangents->setText(tr("Align tangents"));
}

/*!

*/
bool Monofin::saveFile(const QString &fileName)
{
    qDebug() << QString("Monofin::saveFile(%1)").arg(fileName);
    _projectFile->saveProject(fileName);
    setCurrentFile(fileName);
    return true;
}

/*!

*/
void Monofin::setConnections()
{
    // TOOLBAR
    connect(_actionCreatePolygon, SIGNAL(toggled(bool)), this, SLOT(beginLine(bool)));
    connect(_actionCleanPolygon, SIGNAL(triggered()), this, SLOT(clean()));
    connect(_actionAddControl, SIGNAL(toggled(bool)), this, SLOT(activeAddControl(bool)));
    connect(_actionRemoveControl, SIGNAL(toggled(bool)), this, SLOT(activateRemoveControl(bool)));
    connect(_actionAddPoint, SIGNAL(toggled(bool)), this, SLOT(activeAddPoint(bool)));

    // SCENE
    connect(_scene.data(), SIGNAL(lineFinished(bool)), _actionCreatePolygon, SLOT(toggle()));
    connect(_scene.data(), SIGNAL(lineFinished(bool)), this, SLOT(finishedLine(bool)));
    connect(_scene.data(), SIGNAL(pointsOnScene(bool)), this, SLOT(finishedLine(bool)));
    connect(_scene.data(), SIGNAL(somethingChanged(int)), this, SLOT(monofinWasModified()));

    QMetaObject::connectSlotsByName(this);
}


/*!

*/
void Monofin::setCurrentFile(const QString &fileName)
{
    qDebug("entering Monofin::setCurrentFile(%s)...", fileName.toStdString().c_str());
    _curFile = fileName;
    setWindowModified(false);

    QString shownName = tr("(Untitled %1)").arg(documentNumber);
    _isUntitled = true;

    if (!_curFile.isEmpty()) {
        shownName = strippedName(_curFile);
        _isUntitled = false;
        emit currentFileChanged();
    }

    setWindowTitle(tr("%1[*]").arg(shownName));
    qDebug("leaving Monofin::setCurrentFile...");
}

/*!

*/
QString Monofin::strippedName(const QString &fullFileName)
{
    qDebug() << QString("entering Monofin::strippedName(%1)...").arg(fullFileName);
    QString str = QFileInfo(fullFileName).fileName();
    qDebug() << QString("leaving Monofin::strippedName with result %1").arg(str);
    return str;
}
