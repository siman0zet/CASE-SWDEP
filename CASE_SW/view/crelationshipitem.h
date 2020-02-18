#ifndef CRELATIONSHIPITEM_H
#define CRELATIONSHIPITEM_H

#include <QGraphicsLineItem>
#include "cobjectitem.h"

class CTableItem;
class CRelationship;

class CRelationshipItem : public CObjectItem
{
public:
    enum { Type = UserType + 2 };

    CRelationshipItem(CTableItem *startItem, CTableItem *endItem, CRelationship *relationship);
    ~CRelationshipItem();

    int type() const override;

    CRelationship *relationship() const;

    CTableItem *startItem() const;
    void setStartItem(CTableItem *startItem);
    CTableItem *endItem() const;
    void setEndItem(CTableItem *endItem);

    void updatePosition();
    void updatePolygons();

protected:
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
    CRelationship *_relationship;

    CTableItem *_startItem;
    CTableItem *_endItem;

    QLineF _line;

    QPointF _startPoint;
    QPointF _endPoint;

    QPointF _midPoint1;
    QPointF _midPoint2;

    QPolygonF _startMaxPolygon;
    QPolygonF _endMaxPolygon;

    QPointF _startMinCenterPoint;
    QPointF _endMinCenterPoint;

    QPointF findIntersectionPoint(const QLineF &line, const CTableItem *item) const;
    int findIntersectionSide(const QLineF &line, const CTableItem *item) const;
    QPolygonF createManyPolygon(const QLineF &line, const CTableItem *item, qreal delta = 6) const;
    QPolygonF createAggregatePolygon(const QLineF &line, const CTableItem *item, qreal delta = 6) const;
    QPolygonF createArrowPolygon(const QLineF &line, const CTableItem *item, qreal delta = 6) const;
    QPointF findMinCenterPoint(const QLineF &line, const CTableItem *item, qreal delta = 6) const;
};

#endif // CRELATIONSHIPITEM_H
