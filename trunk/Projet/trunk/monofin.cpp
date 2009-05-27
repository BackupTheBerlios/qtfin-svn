#include "monofin.h"
#include "Data/projectfile.h"
#include "Drawing/paintingview.h"
#include "Ui/geometry3dviewer.h"
#include "Ui/generatecomsolfile.h"
#include "Ui/insertlayerdialog.h"
#include "Ui/layerindexdialog.h"
#include "Ui/layerparameters.h"
#include "Ui/parametersdialog.h"

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
    _paramDiag = new ParametersDialog(this);

    _viewer = new Geometry3DViewer(*_projectFile);
    _generator = new GenerateComsolFile(*_projectFile);
    createToolBar();
}
/*!
    Destroys the monofin.

    All this widget's children are deleted first. The application
    exits if this widget is the main widget.
*/
Monofin::~Monofin()
{
    qDebug("Monofin::~Monofin()");
    if (_projectFile) {
        qDebug("delete Data::Project");
        delete _projectFile;
    }
}

// PUBLIC
/*!

*/
static int documentNumber = 1;

/*!

*/
int Monofin::getNbLayers()
{
    if (!_layerView.isNull())
        return _layerView->nbLayers();
    else
        return 0;
}

/*!
    Create a new file.
*/
void Monofin::newFile()
{
    qDebug("Monofin::newFile()");
    setCurrentFile(QString());
    ++documentNumber;
    initialize();
}

