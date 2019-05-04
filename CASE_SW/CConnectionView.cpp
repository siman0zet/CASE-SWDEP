#include "CConnectionView.h"

#include "CViewModel.h"

CConnectionView::CConnectionView(CConnection *_connection, CViewModel *_parent): parent(_parent)
{
    connection = _connection;
    only_square_lines = true;

    update_state();
}

CConnectionView::~CConnectionView()
{

}

int CConnectionView::isPointInside(QPoint point)
{
    int left_point = -1;
    for (int i = 0; i < point_vec.size() - 1; ++i)
    {
        if (isPointOnLine(point_vec.at(i), point_vec.at(i + 1), point))
        {
            left_point = i;
            break;
        }
    }
    return left_point;
}

void CConnectionView::addPoint(QPoint point, int left_point_id)
{
    if (left_point_id < 0)
        left_point_id = -1;

    point_vec.insert(left_point_id + 1, point);

    is_point_mandatory.insert(left_point_id + 1, true);

    if (only_square_lines)
    {
        x_intersection_proc();
        square();
    }
}

void CConnectionView::reset()
{
    point_vec.clear();
    is_point_mandatory.clear();
}

void CConnectionView::shiftPoint(QPoint point, int point_id)
{
    if (point_id < 0 || point_id >= point_vec.size())
        return;

    point_vec[point_id] = point;
    is_point_mandatory[point_id] = true;

    if (only_square_lines)
    {

        x_intersection_proc();
        square();
    }
}

int CConnectionView::getPointsCntr()
{
    return point_vec.size();
}

void CConnectionView::setLeftEndStyle(CConnectionView::STYLE style)
{

}

void CConnectionView::setRightEndStyle(CConnectionView::STYLE style)
{

}

CConnection *CConnectionView::getConnection()
{
    return connection;
}

int CConnectionView::getId() const
{
    return connection->getId();
}

bool CConnectionView::isOnlySquare()
{
    return only_square_lines;
}


void CConnectionView::setOnlySquare(bool _flag)
{
    only_square_lines = _flag;
}

void CConnectionView::update_state()
{
    if (connection->getLeftMaxType() == CConnection::ONE)
        left_end_style = LINE;
    if (connection->getLeftMaxType() == CConnection::MANY)
        left_end_style = TRIPEL_LINE;

    if (connection->getRightMaxType() == CConnection::ONE)
        right_end_style = LINE;
    if (connection->getRightMaxType() == CConnection::MANY)
        right_end_style = TRIPEL_LINE;

    if (only_square_lines)
    {
        x_intersection_proc();
        square();
    }
}

void CConnectionView::x_intersection_proc()
{
    for (int i = 1; i < point_vec.size() - 1;)
    {
        if (is_point_mandatory.at(i))
        {
            point_vec.removeAt(i);
            is_point_mandatory.removeAt(i);
        }
        else
            ++i;
    }

    if (point_vec.size() < 2)
        return;

    int intersected = parent->is_entityes_x_intersected(getConnection()->get_left_entity_id(),
                                                        getConnection()->get_right_entity_id());

    if (!intersected)
        return;

    point_vec.insert(1, QPoint(point_vec.at(0).x() + intersected, point_vec.at(0).y()));
    is_point_mandatory.insert(1, true);
}

void CConnectionView::square()
{
    for (int i = 0; i < point_vec.size();)
    {
        if (!is_point_mandatory.at(i))
        {
            point_vec.removeAt(i);
            is_point_mandatory.removeAt(i);
        }
        else
            ++i;
    }

    bool horizontal_shift = false;
    for (int i = 0; i < point_vec.size() - 1; ++i)
    {
        bool is_square = false;
        if (point_vec.at(i).x() == point_vec.at(i + 1).x())
        {
            is_square = true;
            horizontal_shift = false;
        }
        else
        if (point_vec.at(i).y() == point_vec.at(i + 1).y())
        {
            is_square = true;
            horizontal_shift = true;
        }

        if (!is_square)
        {
            if (horizontal_shift)
            {
                QPoint p(point_vec.at(i).x(), point_vec.at(i + 1).y());
                point_vec.insert(i + 1, p);
                is_point_mandatory.insert(i + 1, false);
            }
            else
            {
                if (i + 1 != point_vec.size() - 1)
                {
                    QPoint p(point_vec.at(i + 1).x(), point_vec.at(i).y());
                    point_vec.insert(i + 1, p);
                    is_point_mandatory.insert(i + 1, false);
                }
                else
                {
                    QPoint p1, p2;

                    p1.setX((point_vec.at(i).x() + point_vec.at(i + 1).x())/2);
                    p1.setY(point_vec.at(i).y());

                    p2.setX(p1.x());
                    p2.setY(point_vec.at(i + 1).y());

                    point_vec.insert(i + 1, p1);
                    point_vec.insert(i + 2, p2);

                    is_point_mandatory.insert(i + 1, false);
                    is_point_mandatory.insert(i + 2, false);
                }
            }
        }
    }
    /*
    if (point_vec.size() > 2)
    {
        while (point_vec.size() > 2)
            point_vec.removeAt(1);

        if (point_vec.at(0).y() == point_vec.last().y())
            return;
    }

    if (point_vec.size() == 2)
    {
        QPoint p1, p2;

        p1.setX((point_vec.at(0).x() + point_vec.at(1).x())/2);
        p1.setY(point_vec.at(0).y());

        p2.setX(p1.x());
        p2.setY(point_vec.at(1).y());

        point_vec.insert(1, p1);
        point_vec.insert(2, p2);
    }*/
}

bool CConnectionView::isPointOnLine(QPoint p1, QPoint p2, QPoint point)
{
    int max_x, max_y, min_x, min_y;

    if (p1.x() > p2.x())
    {
        max_x = p1.x();
        min_x = p2.x();
    }
    else
    {
        max_x = p2.x();
        min_x = p1.x();
    }

    if (p1.y() > p2.y())
    {
        max_y = p1.y();
        min_y = p2.y();
    }
    else
    {
        max_y = p2.y();
        min_y = p1.y();
    }

    if (max_x - min_x < 5)
    {
        max_x += 2;
        min_x -= 2;
    }

    if (max_y - min_y < 5)
    {
        max_y += 2;
        min_y -= 2;
    }

    if (point.x() > max_x || point.x() < min_x)
        return false;

    if (point.y() > max_y || point.y() < min_y)
        return false;

    float k, b, c;
    k = p2.y() - p1.y();
    b = p1.y()*p2.x() - p1.x()*p2.y();
    c = p2.x() - p1.x();

    float d = k*point.x() - c*point.y() + b;
    d = fabs(d);
    d = d/sqrt(k*k + c*c);

    if (d < 5)
        return true;
    else
        return false;
}

QVector<QPoint> *CConnectionView::get_points()
{
    return &point_vec;
}

