#include "graphic.h"
#include "ui_graphic.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <math.h>

using namespace Data;

Graphic::Graphic(QWidget *parent, ProjectFile* monofin, qreal width, qreal height) :
    QWidget(parent, Qt::Window), _monofin(monofin), _preview(NULL)
{
    this->setWindowModality(Qt::WindowModal);
    _graphic.setupUi(this);
    if(width == 0 || height == 0){
        _graphicsScene = new EdgesExtractionScene(_graphic.graphicWidget, 800, 600);
    }
    else{
        _graphicsScene = new EdgesExtractionScene(_graphic.graphicWidget, width,
                                                  height);
    }
    _graphicsView = new EdgesExtractionView(_graphicsScene);
    _graphicsView->reScale();

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(_graphicsView);

    _graphic.graphicWidget->layout()->deleteLater();
    _graphic.graphicWidget->setLayout(layout);
    QObject::connect(_graphic.OpenButton, SIGNAL(clicked()),
                     this, SLOT(setPixmap()));
    QObject::connect(_graphic.CancelButton, SIGNAL(clicked()),
                     this, SLOT(close()));

    if(_monofin == 0)
        _monofin = new ProjectFile();

    this->resize(1024, 768);
    this->move(50, 50);

    _preview = new DrawPreview(this, _monofin, _graphicsScene->width(), _graphicsScene->height());
    QObject::connect(_preview, SIGNAL(kept()), this, SIGNAL(kept()));
    QObject::connect(_preview, SIGNAL(kept()), this, SLOT(hide()));
    QObject::connect(_preview, SIGNAL(doNotKept()), this, SLOT(doNotKept()));

    _parametersDialog = new QDialog();

    _diag = Ui::Dialog();
    _diag.setupUi(_parametersDialog);
    _parametersDialog->setModal(true);

    _graphic.parametersButton->setDisabled(true);
    _diag.coeffDetectSnake->setValue(25);
    _diag.precisionPotrace->setValue(1.5);

    QObject::connect(_diag.buttonBox, SIGNAL(accepted()), this, SLOT(changeParameters()));
    QObject::connect(_diag.buttonBox, SIGNAL(rejected()), this, SLOT(doNotChangeParameters()));

    //_parametersDialog->buttonBox

     QObject::connect(_graphic.parametersButton, SIGNAL(clicked()), _parametersDialog, SLOT(show()));
}


void Graphic::setSize(qreal width, qreal height){
    EdgesExtractionScene* newScene = new EdgesExtractionScene(_graphic.graphicWidget, width, height);
    _graphicsView->setScene(newScene);
    _graphicsView->reScale();
    _graphicsScene->deleteLater();
    _graphicsScene = newScene;
    _graphic.parametersButton->setDisabled(true);
    _diag.coeffDetectSnake->setValue(25);
    _diag.precisionPotrace->setValue(1.5);
}

void Graphic::setProjectFile(ProjectFile *monofin, qreal width, qreal height){
    _monofin = monofin;
    if(width !=0 && height != 0)
        this->setSize(width, height);
    else
        this->setSize(_graphicsScene->width(), _graphicsScene->height());
}

void Graphic::setPixmap(){

    if(_graphicsScene->pixItem() != NULL){
        QObject::disconnect(_graphic.scaleSpinBox, SIGNAL(valueChanged(double)),
                         this, SLOT(setScale(double)));
        QObject::disconnect(_graphic.angleSpinBox, SIGNAL(valueChanged(double)),
                    this, SLOT(rotate(double)));
        QObject::disconnect(_graphicsScene, SIGNAL(rotateAngleChanged(double)),
                    _graphic.angleSpinBox, SLOT(setValue(double)));
        QObject::disconnect(_graphicsScene, SIGNAL(positionChanged()),
                         this, SLOT(positionChanged()));
        QObject::disconnect(_graphicsScene, SIGNAL(scaleChanged()),
                     this, SLOT(scaleChanged()));
        QObject::disconnect(_graphic.StartButton, SIGNAL(clicked()),
                         this, SLOT(startAlgo()));
        QObject::disconnect(_graphic.posXSpinBox, SIGNAL(valueChanged(int)),
                     this, SLOT(setPixmapPositionX(int)));
        QObject::disconnect(_graphic.posYSpinBox, SIGNAL(valueChanged(int)),
                     this, SLOT(setPixmapPositionY(int)));
    }
    QString file = QFileDialog::getOpenFileName(this, tr("Open a file"), QString(),
                                                tr("All Supported Image Format (*.bmp *.gif *.jpg *.jpeg *.mng *.png *.pbm *.pgm *.ppm *.tiff *.xbm *.xpm"
                                                   "Windows Bitmap (*.bmp);;"
                                                   "Graphic Interchange Format (*.gif);;"
                                                   "JPEG files (*.jpg *.jpeg);;"
                                                   "Portable Network Graphics (*.png);;"
                                                   "Portable Bitmap (*.pbm *.pgm *.ppm);;"
                                                   "Tagged Image File Format (*.tiff);;"
                                                   "X11 Format (*.xbm *.xpm);;"));
    if(file != ""){
        QPixmap pix(file);
        _graphicsScene->setPixmap(pix);

        _graphic.posXSpinBox->setValue(_graphicsScene->pixItem()->boundingRect().center().toPoint().x());
        _graphic.posYSpinBox->setValue(_graphicsScene->pixItem()->boundingRect().center().toPoint().y());
        _graphic.scaleSpinBox->setValue(100);

        _graphic.posXSpinBox->setValue(_graphicsScene->pixItem()->boundingRect().center().toPoint().x());
        _graphic.posYSpinBox->setValue(_graphicsScene->pixItem()->boundingRect().center().toPoint().y());
        _graphic.scaleSpinBox->setValue(100);

        if(_algo != NULL){
             QObject::disconnect(_graphic.StartButton, SIGNAL(clicked()),
                         this, SLOT(startAlgo()));
            delete _algo;
        }
        _algo = new AlgoSnake(_graphicsScene->pixItem()->scircle());
        _algo->setImage(new QImage(_graphicsScene->pixItem()->pixmap().toImage()));


        _diag.numberPointsSnake->setValue(_algo->numberPointsSnake());
        _graphic.parametersButton->setDisabled(false);
    }

    if(_graphicsScene->pixItem() != NULL){
        QObject::connect(_graphic.scaleSpinBox, SIGNAL(valueChanged(double)),
                         this, SLOT(setScale(double)));
        QObject::connect(_graphic.angleSpinBox, SIGNAL(valueChanged(double)),
                    this, SLOT(rotate(double)));
        QObject::connect(_graphicsScene, SIGNAL(rotateAngleChanged(double)),
                    _graphic.angleSpinBox, SLOT(setValue(double)));
        QObject::connect(_graphicsScene, SIGNAL(positionChanged()),
                         this, SLOT(positionChanged()));
        QObject::connect(_graphicsScene, SIGNAL(scaleChanged()),
                         this, SLOT(scaleChanged()));
        QObject::connect(_graphic.StartButton, SIGNAL(clicked()),
                         this, SLOT(startAlgo()));
        QObject::connect(_graphic.posXSpinBox, SIGNAL(valueChanged(int)),
                         this, SLOT(setPixmapPositionX(int)));
        QObject::connect(_graphic.posYSpinBox, SIGNAL(valueChanged(int)),
                         this, SLOT(setPixmapPositionY(int)));
    }
}