/*!

*/
bool Monofin::newFileFromImage()
{
    qDebug("Monofin::newFileFromImage()");
    newFile();
    /*if (_graphicView.isNull())
        _graphicView = new Graphic(0, _projectFile, _scene->width(), _scene->height());
    _projectFile->startHistory(Data::MonofinSurface);
    _projectFile->clearSurface();
    _projectFile->stopHistory(Data::MonofinSurface);
    int res = _graphicView->exec();
    if (res) {
        qDebug("Graphic.exec() return true");
        _scene->updateMonofinDrawing();
        return true;
    } else {
        qDebug("Graphic.exec() return false");
        _projectFile->undo(Data::MonofinSurface);
        return false;
    }*/
    _scene->updateMonofinDrawing();
    return true;
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

void Monofin::activateMagnet(bool a)
{
    _scene->activateMagnet(a);
}

void Monofin::activateModifyBackgroundPicture(bool a)
{
    _scene->modifyBackgroundPicture(a);
}

/*!
    \a a
*/
void Monofin::activateRemoveControlPoint(bool a)
{
    _scene->activateRemoveControlPoint(a);
}

void Monofin::addBackgroundPicture()
{
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Open file"),
                                                QString(),
                                                "Images (*.png *.bmp *.jpg)");
    if(!file.isNull()){
        QPixmap pix(file);
        _scene->setBackGroundPicture(pix);
        //this->addABackgroundPicture();
    }
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

void Monofin::configurate()
{
    _paramDiag->setNbLayers(_layerView->nbLayers());
    if(_paramDiag->exec()) {
        QTabWidget *qtw = _paramDiag->layerTabWidget;
        if (qtw->count() > 0) {
            _projectFile->startHistory(Data::MonofinLayerConfig);
            for(int i = 0; i<qtw->count(); ++i) {
                LayerParameters * ll = static_cast<LayerParameters*> (qtw->widget(i));
                _projectFile->setLayerConfigPoisson(i, ll->poissonDoubleSpinBox->value());
                _projectFile->setLayerConfigRho(i, ll->densityDoubleSpinBox->value());
                _projectFile->setLayerConfigYoung(i, ll->youngDoubleSpinBox->value());
            }
            _projectFile->stopHistory(Data::MonofinLayerConfig);
        }
    }
}

void Monofin::decreaseGridUnitSize()
{
    _scene->setGridUnit(_scene->gridUnit() - 1);
}

void Monofin::decreaseWindowSize()
{
    _scene->adjustSceneRect(-20,-20);
}

void Monofin::increaseGridUnitSize()
{
    _scene->setGridUnit(_scene->gridUnit() + 1);
}

void Monofin::increaseWindowSize()
{
    _scene->adjustSceneRect(20,20);
}

void Monofin::keepBezierCurve(bool a)
{
    _scene->keepBezierCurve(a);
}

void Monofin::launch()
{
    _generator->show();
}

void Monofin::preview3D()
{
    _viewer->show();
}

void Monofin::redo()
{
    _scene->redo();
}

void Monofin::removeBackgroundPicture()
{
    _scene->removeBackgroundPicture();
}

/*!

*/
void Monofin::removeSelectedPoints()
{
    _scene->removeSelectedPoints();
}

void Monofin::saveForm(QString path){
    QImage image = _scene->getPictureOfTheScene(64,64).toImage();
    _projectFile->saveForm(path, QString("Untitled"), image);
}

void Monofin::simplifyView(bool a)
{
    _scene->simplifyView(a);
}

void Monofin::showGrid(bool a){
    _scene->showGrid(a);
}

void Monofin::switchToBlack()
{
    _scene->changeColor(PaintingScene::LineColor, PaintingScene::NormalColor, QColor(Qt::black));
    _scene->changeColor(PaintingScene::BoundingPointColor, PaintingScene::NormalColor, QColor(Qt::black));
    _scene->changeColor(PaintingScene::ControlPointColor, PaintingScene::NormalColor, QColor(Qt::black));
    _scene->changeColor(PaintingScene::BoundingPointColor, PaintingScene::SelectionColor, QColor(Qt::red));
}

void Monofin::switchToRed()
{
    _scene->changeColor(PaintingScene::LineColor, PaintingScene::NormalColor, QColor(Qt::red));
    //_scene->changeColor(PaintingScene::BoundingPointColor, PaintingScene::NormalColor, QColor(Qt::red));
    _scene->changeColor(PaintingScene::ControlPointColor, PaintingScene::NormalColor, QColor(Qt::red));
}

void Monofin::switchToWhite()
{
    _scene->changeColor(PaintingScene::LineColor, PaintingScene::NormalColor, QColor(Qt::white));
    _scene->changeColor(PaintingScene::BoundingPointColor, PaintingScene::NormalColor, QColor(Qt::gray));
    _scene->changeColor(PaintingScene::ControlPointColor, PaintingScene::NormalColor, QColor(Qt::white));
}

void Monofin::undo()
{
    _scene->undo();
}

void Monofin::zoomIn(){

}

void Monofin::zoomInOnBackgroundPicture()
{
    _scene->zoomOnBackgroundPicture(1.1);
}

void Monofin::zoomOut(){


}

void Monofin::zoomOutOnBackgroundPicture()
{
    _scene->zoomOnBackgroundPicture(0.9);
}

// PROTECTED SLOTS

void Monofin::activeAddControl(bool a){
    qDebug("activeAddControl: %d", a);
    _actionAddPoint->setDisabled(a);
    _actionAlignTangents->setDisabled(a);
    _actionCleanPolygon->setDisabled(a);
    _actionRemoveControl->setDisabled(a);
    _actionRemovePoints->setDisabled(a);
    _actionTransformBackgroundPicture->setDisabled(a);
}

void Monofin::activeAddPoint(bool a){
    qDebug("activeAddPoint: %d", a);
    _actionAddControl->setDisabled(a);
    _actionAlignTangents->setDisabled(a);
    _actionCleanPolygon->setDisabled(a);
    _actionRemoveControl->setDisabled(a);
    _actionRemovePoints->setDisabled(a);
    _actionTransformBackgroundPicture->setDisabled(a);
}

void Monofin::activeModifyBackgroundPicture(bool a){
    if(_isEmpty){
        _actionCreatePolygon->setDisabled(a);
    }
    if(a){
        _actionGroupDraw->setDisabled(true);
    }else{
        if(!_isEmpty){
            _actionGroupDraw->setEnabled(true);
        }
    }
    _enlargeBackgroundPictureSize->setEnabled(a);
    _reduceBackgroundPictureSize->setEnabled(a);
}

void Monofin::activateRemoveControl(bool a){
    qDebug("activeRemoveControl: %d", a);
    _actionAddPoint->setDisabled(a);
    _actionAddControl->setDisabled(a);
    _actionAlignTangents->setDisabled(a);
    _actionCleanPolygon->setDisabled(a);
    _actionRemovePoints->setDisabled(a);
    _actionTransformBackgroundPicture->setDisabled(a);
}

/*void Monofin::addABackgroundPicture(){
    _actionTransformBackgroundPicture->setEnabled(true);
}*/

void Monofin::beginLine(bool a){
    _actionGroupDraw->setDisabled(a || !_actionCreatePolygon->isChecked());
    _actionTransformBackgroundPicture->setDisabled(a);
}

void Monofin::clean(){
    _actionCreatePolygon->setEnabled(true);
    _actionGroupDraw->setDisabled(true);
}

void Monofin::finishedLine(bool a){
    _actionCreatePolygon->setDisabled(a);
    _actionGroupDraw->setEnabled(a);
    _isEmpty = !a;
}

/*void Monofin::removeABackgroundPicture(){
    _actionTransformBackgroundPicture->setDisabled(true);
}*/

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

void Monofin::on_actionInsertLayer_triggered()
{
    InsertLayerDialog *ild =  new InsertLayerDialog(this);
    ild->setNbLayer(_layerView->nbLayers());
    if (ild->exec()) {
        int r = ild->getLayerRank() + ild->before();
        _layerView->addLayerItem(r, ild->getLayerHeight(), ild->getLayerLength());
    }
    ild->deleteLater();
}

void Monofin::on_actionRemoveLayer_triggered()
{
    if (_layerView->nbLayers() < 2) {
        // alert user
    } else {
        LayerIndexDialog *lid = new LayerIndexDialog(this);
        lid->setNbLayer(_layerView->nbLayers());
        if (lid->exec()) {
            int r = lid->getLayerRank();
            _layerView->removeLayerItem(r);
        }
        lid->deleteLater();
    }
}

// PRIVATE

void Monofin::createToolBar()
{
    // ACTIONS

    _actionUndo = new QAction(this);
    _actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
    _actionUndo->setIcon(QIcon(":/icons/drawing/undo.png"));

    _actionRedo = new QAction(this);
    _actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
    _actionRedo->setIcon(QIcon(":/icons/drawing/redo.png"));

    _actionAddControl = new QAction(this);
    _actionAddControl->setObjectName(QString::fromUtf8("actionAddControl"));
    _actionAddControl->setCheckable(true);
    _actionAddControl->setIcon(QIcon(":/icons/drawing/createControlPoint.png"));

    _actionAddPoint = new QAction(this);
    _actionAddPoint->setObjectName(QString::fromUtf8("actionAddPoint"));
    _actionAddPoint->setCheckable(true);
    _actionAddPoint->setIcon(QIcon(":/icons/drawing/createPoint.png"));

    _actionRemovePoints = new QAction(this);
    _actionRemovePoints->setObjectName(QString::fromUtf8("actionRemovePoint"));
    _actionRemovePoints->setIcon(QIcon(":/icons/drawing/erasePoint.png"));
    _actionRemovePoints->setShortcut(QKeySequence(QKeySequence::Delete));

    _actionCreatePolygon = new QAction(this);
    _actionCreatePolygon->setObjectName(QString::fromUtf8("actionCreatePolygon"));
    _actionCreatePolygon->setCheckable(true);
    _actionCreatePolygon->setEnabled(true);
    _actionCreatePolygon->setIcon(QIcon(":/icons/drawing/beginLine.png"));

    _actionCleanPolygon = new QAction(this);
    _actionCleanPolygon->setObjectName(QString::fromUtf8("actionCleanPolygon"));
    _actionCleanPolygon->setIcon(QIcon(":/icons/drawing/clear.png"));

    _actionRemoveControl = new QAction(this);
    _actionRemoveControl->setObjectName(QString::fromUtf8("actionRemoveControl"));
    _actionRemoveControl->setCheckable(true);
    _actionRemoveControl->setIcon(QIcon(":/icons/drawing/eraseControlPoint.png"));

    _actionAlignTangents = new QAction(this);
    _actionAlignTangents->setObjectName(QString::fromUtf8("actionAlignTangents"));
    _actionAlignTangents->setIcon(QIcon(":/icons/drawing/alignTangent.png"));

    _actionGroupDraw = new QActionGroup(this);
    _actionGroupDraw->setExclusive(false);

    _actionGroupDraw->addAction(_actionAddControl);
    _actionGroupDraw->addAction(_actionAddPoint);
    _actionGroupDraw->addAction(_actionRemovePoints);
    _actionGroupDraw->addAction(_actionCleanPolygon);
    _actionGroupDraw->addAction(_actionRemoveControl);
    _actionGroupDraw->addAction(_actionAlignTangents);

    _actionInsertLayer = new QAction(this);
    _actionInsertLayer->setObjectName(QString::fromUtf8("actionInsertLayer"));
    _actionInsertLayer->setIcon(QIcon(":/icons/drawing/addLayer.png"));

    _actionRemoveLayer = new QAction(this);
    _actionRemoveLayer->setObjectName(QString::fromUtf8("actionRemoveLayer"));
    _actionRemoveLayer->setIcon(QIcon(":/icons/drawing/removeLayer.png"));

    _actionMagnet = new QAction(this);
    _actionMagnet->setObjectName(QString::fromUtf8("actionMagnet"));
    _actionMagnet->setCheckable(true);
    _actionMagnet->setIcon(QIcon(":/icons/drawing/magnet.png"));


    _actionSwitchColors = new QAction(this);
    _actionSwitchColors->setObjectName(QString::fromUtf8("actionSwitchColors"));
    _actionSwitchColors->setIcon(QIcon(":/icons/drawing/switchColor.png"));

    _menuColors = new QMenu(this);
    _menuColors->setObjectName(QString::fromUtf8("menuColors"));
    _menuColors->setIcon(QIcon(":/icons/drawing/switchColor.png"));

    _actionSwitchToRed = new QAction(this);
    _actionSwitchToRed->setObjectName(QString::fromUtf8("actionSwitchToRed"));

    _actionSwitchToBlack = new QAction(this);
    _actionSwitchToBlack->setObjectName(QString::fromUtf8("actionSwitchToBlack"));

    _actionSwitchToWhite = new QAction(this);
    _actionSwitchToWhite->setObjectName(QString::fromUtf8("actionSwitchToWhite"));

    _menuColors->addAction(_actionSwitchToBlack);
    _menuColors->addAction(_actionSwitchToRed);
    _menuColors->addAction(_actionSwitchToWhite);

    _actionSwitchColors->setMenu(_menuColors);


    _actionSimplifyView = new QAction(this);
    _actionSimplifyView->setObjectName(QString::fromUtf8("actionSimplifyView"));
    _actionSimplifyView->setCheckable(true);

    _actionIncreaseWindowSize = new QAction(this);
    _actionIncreaseWindowSize->setObjectName(QString::fromUtf8("actionIncreaseWindowSize"));
    _actionIncreaseWindowSize->setIcon(QIcon(":/icons/drawing/increaseSizeWindow.png"));

    _actionDecreaseWindowSize = new QAction(this);
    _actionDecreaseWindowSize->setObjectName(QString::fromUtf8("actionDecreaseWindowSize"));
    _actionDecreaseWindowSize->setIcon(QIcon(":/icons/drawing/decreaseSizeWindow.png"));

    _increaseGridUnitSize = new QAction(this);
    _increaseGridUnitSize->setObjectName(QString::fromUtf8("increaseGridUnitSize"));
    _increaseGridUnitSize->setShortcut(QKeySequence(Qt::Key_Plus));
    _increaseGridUnitSize->setIcon(QIcon(":/icons/drawing/increaseGridUnit.png"));

    _decreaseGridUnitSize = new QAction(this);
    _decreaseGridUnitSize->setObjectName(QString::fromUtf8("decreaseGridUnitSize"));
    _decreaseGridUnitSize->setShortcut(QKeySequence(Qt::Key_Minus));
    _decreaseGridUnitSize->setIcon(QIcon(":/icons/drawing/decreaseGridUnit.png"));

    _actionAddBackgroundPicture = new QAction(this);
    _actionAddBackgroundPicture->setObjectName(QString::fromUtf8("actionAddBackgroundPicture"));
    _actionAddBackgroundPicture->setIcon(QIcon(":/icons/drawing/background.png"));

    _actionTransformBackgroundPicture = new QAction(this);
    _actionTransformBackgroundPicture->setObjectName(QString::fromUtf8("actionAddBackgroundPicture"));
    _actionTransformBackgroundPicture->setCheckable(true);
    _actionTransformBackgroundPicture->setIcon(QIcon(":/icons/drawing/transformBackground.png"));


    _actionRemoveBackgroundPicture = new QAction(this);
    _actionRemoveBackgroundPicture->setObjectName(QString::fromUtf8("actionRemoveBackgroundPicture"));
    _actionRemoveBackgroundPicture->setIcon(QIcon(":/icons/drawing/removeBackground.png"));

    _enlargeBackgroundPictureSize = new QAction(this);
    _enlargeBackgroundPictureSize->setObjectName(QString::fromUtf8("enlargeBackgroundPictureSize"));
    _enlargeBackgroundPictureSize->setDisabled(true);
    _enlargeBackgroundPictureSize->setIcon(QIcon(":/icons/drawing/enlargeBackgroundSize.png"));

    _reduceBackgroundPictureSize = new QAction(this);
    _reduceBackgroundPictureSize->setObjectName(QString::fromUtf8("reduceBackgroundPictureSize"));
    _reduceBackgroundPictureSize->setDisabled(true);
    _reduceBackgroundPictureSize->setIcon(QIcon(":/icons/drawing/reduceBackgroundSize.png"));

    // TOOLBAR
    _toolBarDrawArea = Qt::TopToolBarArea;
    _toolBarViewArea = Qt::LeftToolBarArea;

    QToolBar *toolBarDraw = new QToolBar();

    toolBarDraw->setIconSize(QSize(48, 48));
    toolBarDraw->setObjectName(QString::fromUtf8("mainToolBar"));
    toolBarDraw->setFloatable(false);
    toolBarDraw->setAllowedAreas(_toolBarDrawArea);


    QToolBar *toolBarView = new QToolBar();
    toolBarView->setIconSize(QSize(48, 48));
    toolBarView->setObjectName(QString::fromUtf8("viewToolBar"));
    toolBarView->setFloatable(false);
    toolBarView->setAllowedAreas(_toolBarViewArea);

    toolBarDraw->addAction(_actionUndo);
    toolBarDraw->addAction(_actionRedo);
    toolBarDraw->addSeparator();
    toolBarDraw->addAction(_actionCreatePolygon);
    toolBarDraw->addAction(_actionCleanPolygon);
    toolBarDraw->addSeparator();
    toolBarDraw->addAction(_actionAddControl);
    toolBarDraw->addAction(_actionRemoveControl);
    toolBarDraw->addAction(_actionAddPoint);
    toolBarDraw->addAction(_actionRemovePoints);
    toolBarDraw->addSeparator();
    toolBarDraw->addAction(_actionAlignTangents);
    toolBarDraw->addSeparator();
    toolBarDraw->addAction(_actionMagnet);
    toolBarDraw->addSeparator();
    toolBarDraw->addAction(_actionInsertLayer);
    toolBarDraw->addAction(_actionRemoveLayer);
    toolBarDraw->addSeparator();
    toolBarDraw->addAction(_actionAddBackgroundPicture);
    toolBarDraw->addAction(_actionRemoveBackgroundPicture);
    toolBarDraw->addAction(_actionTransformBackgroundPicture);
    toolBarDraw->addAction(_enlargeBackgroundPictureSize);
    toolBarDraw->addAction(_reduceBackgroundPictureSize);



    toolBarView->addAction(_actionSimplifyView);
    toolBarView->addAction(_actionSwitchColors);
    toolBarView->addAction(_actionIncreaseWindowSize);
    toolBarView->addAction(_actionDecreaseWindowSize);
    toolBarView->addAction(_increaseGridUnitSize);
    toolBarView->addAction(_decreaseGridUnitSize);


    _toolBars.append(toolBarDraw); //first
    _toolBars.append(toolBarView); //second
}

void Monofin::initialize()
{
    qDebug("Monofin::initialize()");
    if (!_layerView.isNull()) {
        _layout->removeWidget(_layerView.data());
        _layerView->close();
    }
    _layerView = new LayerView(_projectFile);
    if (!_scene.isNull())
        _scene->deleteLater();
    _scene = new PaintingScene(800, 600, _projectFile);

    _layout->addWidget(new PaintingView(_scene.data(), this));
    _layout->addWidget(_layerView.data());
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
    if(_toolBars.size() >= 1){
        _toolBars.first()->setWindowTitle(tr("Draw toolbar"));
        if(_toolBars.size() >= 2){
            _toolBars.at(1)->setWindowTitle(tr("View toolbar"));
        }
    }
    _actionUndo->setText(tr("Undo"));
    _actionUndo->setShortcut(tr("Ctrl+Z"));
    _actionRedo->setText(tr("Redo"));
    _actionRedo->setShortcut(tr("Ctrl+Shift+Z"));
    _actionAddControl->setText(tr("Add control point"));
    _actionAddPoint->setText(tr("Add point"));
    _actionRemovePoints->setText(tr("Remove selected points"));
    _actionCreatePolygon->setText(tr("Create polygon"));
    _actionCleanPolygon->setText(tr("Clean polygon"));
    _actionRemoveControl->setText(tr("Remove control point"));
    _actionAlignTangents->setText(tr("Align tangents"));
    _actionSimplifyView->setText(tr("Simplify the view"));
    _actionMagnet->setText(tr("Activate / Deactivate the magnet"));
    _actionSwitchColors->setText(tr("Switch colors"));
    _actionSwitchToBlack->setText(tr("Black"));
    _actionSwitchToRed->setText(tr("Red"));
    _actionSwitchToWhite->setText(tr("White"));
    _actionInsertLayer->setText(tr("Add a layer"));
    _actionRemoveLayer->setText(tr("Remove a layer"));
    _actionIncreaseWindowSize->setText(tr("Increase size of the drawing zone"));
    _actionDecreaseWindowSize->setText(tr("Decrease size of the drawing zone"));
    _actionAddBackgroundPicture->setText(tr("Add a background picture"));
    _actionTransformBackgroundPicture->setText(tr("Transform the background picture"));
    _actionRemoveBackgroundPicture->setText(tr("Remove the background picture"));
    _enlargeBackgroundPictureSize->setText(tr("Enlarge picture size"));
    _reduceBackgroundPictureSize->setText(tr("Reduce picture size"));
    _increaseGridUnitSize->setText(tr("Increase size of the grid unit"));
    _decreaseGridUnitSize->setText(tr("Decrease size of the grid unit"));
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
    connect(_actionUndo, SIGNAL(triggered()), this, SLOT(undo()));
    connect(_actionRedo, SIGNAL(triggered()), this, SLOT(redo()));
    connect(_actionCreatePolygon, SIGNAL(toggled(bool)), this, SLOT(beginLine(bool)));
    connect(_actionCleanPolygon, SIGNAL(triggered()), this, SLOT(clean()));
    connect(_actionAddControl, SIGNAL(toggled(bool)), this, SLOT(activeAddControl(bool)));
    connect(_actionRemoveControl, SIGNAL(toggled(bool)), this, SLOT(activateRemoveControl(bool)));
    connect(_actionAddPoint, SIGNAL(toggled(bool)), this, SLOT(activeAddPoint(bool)));
    connect(_actionRemovePoints, SIGNAL(triggered()), this, SLOT(removeSelectedPoints()));
    connect(_actionIncreaseWindowSize, SIGNAL(triggered()), this, SLOT(increaseWindowSize()));
    connect(_actionDecreaseWindowSize, SIGNAL(triggered()), this, SLOT(decreaseWindowSize()));
    connect(_actionMagnet, SIGNAL(toggled(bool)), this, SLOT(activateMagnet(bool)));
    connect(_actionSimplifyView, SIGNAL(toggled(bool)), this, SLOT(simplifyView(bool)));
    connect(_actionAddBackgroundPicture, SIGNAL(triggered()), this, SLOT(addBackgroundPicture()));
    connect(_actionTransformBackgroundPicture, SIGNAL(toggled(bool)), this, SLOT(activateModifyBackgroundPicture(bool)));
    connect(_actionTransformBackgroundPicture, SIGNAL(toggled(bool)), this, SLOT(activeModifyBackgroundPicture(bool)));
    connect(_actionRemoveBackgroundPicture, SIGNAL(triggered()), this, SLOT(removeBackgroundPicture()));
    //connect(_actionRemoveBackgroundPicture, SIGNAL(triggered()), this, SLOT(removeABackgroundPicture()));
    connect(_actionSwitchToBlack, SIGNAL(triggered()), this, SLOT(switchToBlack()));
    connect(_actionSwitchToRed, SIGNAL(triggered()), this, SLOT(switchToRed()));
    connect(_actionSwitchToWhite, SIGNAL(triggered()), this, SLOT(switchToWhite()));
    connect(_enlargeBackgroundPictureSize, SIGNAL(triggered()), this, SLOT(zoomInOnBackgroundPicture()));
    connect(_reduceBackgroundPictureSize, SIGNAL(triggered()), this, SLOT(zoomOutOnBackgroundPicture()));
    connect(_increaseGridUnitSize, SIGNAL(triggered()), this, SLOT(increaseGridUnitSize()));
    connect(_decreaseGridUnitSize, SIGNAL(triggered()), this, SLOT(decreaseGridUnitSize()));


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
    qDebug() << QString("entering Monofin::setCurrentFile(%1)...").arg(fileName);
    _curFile = fileName;
    setWindowModified(false);

    QString shownName = tr("(Untitled %1)").arg(documentNumber);
    _isUntitled = true;

    if (!_curFile.isEmpty()) {
        shownName = strippedName(_curFile);
        qDebug() << QString("setting current file name %1").arg(shownName);
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
