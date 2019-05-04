#include "CEntityView.h"

#include "CViewModel.h"

CEntityView::CEntityView(CEntity* _entity, CViewModel *_parent): entity(_entity), QWidget(_parent), box_layout(QBoxLayout::TopToBottom, this),
    parent(_parent)
{
    this->setLayout(&box_layout);
    this->setGeometry(0, 0, 100, 200);

    list_model = NULL;

    left_button = false;
    right_button = false;

    is_selected = false;

    selectable = true;

    unselect_on_mouse_release = false;
    moved = false;

    connect(entity, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(update_state()));

    update_state();
}

CEntityView::~CEntityView()
{
    delete list_model;
}

int CEntityView::getId() const
{
    return entity->getId();
}

CEntity *CEntityView::getEntity()
{
    return entity;
}

void CEntityView::mousePressEvent(QMouseEvent *me)
{
    moved = false;

    if (!selectable)
    {
        me->ignore();
        return;
    }

    if (me->button() == Qt::LeftButton)
    {
        prev_mouse_point = me->globalPos();

        left_button = true;

        if (!is_selected)
        {
            parent->select_entity(this);
        }
        else
        {
            if (parent->getSelectedEnts().size() > 1)
            {
                unselect_on_mouse_release = true;
            }
            else
                parent->unselect_entity(this);
        }

        me->accept();
    }
    if (me->button() == Qt::RightButton)
    {
        if (!is_selected)
        {
            parent->select_entity(this);
        }
        else
        {/*
            parent->unselect_entity(this);
            parent->select_entity(this);*/
        }
        me->ignore();
    }
}

void CEntityView::mouseDoubleClickEvent(QMouseEvent *me)
{
    if (!selectable)
    {
        me->ignore();
        return;
    }

    if (!is_selected)
    {
        parent->select_entity(this);
    }
    me->ignore();
}

void CEntityView::mouseMoveEvent(QMouseEvent *me)
{
    if (!selectable)
    {
        me->ignore();
        return;
    }

    if (left_button)
    {
        moved = true;
        parent->move_entity(this->pos() + me->globalPos() - prev_mouse_point, this->getId());
        prev_mouse_point = me->globalPos();
    }
}

void CEntityView::mouseReleaseEvent(QMouseEvent *me)
{
    if (!selectable)
    {
        me->ignore();
        return;
    }

    if (me->button() == Qt::LeftButton)
    {
        left_button = false;

        if (unselect_on_mouse_release)
        {
            if (!moved && parent->isMultipySelEnabled())
                parent->unselect_entity(this);

            if (!moved && !parent->isMultipySelEnabled())
            {
                parent->unselect_entity(this);
                parent->select_entity(this);
            }

            unselect_on_mouse_release = false;
        }
    }
}

void CEntityView::update_state()
{
    update();
}

void CEntityView::set_selected(bool falg)
{
    is_selected = falg;
    update();
}

bool CEntityView::selected()
{
    return is_selected;
}

void CEntityView::paintEvent(QPaintEvent *p)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);
    if (!is_selected)
        painter.setPen(QPen(Qt::black, 1));
    else
        painter.setPen(QPen(Qt::yellow, 1));
    painter.setBrush(QBrush(Qt::lightGray));
    painter.setBackground(QBrush(QColor(10, 200, 10)));

    painter.drawRoundedRect(QRect(5, 5, this->geometry().width() - 10, this->geometry().height() - 10), 10, 10);

    draw_entity_text(QPoint(12, 18), entity->getName(), painter);

    painter.drawLine(QPoint(8, 35), QPoint(this->geometry().width() - 8, 35));

    QPoint curr_point(12, 35);
    for (int i = 0; i < entity->getRowCount(); ++i)
    {
        if (curr_point.y() + 25 <= geometry().height())
        {
            draw_entity_text(curr_point, entity->getRow(i)->getName(), painter, entity->getRow(i)->isPrimaryKey(),
                             entity->getRow(i)->isNotNull(), entity->getRow(i)->isUnique());
            curr_point.setY(curr_point.y() + 15);
        }
        else
        {
            QPoint p = curr_point;

            p.setX(this->width()/2 - 12);
            p.setY(this->height() - 18);

            draw_etc_sign(p, painter);
            break;
        }
    }

    for (int i = 0; i < entity->getForeignRowCount(); ++i)
    {
        if (curr_point.y() + 25 <= geometry().height())
        {
            draw_entity_text(curr_point, entity->getForeignRow(i)->getName(), painter, entity->getForeignRow(i)->isPrimaryKey(),
                             entity->getForeignRow(i)->isNotNull(), entity->getForeignRow(i)->isUnique());
            curr_point.setY(curr_point.y() + 15);
        }
        else
        {
            QPoint p = curr_point;

            p.setX(this->width()/2 - 12);
            p.setY(this->height() - 18);

            draw_etc_sign(p, painter);
            break;
        }
    }
}

void CEntityView::draw_etc_sign(QPoint point, QPainter &painter)
{
    painter.setPen(QPen(Qt::white, 1));

    point.setY(point.y() + 3);

    painter.drawLine(QPoint(8, point.y()), QPoint(this->width() - 8, point.y()));

    point.setY(point.y() + 5);

    painter.drawEllipse(point, 2, 2);

    point.setX(point.x() + 12);

    painter.drawEllipse(point, 2, 2);

    point.setX(point.x() + 12);

    painter.drawEllipse(point, 2, 2);
}

void CEntityView::draw_entity_text(QPoint point, QString text, QPainter &painter, bool pk, bool nn, bool uq, int font_size)
{
    bool is_etc = false;
    if (font_size == -1)
    {
        painter.setPen(QPen(Qt::white, 1));
        painter.setFont(QFont("Times New Roman", 10));
        if (text.size() > 8)
            painter.setFont(QFont("Times New Roman", 9));
        if (text.size() > 9)
            painter.setFont(QFont("Times New Roman", 8));
        if (text.size() > 10)
            painter.setFont(QFont("Times New Roman", 7));
        if (text.size() > 10)
        {
            is_etc = true;
            text.remove(10, text.size() - 10);
            text.append("...");
        }
    }
    else
        painter.setFont(QFont("Times New Roman", font_size));

    painter.drawText(QRect(point.x(), point.y(), this->geometry().width() - point.x() - 12, 15), text);
}

