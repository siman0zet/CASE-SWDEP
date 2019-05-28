#ifndef CTABLEITEM_H
#define CTABLEITEM_H

#include <QGraphicsPolygonItem>

class CRelationshipItem;
class CTable;

class CTableItem : public QGraphicsPolygonItem
{
public:
    enum { Type = UserType + 1 };

    explicit CTableItem(CTable *table);

    int width() const;
    int height() const;
    QPolygonF polygon() const;

    void removeRelationship(CRelationshipItem *relationship);
    void removeRelationships();
    void addRelationship(CRelationshipItem *relationship);
    void setColor(const QColor &color);
    void setSelectedForRelation(bool selectedForRelation);

    int type() const override;
    int id();
    QString name();
    CTable *table() const;

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
