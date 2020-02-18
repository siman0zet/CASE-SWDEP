#ifndef CTABLEITEM_H
#define CTABLEITEM_H

#include <QGraphicsPolygonItem>
#include "cobjectitem.h"

class CRelationshipItem;
class CTable;

class CTableItem : public CObjectItem
{
public:
    enum { Type = UserType + 1 };

    explicit CTableItem(CTable *table);
    ~CTableItem();

    int type() const override;

    CTable *table() const;
    int width() const;
    void setWidth(int width);
    int height() const;
    void setHeight(int height);
    QPolygonF polygon() const;

    void removeRelationship(CRelationshipItem *relationship);
    void removeRelationships();
    void addRelationship(CRelationshipItem *relationship);
    void setColor(const QColor &color);
    void setSelectedForRelation(bool selectedForRelation);

    QList<CRelationshipItem *> relationships() const;

protected:
    // QGraphicsItem interface
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    CTable *_table;

    int   _width;
    int   _height;

    bool _selectedForRelation;
    QColor _color;
    QPolygonF _polygon;
    QList<CRelationshipItem *> _relationships;
};

#endif // CTABLEITEM_H
