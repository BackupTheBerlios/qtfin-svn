#include "graphic.h"
#include "ui_graphic.h"
#include <QVBoxLayout>
#include <QFileDialog>

Graphic::Graphic(QWidget *parent) :
    QWidget(parent)
{
    _graphic.setupUi(this);

    _graphicsScene = new EdgesExtractionScene(_graphic.graphicWidget);
    _graphicsView = new EdgesExtractionView(_graphicsScene);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(_graphicsView);

    _graphic.graphicWidget->setLayout(layout);
    /*_graphic.posXSpinBox->setValue((int)_graphicsScene->pixItem()->boundingRect().center().x());
    _graphic.posYSpinBox->setValue((int)_graphicsScene->pixItem()->boundingRect().center().y());

    _algo = new AlgoSnake(_graphicsScene->pixItem()->scircle());
    _algo->setImage(new QImage(_graphicsScene->pixItem()->pixmap().toImage()));

    QObject::connect(_graphic.scaleSpinBox, SIGNAL(valueChanged(double)),
                     this, SLOT(scaleChanged(double)));
    QObject::connect(_graphic.angleSpinBox, SIGNAL(valueChanged(double)),
                this, SLOT(rotate(double)));
    QObject::connect(_graphicsScene, SIGNAL(rotateAngleChanged(double)),
                _graphic.angleSpinBox, SLOT(setValue(double)));
    QObject::connect(_graphicsScene, SIGNAL(positionChanged()),
                     this, SLOT(positionChanged()));
    QObject::connect(_graphic.StartButton, SIGNAL(clicked()),
                     this, SLOT(startAlgo()));*/
    QObject::connect(_graphic.OpenButton, SIGNAL(clicked()),
                     this, SLOT(setPixmap()));

}

Graphic::~Graphic(){}

void Graphic::setPixmap(){

    QString file = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "Images (*.png *.bmp)");

    _graphicsScene->setPixmap(QPixmap(file));

    _graphic.posXSpinBox->setValue((int)_graphicsScene->pixItem()->boundingRect().center().x());
    _graphic.posYSpinBox->setValue((int)_graphicsScene->pixItem()->boundingRect().center().y());

    _algo = new AlgoSnake(_graphicsScene->pixItem()->scircle());
    _algo->setImage(new QImage(_graphicsScene->pixItem()->pixmap().toImage()));

    QObject::connect(_graphic.scaleSpinBox, SIGNAL(valueChanged(double)),
                     this, SLOT(scaleChanged(double)));
    QObject::connect(_graphic.angleSpinBox, SIGNAL(valueChanged(double)),
                this, SLOT(rotate(double)));
    QObject::connect(_graphicsScene, SIGNAL(rotateAngleChanged(double)),
                _graphic.angleSpinBox, SLOT(setValue(double)));
    QObject::connect(_graphicsScene, SIGNAL(positionChanged()),
                     this, SLOT(positionChanged()));
    QObject::connect(_graphic.StartButton, SIGNAL(clicked()),
                     this, SLOT(startAlgo()));
}

void Graphic::rotate(double angle){
    if(!_graphicsScene->rotCircle()->isMoved()){
        _graphicsScene->pixItem()->rotate2(angle);
        _graphicsScene->rotCircle()->setPosition(angle);
        _graphicsScene->update();
    }
}

void Graphic::scaleChanged(double scale){
    PixmapItem* pi = _graphicsScene->pixItem();
    pi->translate(pi->boundingRect().center().x(), pi->boundingRect().center().y());
    pi->scaled(scale);
    pi->translate(- pi->boundingRect().center().x(), - pi->boundingRect().center().y());
    _graphicsScene->update();
}

void Graphic::positionChanged(){
    _graphic.posXSpinBox->setValue((int)_graphicsScene->pixItem()->boundingRect().center().x());
    _graphic.posYSpinBox->setValue((int)_graphicsScene->pixItem()->boundingRect().center().y());
}

void Graphic::startAlgo(){
    _algo->edgesDetection();
}
