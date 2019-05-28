#include "CRelationshipItem.h"
#include "CEntityItem.h"

#include <QPainter>
#include <qmath.h>
#include <QDebug>
#include <QStyleOptionGraphicsItem>

CRelationshipItem::CRelationshipItem(CEntityItem *startItem, CEntityItem *endItem) :
    _startItem(startItem),
    _endItem(endItem)
{
    setFlags(ItemIsSelectable |
             ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    setZValue(-1000.0);

    _startItem->addRelationship(this);
    _endItem->addRelationship(this);
    updatePosition();
    updatePolygons();
}

int CRelationshipItem::type() const
{
    return Type;
}

QRectF CRelationshipItem::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                    line().p2().y() - line().p1().y()))
            .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

QPainterPath CRelationshipItem::shape() const
{
    QPainterPath path;
    path.addPolygon(_startPolygon);
    path.addPolygon(_endPolygon);
    path.addRect(QRectF(QPointF(_startPoint.x(), _startPoint.y()),
                        QPointF(_midPoint1.x(), _midPoint1.y())));
    path.addRect(QRectF(QPointF(_midPoint1.x() - 1, _midPoint1.y() - 1),
                        QPointF(_midPoint2.x() + 1, _midPoint2.y() + 1)));
    path.addRect(QRectF(QPointF(_midPoint2.x() - 1, _midPoint2.y() - 1),
                        QPointF(_endPoint.x() + 1, _endPoint.y() + 1)));
    return path;
}

CEntityItem *CRelationshipItem::startItem() const
{
    return _startItem;
}

CEntityItem *CRelationshipItem::endItem() const
{
    return _endItem;
}

void CRelationshipItem::updatePosition()
{
    if (_startItem->collidesWithItem(_endItem))
        return;

    prepareGeometryChange();

    // find start and end points
    QLineF line(mapFromItem(_startItem, _startItem->width() / 2, _startItem->height() / 2),
                mapFromItem(_endItem, _endItem->width() / 2, _endItem->height() / 2));
    setLine(line);
    _startPoint = findIntersectionPoint(line, _startItem);
    _endPoint = findIntersectionPoint(line, _endItem);

    // find middle line points
    QRectF rect(_startPoint, _endPoint);
    if(abs(rect.width()) > abs(rect.height()))
    {
        // from top to bottom
        _midPoint1 = QPointF(rect.topLeft().x() + rect.width() / 2, rect.topLeft().y());
        _midPoint2 = QPointF(rect.topLeft().x() + rect.width() / 2, rect.bottomRight().y());
    }
    else
    {
        // from left to right
        _midPoint1 = QPointF(rect.topLeft().x(), rect.topLeft().y() + rect.height() / 2);
        _midPoint2 = QPointF(rect.bottomRight().x(), rect.topLeft().y() + rect.height() / 2);
    }
}

void CRelationshipItem::updatePolygons()
{
    prepareGeometryChange();
//    _startPolygon = createAggregatePolygon(this->line(), _startItem);
//    _endPolygon = createManyPolygon(this->line(), _endItem);
}

void CRelationshipItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    if (_startItem->collidesWithItem(_endItem))
        return;

    prepareGeometryChange();

    // colored outline if selected or mouseovered
    if((option->state & QStyle::State_Selected) ||
        (option->state & QStyle::State_MouseOver))
    {
        QColor penColor;
        if(option->state & QStyle::State_Selected)
            penColor.setRgb(61, 141, 253);
        if(option->state & QStyle::State_MouseOver)
            penColor.setRgb(206, 70, 72);
        painter->setPen(QPen(penColor.light(), 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawLine(_startPoint, _midPoint1);
        painter->drawLine(_midPoint1, _midPoint2);
        painter->drawLine(_midPoint2, _endPoint);
    }

    // main line
    painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setBrush(Qt::NoBrush);
    painter->drawLine(_startPoint, _midPoint1);
    painter->drawLine(_midPoint1, _midPoint2);
    painter->drawLine(_midPoint2, _endPoint);

    // endline polygons
    painter->drawPolygon(_startPolygon);
    painter->drawPolygon(_endPolygon);
}

QPointF CRelationshipItem::findIntersectionPoint(const QLineF &line, const QGraphicsPolygonItem *item)
{
    QLineF centerLine(line);
    QPolygonF polygon = item->polygon();
    QPointF p1 = polygon.first() + item->pos();
    QPointF p2;    
    QPointF intersectPoint;
    QLineF polyLine;
    for (int i = 1; i < polygon.count(); ++i) {
        p2 = polygon.at(i) + item->pos();
        polyLine = QLineF(p1, p2);
        QLineF::IntersectType intersectType =
            polyLine.intersect(centerLine, &intersectPoint);
        if (intersectType == QLineF::BoundedIntersection)
            break;
        p1 = p2;
    }
    return intersectPoint;
}

int CRelationshipItem::findIntersectionSide(const QLineF &line, const QGraphicsPolygonItem *item)
{
    QLineF centerLine(line);
    QPolygonF polygon = item->polygon();
    QPointF p1 = polygon.first() + item->pos();
    QPointF p2;
    QPointF intersectPoint;
    QLineF polyLine;
    for (int i = 1; i < polygon.count(); ++i) {
        p2 = polygon.at(i) + item->pos();
        polyLine = QLineF(p1, p2);
        QLineF::IntersectType intersectType =
            polyLine.intersect(centerLine, &intersectPoint);
        if (intersectType == QLineF::BoundedIntersection)
            return i;
        p1 = p2;
    }
    return -1;
}

QPolygonF CRelationshipItem::createManyPolygon(const QLineF &line, const QGraphicsPolygonItem *item)
{
    QPolygonF polygon;
    qreal d = 6;
    QPointF p1, p2, p3;

    QPointF intersectionPoint = findIntersectionPoint(line, item);
    int intersectionSide = findIntersectionSide(line, item);

    switch (intersectionSide) {
    case 1:
        //top
        p1 = QPointF(intersectionPoint.x(), intersectionPoint.y() - 3 * d);
        p2 = QPointF(intersectionPoint.x() + d, intersectionPoint.y());
        p3 = QPointF(intersectionPoint.x() - d, intersectionPoint.y());
        break;
    case 2:
        //right
        p1 = QPointF(intersectionPoint.x() + 3 * d, intersectionPoint.y());
        p2 = QPointF(intersectionPoint.x(), intersectionPoint.y() + d);
        p3 = QPointF(intersectionPoint.x(), intersectionPoint.y() - d);
        break;
    case 3:
        //bot
        p1 = QPointF(intersectionPoint.x(), intersectionPoint.y() + 3 * d);
        p2 = QPointF(intersectionPoint.x() - d, intersectionPoint.y());
        p3 = QPointF(intersectionPoint.x() + d, intersectionPoint.y());
        break;
    case 4:
        //left
        p1 = QPointF(intersectionPoint.x() - 3 * d, intersectionPoint.y());
        p2 = QPointF(intersectionPoint.x(), intersectionPoint.y() - d);
        p3 = QPointF(intersectionPoint.x(), intersectionPoint.y() + d);
        break;
    default:
        break;
    }
    polygon << p1 << p2 << p3;
    return polygon;
}

QPolygonF CRelationshipItem::createAggregatePolygon(const QLineF &line, const QGraphicsPolygonItem *item)
{
    QPolygonF polygon;
    qreal d = 6;
    QPointF p1, p2, p3, l1, l2;

    QPointF intersectionPoint = findIntersectionPoint(line, item);
    int intersectionSide = findIntersectionSide(line, item);

    switch (intersectionSide) {
    case 1:
        //top
        p1 = QPointF(intersectionPoint.x(), intersectionPoint.y() - 3 * d);
        p2 = QPointF(intersectionPoint.x() - d, intersectionPoint.y() - d);
        p3 = QPointF(intersectionPoint.x() + d, intersectionPoint.y() - d);
        l1 = QPointF(intersectionPoint.x() - d, intersectionPoint.y());
        l2 = QPointF(intersectionPoint.x() + d, intersectionPoint.y());
        break;
    case 2:
        //right
        p1 = QPointF(intersectionPoint.x() + 3 * d, intersectionPoint.y());
        p2 = QPointF(intersectionPoint.x() + d, intersectionPoint.y() - d);
        p3 = QPointF(intersectionPoint.x() + d, intersectionPoint.y() + d);
        l1 = QPointF(intersectionPoint.x(), intersectionPoint.y() - d);
        l2 = QPointF(intersectionPoint.x(), intersectionPoint.y() + d);
        break;
    case 3:
        //bot
        p1 = QPointF(intersectionPoint.x(), intersectionPoint.y() + 3 * d);
        p2 = QPointF(intersectionPoint.x() + d, intersectionPoint.y() + d);
        p3 = QPointF(intersectionPoint.x() - d, intersectionPoint.y() + d);
        l1 = QPointF(intersectionPoint.x() + d, intersectionPoint.y());
        l2 = QPointF(intersectionPoint.x() - d, intersectionPoint.y());
        break;
    case 4:
        //left
        p1 = QPointF(intersectionPoint.x() - 3 * d, intersectionPoint.y());
        p2 = QPointF(intersectionPoint.x() - d, intersectionPoint.y() + d);
        p3 = QPointF(intersectionPoint.x() - d, intersectionPoint.y() - d);
        l1 = QPointF(intersectionPoint.x(), intersectionPoint.y() + d);
        l2 = QPointF(intersectionPoint.x(), intersectionPoint.y() - d);
        break;
    default:
        break;
    }
    polygon << p2 << p3 << p1 << p2 << l1 << l2 << p3;
    return polygon;
}
