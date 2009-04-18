#ifndef TANGENT_H
#define TANGENT_H

#include <QWidget>
#include <QGraphicsItem>
#include <QLineF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QPen>

class PaintingScene;

class Tangent: public QGraphicsItem{

public:
    /**
    * Constructs a tangent by creating a new QLineF between the two given
    * points. The created line is the one used to be painted, so it has
    * to be modified when a bounding point or a control point moves.
    *@param p1 the coordinates of the left point of the tangent
    *@param p2 the coordinates of the right point of the tangent
    *@param scene a pointer to the main PaintingScene
    **/
    Tangent(QPointF p1, QPointF p2, PaintingScene* scene);

    /**
    * Destroys the line created in the constructor.
    **/
    ~Tangent();


    /**
    * Gives the bounding rectangle of the line, the top left point of which
    * is the left point of the line and the bottom right point the right
    * point of the line.
    *@return the bounding rectangle of the item
    **/
    virtual QRectF boundingRect() const;

    /**
    *@return a copy of the tangent as a QLineF
    **/
    QLineF line(){return *_line;}

    /**
    * Draws the tangent as a dash line.
    *@param painter the painter which will be used to draw the line
    *@param option not used here, see the documentation of QGraphicsItem
    *@param widget not used here, see the documentation of QGraphicsItem
    **/
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    /**
    * Modifies the two extremity points of the line.
    *@param p1 the new first point of the line
    *@param p2 the new second point of the line
    **/
    void setLine(QPointF p1, QPointF p2);

protected:
    QLineF* _line;
    PaintingScene* _scene;

};

#endif // TANGENT_H
