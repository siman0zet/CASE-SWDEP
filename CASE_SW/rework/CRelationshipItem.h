#ifndef CRELATIONSHIPITEM_H
#define CRELATIONSHIPITEM_H

#include <QGraphicsLineItem>

class CTableItem;
class CRelationship;

class CRelationshipItem : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 2 };

    CRelationshipItem(CTableItem *startItem, CTableItem *endItem, CRelationship *relationship);

    int type() const override;
    int id();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    CTableItem *startItem() const;
    CTableItem *endItem() const;

    void updatePosition();
    void updatePolygons();

    CRelationship *relationship() const;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
    CTableItem *_startItem;
    CTableItem *_endItem;
    CRelationship *_relationship;

    QPointF _startPoint;
    QPointF _endPoint;

    QPointF _midPoint1;
    QPointF _midPoint2;

    QPolygonF _startPolygon;
    QPolygonF _endPolygon;

    QPointF findIntersectionPoint(const QLineF &line, const QGraphicsPolygonItem *item);
    int findIntersectionSide(const QLineF &line, const QGraphicsPolygonItem *item);
    QPolygonF createManyPolygon(const QLineF &line, const QGraphicsPolygonItem *item);
    QPolygonF createAggregatePolygon(const QLineF &line, const QGraphicsPolygonItem *item);
//    QPolygonF createPhysicalPolygon(const QLineF &line, const QGraphicsPolygonItem *item);
};

#endif // CRELATIONSHIPITEM_H
