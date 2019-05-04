#ifndef CENTITYVIEW_H
#define CENTITYVIEW_H

#include <QWidget>
#include <QBoxLayout>
#include <QStringList>
#include <QAbstractItemModel>
#include <QStringListModel>

#include <QPainter>
#include <QMouseEvent>

#include "CEntity.h"

/* Представление сущности (объекта CEntity). Кратко отображает информацию о сущности. */

class CViewModel;

class CEntityView : public QWidget
{
    Q_OBJECT
public:
    CEntityView(CEntity* _entity, CViewModel* _parent);
    ~CEntityView();

    int getId() const;

    CEntity *getEntity();

    void mousePressEvent(QMouseEvent * me);
    void mouseDoubleClickEvent(QMouseEvent* me);
    void mouseMoveEvent(QMouseEvent* me);
    void mouseReleaseEvent(QMouseEvent* me);

    void set_selected(bool falg);
    bool selected();

    bool selectable;

    bool unselect_on_mouse_release, moved;

public slots:

    void update_state(); // Обновляет свои параметры

protected:
    void paintEvent(QPaintEvent* p);

private:
    void draw_etc_sign(QPoint point, QPainter &painter);
    void draw_entity_text(QPoint point, QString text, QPainter &painter, bool pk = false,
                          bool nn = false, bool uq = false, int font_size = -1);

    QBoxLayout box_layout;

    QStringListModel *list_model;

    CEntity* entity;

    CViewModel* parent;

    QPoint prev_mouse_point;

    bool left_button, right_button;

    bool is_selected;
};

#endif // CENTITYVIEW_H
