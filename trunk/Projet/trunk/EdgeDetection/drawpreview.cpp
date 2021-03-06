#include "drawpreview.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPainter>
#include <QPainterPath>

using namespace Data;

DrawPreview::DrawPreview(QWidget* parent, ProjectFile* preview, qreal width, qreal height) :
        QWidget(parent, Qt::Window), _preview(preview), _width(width), _height(height)
{
    this->setWindowModality(Qt::WindowModal);
    this->setObjectName("Preview Form");
    this->resize(1024, 768);

    _view = new DrawPreviewView(this, preview, width, height);

    _keepButton = new QPushButton(this);
    _keepButton->setIcon(QIcon(":/icons/snake/snakeOK.png"));
    _keepButton->setIconSize(QSize(50, 50));
    _keepButton->setToolTip(tr("Keep the form"));
    _doNotKeepButton = new QPushButton(this);
    _doNotKeepButton->setIcon(QIcon(":/icons/snake/snakeNotOK.png"));
    _doNotKeepButton->setIconSize(QSize(50, 50));
    _doNotKeepButton->setToolTip(tr("Do not keep the form"));
    _doNotKeepButton->setDefault(true);
    QSpacerItem* spacerH1 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem* spacerH2 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QVBoxLayout* layoutV = new QVBoxLayout(this);
    QHBoxLayout* layoutH= new QHBoxLayout(this);
    layoutH->addSpacerItem(spacerH1);
    layoutH->addWidget(_keepButton);
    layoutH->addWidget(_doNotKeepButton);
    layoutH->addSpacerItem(spacerH2);
    layoutV->addWidget(_view);
    layoutV->addLayout(layoutH);
    this->setLayout(layoutV);

    QObject::connect(_keepButton, SIGNAL(clicked()), this, SLOT(keep()));
    QObject::connect(_doNotKeepButton, SIGNAL(clicked()), this, SLOT(doNotKeep()));
}

void DrawPreview::setProjectFile(ProjectFile *monofin, qreal width, qreal height){
    _preview = monofin;
    _width = width;
    _height = height;
    _view->setProjectFile(_preview, _width, _height);
}

void DrawPreview::closeEvent(QCloseEvent* event){
    _preview->undo(Data::MonofinSurface);
    emit this->doNotKept();
}

void DrawPreview::keep(){
    emit this->kept();
    this->hide();
}

void DrawPreview::doNotKeep(){
    _preview->undo(Data::MonofinSurface);
    emit this->doNotKept();
    this->close();
}