void Graphic::rotate(double angle){
    if(!_graphicsScene->rotCircle()->isMoved()){
        _graphicsScene->pixItem()->rotate2(angle);
        _graphicsScene->rotCircle()->setPosition(angle);
        _graphicsScene->update();
    }
}

void Graphic::setScale(double scale){
    PixmapItem* pi = _graphicsScene->pixItem();
    pi->translate(pi->boundingRect().center().x(), pi->boundingRect().center().y());
    pi->scaled(scale / 100);
    pi->translate(- pi->boundingRect().center().x(), - pi->boundingRect().center().y());
    _graphicsScene->update();
}

void Graphic::positionChanged(){
    QObject::disconnect(_graphic.posXSpinBox, SIGNAL(valueChanged(int)),
                     this, SLOT(setPixmapPositionX(int)));
    QObject::disconnect(_graphic.posYSpinBox, SIGNAL(valueChanged(int)),
                     this, SLOT(setPixmapPositionY(int)));
    _graphic.posXSpinBox->setValue((int)_graphicsScene->pixItem()->boundingRect().center().x());
    _graphic.posYSpinBox->setValue((int)_graphicsScene->pixItem()->boundingRect().center().y());
    QObject::connect(_graphic.posXSpinBox, SIGNAL(valueChanged(int)),
                     this, SLOT(setPixmapPositionX(int)));
    QObject::connect(_graphic.posYSpinBox, SIGNAL(valueChanged(int)),
                     this, SLOT(setPixmapPositionY(int)));
}

void Graphic::scaleChanged(){
    QObject::disconnect(_graphic.scaleSpinBox, SIGNAL(valueChanged(double)),
                        this, SLOT(setScale(double)));
    _graphic.scaleSpinBox->setValue(_graphicsScene->pixItem()->getScale() * 100);
    QObject::connect(_graphic.scaleSpinBox, SIGNAL(valueChanged(double)),
                        this, SLOT(setScale(double)));
}

void Graphic::setPixmapPositionX(int x){
    PixmapItem* pix = _graphicsScene->pixItem();
    pix->translate2(x - pix->boundingRect().center().x(), 0);
    _graphicsScene->update();
}

void Graphic::setPixmapPositionY(int y){
    PixmapItem* pix = _graphicsScene->pixItem();
    pix->translate2(0, y - pix->boundingRect().center().y());
    _graphicsScene->update();
}

void Graphic::startAlgo(){
    bool ok =_algo->edgesDetection(_graphicsScene->width(), _graphicsScene->height());
    if(!ok){
        QMessageBox::warning(this, tr("Over position"), tr("Warning, the image is not correctly positionned!\nSome detected points are not in the scene."));
        _algo->reinitialize();
    }else{
        ok =_algo->edgesExtraction(_monofin, _graphicsScene->heal(), _graphicsScene->symetryAxe());
        if(!ok){
            QMessageBox::warning(this, tr("No edge detected"), tr("Warning, the image is not correctly positionned!\nThe symmetry axis is not detected."));
            _algo->reinitialize();
        }else{
            _preview->setProjectFile(_monofin, _graphicsScene->width(), _graphicsScene->height());
            _preview->show();
        }
    }
}

/* void Graphic::kept(){
     // voir si on ferme la fenetre ou si on la cache
    qDebug("MaMonopalme");
    this->close();
}*/

void Graphic::doNotKept(){
    _algo->reinitialize();
}

void Graphic::changeParameters(){
    _algo->setParameters(_diag.numberPointsSnake->value(),
                             _diag.coeffDetectSnake->value(),
                             _diag.precisionPotrace->value());
}

void Graphic::doNotChangeParameters(){
    _diag.numberPointsSnake->setValue(_algo->numberPointsSnake());
    _diag.coeffDetectSnake->setValue(_algo->coefficientDetectionSnake());
    _diag.precisionPotrace->setValue(_algo->precisionPotrace());
}
