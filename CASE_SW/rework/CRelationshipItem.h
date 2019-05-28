#ifndef CRELATIONSHIPITEM_H
#define CRELATIONSHIPITEM_H

#include <QGraphicsLineItem>

class CEntityItem;

class CRelationshipItem : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 2 };

    CRelationshipItem(CEntityItem *startItem, CEntityItem *endItem);

    int type() const override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    CEntityItem *startItem() const;
    CEntityItem *endItem() const;

    void updatePosition();
    void updatePolygons();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:    
    CEntityItem *_startItem;
    CEntityItem *_endItem;

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
