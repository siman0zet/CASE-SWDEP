#ifndef CCONNECTIONVIEW_H
#define CCONNECTIONVIEW_H

#include <QPoint>
#include <QVector>

#include <math.h>

#include "CDataModel.h"

/* Объект, для отображения связей. */

class CViewModel;

class CConnectionView
{
    friend class CViewModel;
public:
    enum STYLE {LINE, LINE_ARROW, TRIPEL_LINE}; /* Тип конца линии */

    CConnectionView(CConnection* _connection, CViewModel* _parent);
    ~CConnectionView();

    int isPointInside(QPoint point); /* Возвращает id ближайшей точки - если точка на линии. */

    void addPoint(QPoint point, int left_point_id); // Добавляет точку на линию
    void remPoint(int point_id); // Удаляет точку
    void reset(); // Удаляет все точки
    void shiftPoint(QPoint point, int point_id); // Сдвигает точку с номером point_id на место point

    int getPointsCntr();

    void setLeftEndStyle(STYLE style); // Устанавливаеь тип концов линии
    void setRightEndStyle(STYLE style);

    CConnection* getConnection();
    int getId() const;

    bool isOnlySquare(); // Возвращает параметр only_square_lines
    void setOnlySquare(bool _flag); /* Устанавливает only_square_lines параметр, если only_square_lines - true,
                                            то все отрезки должны быть под углом 90 градусов к смежным им отрезкам. */

    void update_state(); // Обновляет свои параметры

private:

    void x_intersection_proc();
    void square();

    bool isPointOnLine(QPoint p1, QPoint p2, QPoint point);

    QVector<QPoint>* get_points(); // Возвращает указатель на вектор точек

    QVector<QPoint> point_vec;

    QVector<bool> is_point_mandatory;

    CConnection* connection;

    STYLE left_end_style, right_end_style;

    bool only_square_lines;

    CViewModel *parent;
};

#endif // CCONNECTIONVIEW_H
