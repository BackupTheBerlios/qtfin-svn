#ifndef DRAWPREVIEW_H
#define DRAWPREVIEW_H

#include <QWidget>
#include <QPushButton>
#include "drawpreviewview.h"

class DrawPreview : public QWidget{
    Q_OBJECT

public:
    /**
     * constructor
     * create a Widget wich contains the view wich displays a preview of the Bézier curves extracted by the algoedgesextraction
     * @param parent : the parent of the widget
     * @param preview : the structure of the form to preview
     * @param width : the width of the scene
     * @param height : the height of the scene
     */
    DrawPreview(QWidget* parent, Data::ProjectFile* preview, qreal width, qreal height);

    /**
     * setter
     * change the structure to preview
     * @param preview : the structure of the form to preview
     * @param width : the width of the scene
     * @param height : the height of the scene
     */
    void setProjectFile(Data::ProjectFile *monofin, qreal width = 0, qreal height = 0);

protected:
    DrawPreviewView* _view; // the view wich displays the scene

    Data::ProjectFile* _preview; //the structure to preview
    qreal _width; //the width of the scene
    qreal _height; //the height of the scene

    QPushButton* _keepButton; // button attach to the slot keep
    QPushButton* _doNotKeepButton;// button attach to the slot do not keep

    /**
     * see the Qt Reference Documentation
     */
    virtual void closeEvent(QCloseEvent*);

public slots:

    /**
     * the structure wants to be kept
     */
    void keep();

    /**
     * the structure does not want to be kept
     */
    void doNotKeep();

signals:
    /**
     * signal recovered by the widget parent
     * the structure is kept
     */
    void kept();

    /**
     * signal recovered by the widget parent
     * the structure is not kept
     */
    void doNotKept();


};

#endif // DRAWPREVIEW_H
