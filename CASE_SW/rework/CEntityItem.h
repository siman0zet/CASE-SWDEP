#ifndef CENTITYITEM_H
#define CENTITYITEM_H

#include <QGraphicsPolygonItem>

class CRelationshipItem;

class CEntityItem : public QGraphicsPolygonItem
{
public:
    enum { Type = UserType + 1 };

    CEntityItem();

    int width() const;
    int height() const;
    QPolygonF polygon() const;

    void removeRelationship(CRelationshipItem *relationship);
    void removeRelationships();
    void addRelationship(CRelationshipItem *relationship);
    void setColor(const QColor &color);

    int type() const override;

    // QGraphicsItem interface
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;



protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    int   _width;
    int   _height;

    QColor _color;
    QPolygonF _polygon;
    QList<CRelationshipItem *> _relationships;

};

#endif // CENTITYITEM_H
