#include "monofin.h"

#include "Data/projectfile.h"
#include "Drawing/paintingscene.h"
#include "Drawing/paintingview.h"

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

    \sa MonofinFile, PaintingScene, PaintingView
*/

/*!
    Constructs a Monofin Object. The \a parent parameter is
    passed to the QWidget constructor.
*/
Monofin::Monofin(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    _curFile = QString();
    _isEmpty = true;
    _isUntitled = true;
    _layout = new QVBoxLayout;

    _projectFile = new Data::ProjectFile();
    _scene = new PaintingScene(1024, 768, _projectFile, this);

    _layout->addWidget(new PaintingView(_scene));
    createToolBar();
    retranslateUi();
    setLayout(_layout);
    setConnections();
    clean();
}

Monofin::~Monofin()
{
    qDebug("Monofin::~Monofin()");
}

// PUBLIC
/*!

*/
static int documentNumber = 1;

/*!

*/
void Monofin::newFile()
{
    setCurrentFile(QString());
    ++documentNumber;
}

/*!

*/
bool Monofin::okToContinue()
{
    if (isWindowModified()) {
        int r = QMessageBox::warning(0, tr("Monofin"),
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

*/
bool Monofin::open()
{
    if (okToContinue()) {
        QString fileName = QFileDialog::getOpenFileName(0,
                                                        tr("Open Monofin"), ".",
                                                        tr("Monofin files (*.fin)"));
        if (!fileName.isEmpty())
            return openFile(fileName);
    }
    return false;
}

/*!
    \a fileName
*/
bool Monofin::openFile(const QString &fileName)
{
    if (QFile::exists(fileName))
        return readFile(fileName);
    else
        return false;
}

/*!

*/
bool Monofin::save()
{
    if (_isUntitled)
        return saveAs();
    else
        return saveFile(_curFile);
}

/*!

*/
bool Monofin::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Monofin"), ".",
                                                    tr("Monofin files (*.fin)"));
    if(fileName.isEmpty())
        return false;
    else
        return saveFile(fileName);
}

/*!

*/
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
    if(a){
        _actionCreatePolygon->toggle();
        _actionCreatePolygon->setDisabled(true);
        _actionGroupDraw->setEnabled(true);
    }
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

/*!

*/
bool Monofin::readFile(const QString &fileName)
{
    // TODO
    setCurrentFile(fileName);
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
    // TODO
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
    connect(_scene, SIGNAL(lineFinished(bool)), this, SLOT(finishedLine(bool)));

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
    qDebug("entering Monofin::strippedName(%s)...", fullFileName.toStdString().c_str());
    QString str = QFileInfo(fullFileName).fileName();
    qDebug("leaving Monofin::strippedName with result %s", str.toStdString().c_str());
    return str;
}
