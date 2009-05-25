#include "graphic.h"
#include "ui_graphic.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <math.h>

using namespace Data;

Graphic::Graphic(QWidget *parent, ProjectFile* monofin, qreal width, qreal height) :
    QWidget(parent), _monofin(monofin), _preview(NULL)
{
    _graphic.setupUi(this);
    if(width == 0 || height == 0)
        _graphicsScene = new EdgesExtractionScene(_graphic.graphicWidget, 800, 600);
    else
        _graphicsScene = new EdgesExtractionScene(_graphic.graphicWidget, width,
                                                  height);
    _graphicsView = new EdgesExtractionView(_graphicsScene);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(_graphicsView);

    _graphic.graphicWidget->setLayout(layout);
    QObject::connect(_graphic.OpenButton, SIGNAL(clicked()),
                     this, SLOT(setPixmap()));
    QObject::connect(_graphic.CancelButton, SIGNAL(clicked()),
                     this, SLOT(close()));

    if(_monofin == 0)
        _monofin = new ProjectFile();

}

Graphic::~Graphic(){}

void Graphic::setSize(qreal width, qreal height){
    EdgesExtractionScene* newScene = new EdgesExtractionScene(_graphic.graphicWidget, width, height);
    _graphicsView->setScene(newScene);
    delete _graphicsScene;
    _graphicsScene = newScene;
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
    QString file = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "Images (*.png *.bmp)");
    QPixmap pix(file);
    _graphicsScene->setPixmap(pix);

    _graphic.posXSpinBox->setValue((int)_graphicsScene->pixItem()->boundingRect().center().x());
    _graphic.posYSpinBox->setValue((int)_graphicsScene->pixItem()->boundingRect().center().y());
    _graphic.scaleSpinBox->setValue(100);

    _graphic.posXSpinBox->setValue((int)_graphicsScene->pixItem()->boundingRect().center().x());
    _graphic.posYSpinBox->setValue((int)_graphicsScene->pixItem()->boundingRect().center().y());
    _graphic.scaleSpinBox->setValue(100);

    _algo = new AlgoSnake(_graphicsScene->pixItem()->scircle());
    _algo->setImage(new QImage(_graphicsScene->pixItem()->pixmap().toImage()));


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
    bool ok =_algo->edgesDetection(_graphicsScene->width());
    if(!ok){
        QMessageBox::warning(this, "Over position", "Warning, the image is positionned badly !\nSome points detected are not in the scene !");
        _algo->reinitialize();
    }else{
        ok =_algo->edgesExtraction(_monofin, _graphicsScene->heal(), _graphicsScene->symetryAxe());
        if(!ok){
            QMessageBox::warning(this, "No edge detected", "Warning, the image is positionned badly !\nThe axe of symetry is not detected !");
            _algo->reinitialize();
        }else{
            _preview = new DrawPreview(this, _monofin, _graphicsScene->width(), _graphicsScene->height());
            QObject::connect(_preview, SIGNAL(kept()), this, SLOT(kept()));
            QObject::connect(_preview, SIGNAL(doNotKept()), this, SLOT(doNotKept()));
            _preview->show();
        }
    }
    qDebug("quit");
}

void Graphic::kept(){
    /**
     * voir si on ferme la fenetre ou si on la cache
     */
    this->close();
}

void Graphic::doNotKept(){
    QObject::disconnect(_preview, SIGNAL(kept()), this, SLOT(kept()));
    QObject::disconnect(_preview, SIGNAL(doNotKept()), this, SLOT(doNotKept()));
    _algo->reinitialize();
}
