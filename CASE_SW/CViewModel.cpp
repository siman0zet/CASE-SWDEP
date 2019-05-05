#include "CViewModel.h"
#include "mainwindow.h"

CViewModel::CViewModel(QWidget *w, QApplication* _app): QWidget(w), app(_app)
{
    mw = NULL;

    rect_selection = clear_selection = false;

    parent = NULL;
    last_connection_id = -1;
    last_entity_id = -1;

    ScriptGen = NULL;

    data_model = new CDataModel(app);

    entity_context_menu = new QMenu(this);

    detailed_entity_view = entity_context_menu->addAction(tr("Detailed"));
    connect_entityes = entity_context_menu->addAction(tr("Add connection"));
    remove_entity = entity_context_menu->addAction(tr("Remove"));

    connect(detailed_entity_view, SIGNAL(triggered()), this, SLOT(show_entity_detail()));
    connect(remove_entity, SIGNAL(triggered()), this, SLOT(remove_selected_entityes()));
    connect(connect_entityes, SIGNAL(triggered()), this, SLOT(connect_selected_entityes()));

    multiply_selection = false;

    context_menu = new QMenu(this);

    add_new_entity = context_menu->addAction(tr("Create entity"));
    convert_to_query = context_menu->addAction(tr("To PDM"));

    connect(add_new_entity, SIGNAL(triggered()), this, SLOT(add_new_entity_event()));
    connect(convert_to_query, SIGNAL(triggered()), this, SLOT(convert_to_query_event()));

    selected_connection = NULL;

    connection_context_menu = new QMenu(this);

    rem_con = connection_context_menu->addAction(tr("Remove"));
    detail_con = connection_context_menu->addAction(tr("Detail"));

    connect(rem_con, SIGNAL(triggered(bool)), this, SLOT(remove_selected_connection()));
    connect(detail_con, SIGNAL(triggered(bool)), this, SLOT(show_connection_detail()));

    save_CDM = context_menu->addAction(tr("Save CDM"));
    save_CDM_as = context_menu->addAction(tr("Save CDM as"));
    load_CDM = context_menu->addAction(tr("Load CDM"));

    to_script = context_menu->addAction(tr("Convert to query"));

    connect(save_CDM, SIGNAL(triggered()), this, SLOT(save_CDM_event()));
    connect(save_CDM_as, SIGNAL(triggered()), this, SLOT(save_CDM_as_event()));
    connect(load_CDM, SIGNAL(triggered()), this, SLOT(load_CDM_event()));

    connect(to_script, SIGNAL(triggered()), this, SLOT(to_script_event()));

    currFileName = "new";
    this->setWindowTitle("CASE_SW - " + currFileName);
    w_opened = false;

    tmp_conn_entity = NULL;

    cursor_tool = true;
    create_entity_tool = false;
    remove_entity_tool = false;
    one_one_tool = false;
    one_many_tool = false;
    many_many_tool = false;
    agregate_tool = false;

    parent_area = NULL;
}

CViewModel::~CViewModel()
{
    if (parent)
        parent->setEnabled(true);
}

void CViewModel::resizeEvent(QResizeEvent *re)
{
    QSize max_size(0, 0);
    for (int i = 0; i < entity_vec.size(); ++i)
    {
        if (entity_vec.at(i)->x() + entity_vec.at(i)->width() + 60 > max_size.width())
            max_size.setWidth(entity_vec.at(i)->x() + entity_vec.at(i)->width() + 60);
        if (entity_vec.at(i)->y() + entity_vec.at(i)->height() + 60 > max_size.height())
            max_size.setHeight(entity_vec.at(i)->y() + entity_vec.at(i)->height() + 60);
    }

    bool width_changed = true, height_changed = true;

    if (re->size().width() > max_size.width())
    {
        max_size.setWidth(re->size().width());
        width_changed = false;
    }
    if (re->size().height() > max_size.height())
    {
        max_size.setHeight(re->size().height());
        height_changed = false;
    }

    if (width_changed || height_changed)
        this->resize(max_size);

    QWidget::resizeEvent(re);
}

CViewModel *CViewModel::PDMViewConstructor(CDataModel *model)
{
    CViewModel *view = new CViewModel(NULL, app);

    view->parent = this;
    delete view->data_model;

    view->data_model = model;

    int last_id = 0;

    for (int i = 0; i < this->entity_vec.size(); ++i)
    {
        bool is_exist = false;
        CEntity *en;

        for (int j = 0; j < model->entity_vec.size(); ++j)
        {
            if (this->entity_vec.at(i)->getId() == model->entity_vec.at(j)->getId())
            {
                en = model->entity_vec.at(j);
                is_exist = true;
                break;
            }
        }

        if (is_exist)
        {
            view->entity_vec.append(new CEntityView(en, view));
            view->last_entity_id = en->getId();
            last_id = en->getId();
            view->entity_vec.last()->move(this->entity_vec.at(i)->pos());
            view->entity_vec.last()->show();
        }
    }


    for (int i = 0; i < model->entity_vec.size(); ++i)
    {
        if (model->entity_vec.at(i)->getId() > last_id)
        {
            view->entity_vec.append(new CEntityView(model->entity_vec.at(i), view));
            view->last_entity_id = model->entity_vec.at(i)->getId();

            CEntityView *en1 = NULL, *en2 = NULL;
            for (int j = 0; j < model->connection_vec.size(); ++j)
            {
                if (model->connection_vec.at(j)->get_right_entity_id() == model->entity_vec.at(i)->getId())
                {
                    for (int k = 0; k < view->entity_vec.size(); ++k)
                    {
                        if (view->entity_vec.at(k)->getId() == model->connection_vec.at(j)->get_left_entity_id())
                        {
                            if (!en1)
                                en1 = view->entity_vec.at(k);
                            else
                                en2 = view->entity_vec.at(k);
                            break;
                        }
                    }
                }
                if (model->connection_vec.at(j)->get_left_entity_id() == model->entity_vec.at(i)->getId())
                {
                    for (int k = 0; k < view->entity_vec.size(); ++k)
                    {
                        if (view->entity_vec.at(k)->getId() == model->connection_vec.at(j)->get_right_entity_id())
                        {
                            if (!en1)
                                en1 = view->entity_vec.at(k);
                            else
                                en2 = view->entity_vec.at(k);
                            break;
                        }
                    }
                }
            }

            if (en1 && en2)
                view->entity_vec.last()->move((en1->pos() + en2->pos())/2);

            view->entity_vec.last()->show();
        }
    }

    for (int i = 0; i < model->connection_vec.size(); ++i)
    {
        CEntityView *en1 = NULL, *en2 = NULL;
        for (int j = 0; j < view->entity_vec.size(); ++j)
        {
            if (view->entity_vec.at(j)->getId() == model->connection_vec.at(i)->get_left_entity_id())
                en1 = view->entity_vec.at(j);
            if (view->entity_vec.at(j)->getId() == model->connection_vec.at(i)->get_right_entity_id())
                en2 = view->entity_vec.at(j);

            if (en1 && en2)
                break;
        }

        if (!en1 || !en2)
            continue;
        view->connection_vec.append(make_connection(en1, en2, model->connection_vec.at(i)));
        view->linesIntersectionCulc();
    }

    return view;
}

int CViewModel::addEntity()
{
    int id;
        if ((id = data_model->addEntity()) != -1)
        {
            entity_vec.append(new CEntityView(data_model->getEntity(id), this));
            last_entity_id = id;

            entity_vec.last()->show();

            entity_vec.last()->getEntity()->setName("entity_" + QString::number(id) + "'");
            return id;
        }

        return -1;
}

void CViewModel::remEntity(int id)
{
    for (int i = 0; i < entity_vec.size(); ++i)
    {
        if (entity_vec.at(i)->getId() == id)
        {
            if (check_entity(id))
            {
                delete entity_vec.at(i);
                data_model->remEntity(id);
                entity_vec.remove(i);
            }

            return;
        }
    }
}

void CViewModel::remEntityCascade(int id)
{

}

int CViewModel::addConnection(int entity1, int entity2)
{
    if (entity1 == entity2)
        return -1;

    for (int i = 0; i < connection_vec.size(); ++i)
    {
        CConnection *con = connection_vec.at(i)->getConnection();
        if (con->get_left_entity_id() == entity1 && con->get_right_entity_id() == entity2)
            return -1;
        if (con->get_left_entity_id() == entity2 && con->get_right_entity_id() == entity1)
            return -1;
    }

    CEntityView *en1 = NULL, *en2 = NULL;
    for (int i = 0; i < entity_vec.size(); ++i)
    {
        if (entity_vec.at(i)->getId() == entity1)
            en1 = entity_vec.at(i);
        if (entity_vec.at(i)->getId() == entity2)
            en2 = entity_vec.at(i);

        if (en1 && en2)
            break;
    }

    if (!en1 || !en2)
        return -1;

    int id = data_model->addConnection(entity1, entity2);
    connection_vec.append(make_connection(en1, en2, data_model->getConnection(id)));
    last_connection_id = id;
    linesIntersectionCulc();
    return id;
}

void CViewModel::remConnection(int id)
{
    for (int i = 0; i < connection_vec.size(); ++i)
    {
        if (connection_vec.at(i)->getId() == id)
        {
            if (check_connection(id))
            {
                delete connection_vec.takeAt(i);
                data_model->remConnection(id);
            }

            update();
            return;
        }
    }
}

void CViewModel::reconnect(int id, int entity1, int entity2)
{
    if (entity1 == entity2)
        return;

    CConnectionView* con = NULL;

    for (int i = 0; i < connection_vec.size(); ++i)
    {
        if (connection_vec.at(i)->getId() == id)
        {
            con = connection_vec.at(i);
            break;
        }
    }

    if (!con)
        return;

    data_model->reconnect(id, entity1, entity2);

    CEntityView *entity_view_1;
    CEntityView *entity_view_2;

    for (int i = 0; i < entity_vec.size(); ++i)
    {
        if (entity_vec.at(i)->getId() == entity1)
        {
            entity_view_1 = entity_vec.at(i);
            break;
        }
    }

    for (int i = 0; i < entity_vec.size(); ++i)
    {
        if (entity_vec.at(i)->getId() == entity2)
        {
            entity_view_2 = entity_vec.at(i);
            break;
        }
    }

    con->reset();

    con->addPoint(QPoint(entity_view_2->geometry().x() + entity_view_2->geometry().width()*0.5,
                                entity_view_2->geometry().y() + entity_view_2->geometry().height()*0.5), -1);
    con->addPoint(QPoint(entity_view_1->geometry().x() + entity_view_1->geometry().width()*0.5,
                                entity_view_1->geometry().y() + entity_view_1->geometry().height()*0.5), -1);
}

void CViewModel::addViewEntity(int id)
{

}

void CViewModel::addViewConnection(int id, int entity1, int entity2)
{
    int i = 0;
    while (i <= last_connection_id)
        if (connection_vec.at(i)->getId() == id)
            return;
}

void CViewModel::remViewEntity(int id)
{

}

void CViewModel::remViewConnection(int id)
{

}

bool CViewModel::check_entity(int id)
{
    for (int i = 0; i < connection_vec.size(); ++i)
    {
        if (connection_vec.at(i)->getConnection()->get_left_entity_id() == id ||
                connection_vec.at(i)->getConnection()->get_right_entity_id() == id)
            return false;
    }

    return true;
}

bool CViewModel::check_connection(int id)
{
    return true;
}

CConnectionView *CViewModel::make_connection(CEntityView *entity1, CEntityView *entity2, CConnection *conn)
{
    CConnectionView *connection = new CConnectionView(conn, this);

    connection->addPoint(QPoint(entity2->geometry().x() + entity2->geometry().width()*0.5,
                                entity2->geometry().y() + entity2->geometry().height()*0.5), -1);
    connection->addPoint(QPoint(entity1->geometry().x() + entity1->geometry().width()*0.5,
                                entity1->geometry().y() + entity1->geometry().height()*0.5), -1);

    return connection;
}

void CViewModel::move_entity(int id, QPoint point)
{
    CEntityView *entity = NULL;
    for (int i = 0; i < entity_vec.size(); ++i)
        if (entity_vec.at(i)->getId() == id)
        {
            entity = entity_vec.at(i);
            break;
        }

    if (entity)
    {
        entity->move(point);
    }
}

void CViewModel::mousePressEvent(QMouseEvent *me)
{
    if(cursor_tool)
    {
        if (me->button() == Qt::LeftButton)
        {
            if (!multiply_selection)
                clear_entity_selection();

            rect_selection = true;
            rect_point = rect_curr_point = me->pos();
        }
        if(me->button() == Qt::RightButton)
        {
            CEntityView *hovered = (CEntityView *)this->childAt(me->pos());
            selected_connection = NULL;
            for(int i = 0; i < connection_vec.size(); i++)
            {
                if(connection_vec.at(i)->isPointInside(me->pos()) != -1)
                {
                    selected_connection = connection_vec.at(i);
                    break;
                }
            }
            if(hovered)
            {
                if(selected_entityes.size() > 2)
                {
                    showContextMenu(me->pos(), true);
                    return;
                }
                if(selected_entityes.size() == 2)
                {
                    showTableRelationshipContextMenu(me->pos());
                    return;
                }

                QString selectedName = hovered->getEntity()->getName();
                showTableContextMenu(me->pos(), selectedName);
                return;
            }
            else
            {
                if(selected_connection != NULL)
                {
                    clear_entity_selection();
                    showRelationshipContextMenu(me->pos());
                    return;
                }
                else
                {
                    if(selected_entityes.size() == 2)
                    {
                        showTableRelationshipContextMenu(me->pos());
                        return;
                    }
                    if(selected_entityes.size() == 1)
                    {
                        QString selectedName = selected_entityes.last()->getEntity()->getName();
                        showTableContextMenu(me->pos(), selectedName);
                        return;
                    }
                    if(selected_entityes.size() == 0)
                    {
                        showContextMenu(me->pos(), false);
                        return;
                    }

                    showContextMenu(me->pos(), true);
                    return;
                }
            }
        }
    }
    if (create_entity_tool)
    {
        if (me->button() == Qt::LeftButton)
        {
            add_new_entity_event();
            clear_entity_selection();
        }
        else
            if (me->button() == Qt::RightButton)
            {
                QWidget* wid = this->childAt(me->pos());

                if (!wid)
                    clear_entity_selection();

                for (int i = 0; i < connection_vec.size(); ++i)
                {
                    if (connection_vec.at(i)->isPointInside(me->pos()) != -1)
                    {
                        selected_connection = connection_vec.at(i);
                        connection_context_menu_request(me->globalPos());
                        return;
                    }
                }

                if (!selected_entityes.size())
                {
                    context_menu_request(me->globalPos());
                }
                else
                {
                    entity_context_menu_request(me->globalPos());
                }
            }
    }
    if (remove_entity_tool)
    {
        if (me->button() == Qt::LeftButton)
        {
            for (int i = 0; i < connection_vec.size(); ++i)
            {
                if (connection_vec.at(i)->isPointInside(me->pos()) != -1)
                {
                    remConnection(connection_vec.at(i)->getId());
                    selected_connection = NULL;
                    return;
                }
            }

            QWidget* wid = this->childAt(me->pos());
            if (wid)
            {
                CEntityView *en = static_cast<CEntityView*> (wid);

                if (en)
                {
                    clear_entity_selection();

                    selected_entityes.append(en);

                    remove_selected_entityes();
                }
            }
        }
        else
        if (me->button() == Qt::RightButton)
        {
            QWidget* wid = this->childAt(me->pos());

            if (!wid)
                clear_entity_selection();

            for (int i = 0; i < connection_vec.size(); ++i)
            {
                if (connection_vec.at(i)->isPointInside(me->pos()) != -1)
                {
                    selected_connection = connection_vec.at(i);
                    connection_context_menu_request(me->globalPos());
                    return;
                }
            }

            if (!selected_entityes.size())
            {
                context_menu_request(me->globalPos());
            }
            else
            {
                entity_context_menu_request(me->globalPos());
            }
        }
    }
    if (one_one_tool || one_many_tool || many_many_tool || agregate_tool)
    {
        if (me->button() == Qt::LeftButton)
        {
            QWidget* wid = this->childAt(me->pos());
            if (wid)
            {
                CEntityView *en = static_cast<CEntityView*> (wid);

                if (en)
                {
                    tmp_conn_entity = en;
                    tmp_conn_point = en->pos();
                    this->update();
                    return;
                }
                else
                {
                    tmp_conn_entity = NULL;
                    return;
                }
            }
            else
            {
                tmp_conn_entity = NULL;
                return;
            }
        }
        else
        if (me->button() == Qt::RightButton)
        {
            tmp_conn_entity = NULL;
            QWidget* wid = this->childAt(me->pos());

            if (!wid)
                clear_entity_selection();

            for (int i = 0; i < connection_vec.size(); ++i)
            {
                if (connection_vec.at(i)->isPointInside(me->pos()) != -1)
                {
                    selected_connection = connection_vec.at(i);
                    connection_context_menu_request(me->globalPos());
                    return;
                }
            }

            if (!selected_entityes.size())
            {
                context_menu_request(me->globalPos());
            }
            else
            {
                entity_context_menu_request(me->globalPos());
            }
        }

    }
    me->accept();
}

void CViewModel::mouseDoubleClickEvent(QMouseEvent *me)
{
    if (cursor_tool)
    {
        if (selected_entityes.size())
        {
            show_entity_detail();
            return;
        }

        for (int i = 0; i < connection_vec.size(); ++i)
        {
            if (connection_vec.at(i)->isPointInside(me->pos()) != -1 && !data_model->is_PDM())
            {
                selected_connection = connection_vec.at(i);
                show_connection_detail();
                break;
            }
        }
    }
    if (one_one_tool || one_many_tool || many_many_tool || agregate_tool)
    {
        tmp_conn_entity = NULL;
        this->update();
    }
}

void CViewModel::mouseMoveEvent(QMouseEvent *me)
{
    if (rect_selection)
    {
        rect_curr_point = me->pos();

        for (int i = 0; i < rect_selected.size(); ++i)
        {
            rect_selected.at(i)->set_selected(false);
        }

        rect_selected.clear();

        for (int i = 0; i < entity_vec.size(); ++i)
        {
            bool is_selected = false;
            bool in_rect = false;

            if ((rect_point.x() < entity_vec.at(i)->pos().x() && entity_vec.at(i)->pos().x() < rect_curr_point.x()) ||
                    (rect_point.x() > entity_vec.at(i)->pos().x() && entity_vec.at(i)->pos().x() > rect_curr_point.x()))
            {
                if ((rect_point.y() < entity_vec.at(i)->pos().y() && entity_vec.at(i)->pos().y() < rect_curr_point.y()) ||
                        (rect_point.y() > entity_vec.at(i)->pos().y() && entity_vec.at(i)->pos().y() > rect_curr_point.y()))
                {
                    in_rect = true;
                }
            }

            if (in_rect)
            {
                for (int j = 0; j < selected_entityes.size(); ++j)
                {
                    if (entity_vec.at(i) == selected_entityes.at(j))
                    {
                        is_selected = true;
                        break;
                    }
                }

                if (!is_selected)
                {
                    entity_vec.at(i)->set_selected(true);
                    rect_selected.append(entity_vec.at(i));
                }
            }
        }

        this->update();
    }

    if (one_one_tool || one_many_tool || many_many_tool || agregate_tool)
    {
        if (!tmp_conn_entity)
            return;

        QWidget* wid = this->childAt(me->pos());
        if (wid)
        {
            CEntityView *en = NULL;
            en = static_cast<CEntityView*> (wid);

            if (en)
            {
                if (en != tmp_conn_entity)
                {
                    tmp_conn_point = QPoint(en->pos().x() + en->geometry().width()/2,
                                             en->pos().y() + en->geometry().height()/2);
                    this->update();
                    return;
                }
            }
        }

        tmp_conn_point = me->pos();
        this->update();
    }
}

void CViewModel::mouseReleaseEvent(QMouseEvent *me)
{
    if (rect_selection)
    {
        rect_selection = false;

        multiply_selection = true;

        for (int i = 0; i < rect_selected.size(); ++i)
        {
            this->select_entity(rect_selected.at(i));
        }

        multiply_selection = false;

        rect_selected.clear();

        this->update();
    }

    if (one_one_tool || one_many_tool || many_many_tool || agregate_tool)
    {
        if (!tmp_conn_entity)
        {
            return;
        }

        QWidget* wid = this->childAt(me->pos());
        if (wid)
        {
            CEntityView *en = NULL;
            en = static_cast<CEntityView*> (wid);

            if (en)
            {
                if (en != tmp_conn_entity)
                {

                    int rtrn = addConnection(tmp_conn_entity->getId(), en->getId());
                    tmp_conn_entity = NULL;

                    if (rtrn == -1)
                    {
                        this->update();
                        return;
                    }

                    if (one_one_tool)
                    {
                        connection_vec.last()->getConnection()->setLeftMaxType(CConnection::ONE);
                        connection_vec.last()->getConnection()->setLeftMinType(CConnection::MANDATORY);
                        connection_vec.last()->getConnection()->setRightMaxType(CConnection::ONE);
                        connection_vec.last()->getConnection()->setRightMinType(CConnection::MANDATORY);
                    }
                    else
                    if (one_many_tool)
                    {
                        connection_vec.last()->getConnection()->setLeftMaxType(CConnection::ONE);
                        connection_vec.last()->getConnection()->setLeftMinType(CConnection::MANDATORY);
                        connection_vec.last()->getConnection()->setRightMaxType(CConnection::MANY);
                        connection_vec.last()->getConnection()->setRightMinType(CConnection::MANDATORY);
                    }
                    else
                    if (many_many_tool)
                    {
                        connection_vec.last()->getConnection()->setLeftMaxType(CConnection::MANY);
                        connection_vec.last()->getConnection()->setLeftMinType(CConnection::MANDATORY);
                        connection_vec.last()->getConnection()->setRightMaxType(CConnection::MANY);
                        connection_vec.last()->getConnection()->setRightMinType(CConnection::MANDATORY);
                    }
                    else
                    if (agregate_tool)
                    {
                        connection_vec.last()->getConnection()->setLeftMaxType(CConnection::ONE);
                        connection_vec.last()->getConnection()->setLeftMinType(CConnection::MANDATORY);
                        connection_vec.last()->getConnection()->setRightMaxType(CConnection::AGREGATE);
                        connection_vec.last()->getConnection()->setRightMinType(CConnection::MANDATORY);
                    }
                    this->update();
                    return;
                }
            }
        }

        tmp_conn_entity = NULL;
        this->update();
    }
}

void CViewModel::keyPressEvent(QKeyEvent *ke)
{
    if (ke->key() == Qt::Key_Control)
        multiply_selection = true;

    ke->accept();
}

void CViewModel::keyReleaseEvent(QKeyEvent *ke)
{
    if (ke->key() == Qt::Key_Control)
        multiply_selection = false;

    ke->accept();
}

void CViewModel::entity_pos_updated(CEntityView* entity)
{
    int point_num;
    CConnection* curr_conn;
    for (int i = 0; i < connection_vec.size(); ++i)
    {
        curr_conn = connection_vec.at(i)->getConnection();
        point_num = -1;

        if (curr_conn->get_left_entity_id() == entity->getId())
            point_num = 0;
        else
        if (curr_conn->get_right_entity_id() == entity->getId())
            point_num = connection_vec.at(i)->getPointsCntr() - 1;

        if (point_num >= 0)
        {
            connection_vec.at(i)->shiftPoint(QPoint(entity->geometry().x() + entity->geometry().width()*0.5,
                                                    entity->geometry().y() + entity->geometry().height()*0.5), point_num);
        }
    }

    linesIntersectionCulc();
    update();


    checkSize();
}

void CViewModel::context_menu_request(QPoint pos)
{
    if (parent)
    {
        save_CDM->setEnabled(false);
        save_CDM_as->setEnabled(false);
        load_CDM->setEnabled(false);
        add_new_entity->setEnabled(false);
        convert_to_query->setEnabled(false);

        to_script->setEnabled(true);
    }
    else
    {
        save_CDM->setEnabled(true);
        save_CDM_as->setEnabled(true);
        load_CDM->setEnabled(true);
        add_new_entity->setEnabled(true);
        convert_to_query->setEnabled(true);

        to_script->setEnabled(false);
    }
    context_menu->exec(pos);
}

void CViewModel::entity_context_menu_request(QPoint pos)
{
    if (parent)
    {
        remove_entity->setEnabled(false);
    }
    else
    {
        remove_entity->setVisible(true);
    }

    if (!parent)
    {
        if (selected_entityes.size() != 2)
            connect_entityes->setEnabled(false);
        else
            connect_entityes->setEnabled(true);
    }
    else
    {
        connect_entityes->setEnabled(false);
    }

    entity_context_menu->exec(pos);
}

void CViewModel::connection_context_menu_request(QPoint pos)
{
    if (!parent)
        connection_context_menu->exec(pos);
}

void CViewModel::timerEvent(QTimerEvent *te)
{
    while (selected_entityes.size())
    {
        CEntityView *curr_entity = selected_entityes.last();
        unselect_entity(curr_entity);
        remEntity(curr_entity->getId());
    }
    killTimer(timerId);
}

void CViewModel::clear_entity_selection()
{
    while (selected_entityes.size())
        selected_entityes.takeFirst()->set_selected(false);

}

void CViewModel::unselect_entity(CEntityView *entity)
{
    for (int i = 0; i < selected_entityes.size(); ++i)
    {
        if (selected_entityes.at(i) == entity)
        {
            selected_entityes.removeAt(i);
            entity->set_selected(false);
        }
    }
}

void CViewModel::select_entity(CEntityView *entity)
{
    if (!multiply_selection)
        clear_entity_selection();

    selected_entityes.append(entity);
    entity->set_selected(true);
}

void CViewModel::move_entity(QPoint point, int id)
{
    CEntityView* entity;
    QPoint prev_pos;

     for (int i = 0; i < entity_vec.size(); ++i)
     {
                if (id == entity_vec.at(i)->getId())
                {
                    entity = entity_vec.at(i);
                    break;
                }
     }

     prev_pos = entity->pos();
     entity->move(point);

     this->entity_pos_updated(entity);

     if (selected_entityes.size() > 1)
     {
         for (int i = 0; i < selected_entityes.size(); ++i)
         {
             if (selected_entityes.at(i) == entity)
                 continue;

             selected_entityes.at(i)->move(selected_entityes.at(i)->pos() + point - prev_pos);

             this->entity_pos_updated(entity);
         }
     }
}

int CViewModel::is_entityes_x_intersected(int entity1, int entity2)
{
    CEntityView *en_1 = NULL, *en_2 = NULL;
    for (int i = 0; i < entity_vec.size(); ++i)
    {
        if (entity_vec.at(i)->getId() == entity1)
            en_1 = entity_vec.at(i);
        if (entity_vec.at(i)->getId() == entity2)
            en_2 = entity_vec.at(i);
    }

    if (!en_1 || !en_2)
        return 0;

    if (en_1->x() > en_2->x() + en_2->width() + 33)
        return 0;
    if (en_1->x() + en_1->width() + 33 < en_2->x())
        return 0;

    int center1, center2;
    center1 = en_1->x() + en_1->width()/2;
    center2 = en_2->x() + en_2->width()/2;

    if (center2 >= center1)
        return en_1->width()/2 + 25 + en_2->x() + en_2->width() - en_1->x() - en_1->width();
    else
        return -(en_1->width()/2 + 25 + en_1->x() - en_2->x());
}

QVector<CEntityView *> CViewModel::getEntityes()
{
    return entity_vec;
}

void CViewModel::pre_saveInFile(QString _name)
{
    currFileName = _name;
    saveInFile(_name);

    this->setWindowTitle("CASE_SW - " + currFileName);
}

bool CViewModel::pre_loadFromFile(QString _name)
{
    QFile file(_name + QString(".cdmod"));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    else
        file.close();

    for (int i = 0; i < connection_vec.size(); ++i)
        delete connection_vec.at(i);
    connection_vec.clear();

    for (int i = 0; i < entity_vec.size(); ++i)
        delete entity_vec.at(i);
    entity_vec.clear();

    for (int i = 0; i < connection_views.size(); ++i)
        delete connection_views.at(i);
    connection_views.clear();

    for (int i = 0; i < detailed_views.size(); ++i)
        delete detailed_views.at(i);
    detailed_views.clear();

    update();

    data_model->removeAll();

    currFileName = _name;
    loadFromFile(_name);

    this->setWindowTitle("CASE_SW - " + currFileName);
    return true;
}

bool CViewModel::check_entity_name(QString _name, int id)
{
    if (_name == "")
            return false;
        for (int i = 0; i < _name.size(); ++i)
        {
            if (_name.at(i) == ' ' || _name.at(i) == '\'')
                return false;
        }
        for (int i = 0; i < entity_vec.size(); ++i)
        {
            if (entity_vec.at(i)->getId() != id)
            {
                if (entity_vec.at(i)->getEntity()->getName() == _name)
                    return false;
            }
        }
        return true;
}

void CViewModel::closeEvent(QCloseEvent *event)
{
    if (parent)
        parent->setEnabled(true);
    parent = NULL;
    delete this;
}

void CViewModel::cursor_tool_activate()
{
    set_entityes_selectable(true);
    disable_all_tools();

    cursor_tool = true;
}

void CViewModel::create_entity_tool_activate()
{
    set_entityes_selectable(true);
    disable_all_tools();

    create_entity_tool = true;
}

void CViewModel::remove_entity_tool_activate()
{
    set_entityes_selectable(false);
    disable_all_tools();

    remove_entity_tool = true;
}

void CViewModel::one_one_tool_activate()
{
    set_entityes_selectable(false);
    disable_all_tools();

    one_one_tool = true;
}

void CViewModel::one_many_tool_activate()
{
    set_entityes_selectable(false);
    disable_all_tools();

    one_many_tool = true;
}

void CViewModel::many_many_tool_activate()
{
    set_entityes_selectable(false);
    disable_all_tools();

    many_many_tool = true;
}

void CViewModel::aggregate_tool_activate()
{
    set_entityes_selectable(false);
    disable_all_tools();

    agregate_tool = true;
}

void CViewModel::setMultiplySelection(bool flag)
{
    multiply_selection = flag;
}

void CViewModel::reset_text()
{
    detailed_entity_view->setText(tr("Detailed"));
    connect_entityes->setText(tr("Add connection"));
    remove_entity->setText(tr("Remove"));

    add_new_entity->setText(tr("Create entity"));
    convert_to_query->setText(tr("To PDM"));

    rem_con->setText(tr("Remove"));
    detail_con->setText(tr("Detail"));

    save_CDM->setText(tr("Save CDM"));
    save_CDM_as->setText(tr("Save CDM as"));
    load_CDM->setText(tr("Load CDM"));

    to_script->setText(tr("Convert to query"));
}

void CViewModel::show_entity_detail()
{
    for (int k = 0; k < selected_entityes.size(); ++k)
    {
        bool is_detailed = false;
        for (int i = 0; i < detailed_views.size(); ++i)
        {
            if (detailed_views.at(i)->getEntity() == selected_entityes.at(k))
            {
                detailed_views.at(i)->activateWindow();
                is_detailed = true;
                break;
            }
        }

        if (is_detailed)
            continue;

        if (!parent)
            detailed_views.append(new CDetailedEntityWidget(this, selected_entityes.at(k)));
        else
            detailed_views.append(new CDetailedEntityWidget(this, selected_entityes.at(k), false));

        connect(detailed_views.last(), SIGNAL(closing(CDetailedEntityWidget*)), this, SLOT(detailed_view_destroyed(CDetailedEntityWidget*)));
    }
}

void CViewModel::remove_selected_entityes()
{
    if (selected_entityes.size())
        timerId = startTimer(10);
}

void CViewModel::connect_selected_entityes()
{
    if (selected_entityes.size() != 2)
        return;

    addConnection(selected_entityes.at(0)->getId(), selected_entityes.at(1)->getId());

    update();
}

void CViewModel::remove_selected_connection()
{
    if (selected_connection && !parent)
    {
        remConnection(selected_connection->getId());
        selected_connection = NULL;
    }
}

void CViewModel::show_connection_detail()
{
    if (selected_connection)
    {
        bool is_detailed = false;
        for (int i = 0; i < connection_views.size(); ++i)
        {
            if (connection_views.at(i)->getConnection() == selected_connection)
            {
                connection_views.at(i)->activateWindow();
                is_detailed = true;
                break;
            }
        }

        if (!is_detailed)
            connection_views.append(new CConnectionViewWidget(this, selected_connection));
        selected_connection = NULL;

        connect(connection_views.last(), SIGNAL(closing(CConnectionViewWidget*)),
                this, SLOT(conn_view_destroyed(CConnectionViewWidget*)));
    }
}

void CViewModel::detailed_view_destroyed(CDetailedEntityWidget *wid)
{
    for (int i = 0; i < detailed_views.size(); ++i)
    {
        if (wid == detailed_views.at(i))
            delete detailed_views.takeAt(i);
    }
}

void CViewModel::add_new_entity_event()
{
    int entity_num = addEntity();

    move_entity(this->cursor().pos() - this->mapToGlobal(this->pos()) - QPoint(-1, -30), entity_num);
}

void CViewModel::convert_to_query_event()
{
    CDataModel *pdm = data_model->convertToPDM();

    if (!pdm->isValid())
    {
        if (pdm->getErrorCode() == 1)
        {
            QMessageBox *msg = new QMessageBox;
            msg->setText("There are entityes with no primary key selected!");
            msg->show();
        }
        if (pdm->getErrorCode() == 2)
        {
            QMessageBox *msg = new QMessageBox;
            msg->setText("There are no entityes!");
            msg->show();
        }
        return;
    }

    CViewModel *PDMView = PDMViewConstructor(pdm);
    PDMView->show();

    CScrollArea *sc_area = new CScrollArea();

    sc_area->setWidget(PDMView);
    sc_area->show();

    PDMView->setMinimumSize(200, 100);
    sc_area->setMinimumSize(100, 80);

    PDMView->parent_area = sc_area;

    //this->setEnabled(false);

    return;
}

void CViewModel::load_and_save_view_destroyed()
{
    w_opened = false;
    delete load_and_save_view;
}

void CViewModel::save_CDM_event()
{
    if (entity_vec.size() != 0)
        saveInFile(currFileName);
    else
    {
        QMessageBox *msg = new QMessageBox;
        msg->setText("Your file is empty. You cannot save it.");
        msg->show();
    }
}

void CViewModel::save_CDM_as_event()
{
    if (entity_vec.size() != 0)
    {
        if (!w_opened)
        {
            load_and_save_view = new CLoadAndSaveWidget(this, true);
            load_and_save_view->show();
            w_opened = true;
        }
        else
            load_and_save_view->activateWindow();

        connect(load_and_save_view, SIGNAL(closing()),
                this, SLOT(load_and_save_view_destroyed()));
    }
    else
    {
        QMessageBox *msg = new QMessageBox;
        msg->setText("Your file is empty. You cannot save it.");
        msg->show();
    }
}

void CViewModel::load_CDM_event()
{
    if (!w_opened)
    {
        load_and_save_view = new CLoadAndSaveWidget(this, false);
        load_and_save_view->show();
        w_opened = true;
    }
    else
        load_and_save_view->activateWindow();

    connect(load_and_save_view, SIGNAL(closing()),
            this, SLOT(load_and_save_view_destroyed()));
}

void CViewModel::to_script_event()
{
    ScriptGen = new CScriptGenerator(data_model->convertToPDM(), app);
    ScriptGen->loadPlugins(LIB_PATH);
    ScriptGen->show();
}

void CViewModel::showContextMenu(const QPoint &pos, bool isEnabled)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction actionDelete("Delete", this);
    connect(&actionDelete, SIGNAL(triggered(bool)), this, SLOT(remove_selected_entityes()));
    actionDelete.setEnabled(isEnabled);
    contextMenu.addAction(&actionDelete);

    contextMenu.exec(mapToGlobal(pos));
}

void CViewModel::showTableContextMenu(const QPoint &pos, QString tableName)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction actionEdit(QString("Edit '%1'").arg(tableName), this);
    connect(&actionEdit, SIGNAL(triggered(bool)), this, SLOT(show_entity_detail()));
    contextMenu.addAction(&actionEdit);

    QAction actionDelete("Delete", this);
    connect(&actionDelete, SIGNAL(triggered(bool)), this, SLOT(remove_selected_entityes()));
    contextMenu.addAction(&actionDelete);

    contextMenu.exec(mapToGlobal(pos));
}

void CViewModel::showTableRelationshipContextMenu(const QPoint &pos)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction actionRelate("Create Relationship", this);
    connect(&actionRelate, SIGNAL(triggered(bool)), this, SLOT(connect_selected_entityes()));
    contextMenu.addAction(&actionRelate);

    QAction actionDelete("Delete", this);
    connect(&actionDelete, SIGNAL(triggered(bool)), this, SLOT(remove_selected_entityes()));
    contextMenu.addAction(&actionDelete);

    contextMenu.exec(mapToGlobal(pos));
}

void CViewModel::showRelationshipContextMenu(const QPoint &pos)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction actionEdit("Edit Relationship", this);
    connect(&actionEdit, SIGNAL(triggered(bool)), this, SLOT(show_connection_detail()));
    contextMenu.addAction(&actionEdit);

    QAction actionDelete("Delete", this);
    connect(&actionDelete, SIGNAL(triggered(bool)), this, SLOT(remove_selected_connection()));
    contextMenu.addAction(&actionDelete);

    contextMenu.exec(mapToGlobal(pos));
}
// LOOK INTO
void CViewModel::loadFromFile(QString file_name)
{
    QFile file(file_name);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&file);

    QString line;

    in >> line;
    int leid = line.toInt();
    data_model->setLastEntityId(leid);

    in >> line;
    int ent_size = line.toInt();
    for (int i = 0; i < ent_size; ++i)
    {
        QString en_name;
        in >> en_name;
        in >> line;
        int en_x = line.toInt();
        in >> line;
        int en_y = line.toInt();

        int entity_num = addEntity();
        move_entity(QPoint(en_x, en_y), entity_num);
        entity_vec.last()->getEntity()->setName(en_name);

        in >> line;
        int attr_size = line.toInt();
        for (int j = 0; j < attr_size; ++j)
        {
            QString attr_name;
            in >> attr_name;
            if (attr_name == "*attr_name*")
                attr_name = "";
            in >> line;
            int attr_type = line.toInt();
            in >> line;
            int attr_count = line.toInt();
            in >> line;
            int attr_key = line.toInt();
            in >> line;
            int attr_null = line.toInt();
            in >> line;
            int attr_unique = line.toInt();

            int row_num = entity_vec.last()->getEntity()->addRow();
            entity_vec.last()->getEntity()->getRow(row_num)->setName(attr_name);
            entity_vec.last()->getEntity()->getRow(row_num)->setPrimaryKey(attr_key);
            entity_vec.last()->getEntity()->getRow(row_num)->setNotNull(attr_null);
            entity_vec.last()->getEntity()->getRow(row_num)->setUnique(attr_unique);

            if (attr_type == 0)
                entity_vec.last()->getEntity()->getRow(row_num)->setType(CRow::INTEGER, attr_count);
            else
            if (attr_type == 1)
                entity_vec.last()->getEntity()->getRow(row_num)->setType(CRow::FLOAT, attr_count);
            else
            if (attr_type == 2)
                entity_vec.last()->getEntity()->getRow(row_num)->setType(CRow::CHAR, attr_count);
            else
            if (attr_type == 3)
                entity_vec.last()->getEntity()->getRow(row_num)->setType(CRow::BOOLEAN, attr_count);
            else
            if (attr_type == 4)
                entity_vec.last()->getEntity()->getRow(row_num)->setType(CRow::DATE, attr_count);
            else
            if (attr_type == 5)
                entity_vec.last()->getEntity()->getRow(row_num)->setType(CRow::BLOB, attr_count);
        }

        in >> line;
        int ung_count = line.toInt();
        for (int k = 0; k < ung_count; ++k)
        {
            entity_vec.last()->getEntity()->addCmplxUniquesGroup();

            in >> line;
            int ung_size = line.toInt();
            for (int m = 0; m < ung_size; ++m)
            {
                in >> line;
                QString ung_el = line;
                for (int r = 0; r < attr_size; ++r)
                {
                    if (entity_vec.last()->getEntity()->getRow(r)->getName() == ung_el)
                    {
                        entity_vec.last()->getEntity()->addToCmplxUniqueGroup(k, r);
                        break;
                    }
                }
            }
        }
    }

    in >> line;
    int con_size = line.toInt();
    for (int i = 0; i < con_size; ++i)
    {
        QString l_en, r_en;
        int l_en_id, r_en_id;
        in >> l_en >> r_en;
        for (int j = 0; j < entity_vec.size(); ++j)
        {
            if (entity_vec.at(j)->getEntity()->getName() == l_en)
                l_en_id = entity_vec.at(j)->getId();
            if (entity_vec.at(j)->getEntity()->getName() == r_en)
                r_en_id = entity_vec.at(j)->getId();
        }

        addConnection(l_en_id, r_en_id);
        in >> line;
        int l_min_type = line.toInt();
        in >> line;
        int r_min_type = line.toInt();
        in >> line;
        int l_max_type = line.toInt();
        in >> line;
        int r_max_type = line.toInt();

        if (l_min_type == 0)
            connection_vec.last()->getConnection()->setLeftMinType(CConnection::NONE);
        else
        if (l_min_type == 1)
            connection_vec.last()->getConnection()->setLeftMinType(CConnection::MANDATORY);
        if (r_min_type == 0)
            connection_vec.last()->getConnection()->setRightMinType(CConnection::NONE);
        else
        if (r_min_type == 1)
            connection_vec.last()->getConnection()->setRightMinType(CConnection::MANDATORY);

        if (l_max_type == 0)
            connection_vec.last()->getConnection()->setLeftMaxType(CConnection::ONE);
        else
        if (l_max_type == 1)
            connection_vec.last()->getConnection()->setLeftMaxType(CConnection::MANY);
        else
        if (l_max_type == 2)
            connection_vec.last()->getConnection()->setLeftMaxType(CConnection::AGREGATE);
        if (r_max_type == 0)
            connection_vec.last()->getConnection()->setRightMaxType(CConnection::ONE);
        else
        if (r_max_type == 1)
            connection_vec.last()->getConnection()->setRightMaxType(CConnection::MANY);
        else
        if (r_max_type == 2)
            connection_vec.last()->getConnection()->setRightMaxType(CConnection::AGREGATE);
    }

    this->cursor_tool_activate();

    file.close();

    checkSize();
}
// LOOK INTO
void CViewModel::saveInFile(QString file_name)
{
    QFile file(file_name);
    file.open(QIODevice::WriteOnly |
              QIODevice::Text |
              QIODevice::Truncate);
    QTextStream out(&file);

    out << entity_vec.last()->getId() << "\n";

    out << entity_vec.size() << "\n";

    for (int i = 0; i < entity_vec.size(); ++i)
    {
        out << entity_vec.at(i)->getEntity()->getName() << " "
            << entity_vec.at(i)->x() << " " << entity_vec.at(i)->y() << "\n"
            << entity_vec.at(i)->getEntity()->getRowCount() << "\n";

        for (int j = 0; j < entity_vec.at(i)->getEntity()->getRowCount(); ++j)
        {
            if (entity_vec.at(i)->getEntity()->getRow(j)->getName() == "")
                out << "*attr_name*";
            else
                out << entity_vec.at(i)->getEntity()->getRow(j)->getName();
            out << " " << entity_vec.at(i)->getEntity()->getRow(j)->getType()
                << " " << entity_vec.at(i)->getEntity()->getRow(j)->getCount()
                << " " << entity_vec.at(i)->getEntity()->getRow(j)->isPrimaryKey()
                << " " << entity_vec.at(i)->getEntity()->getRow(j)->isNotNull()
                << " " << entity_vec.at(i)->getEntity()->getRow(j)->isUnique()<< "\n";
        }

        out << entity_vec.at(i)->getEntity()->getComplexUniques().count() << "\n";
        for (int k = 0; k < entity_vec.at(i)->getEntity()->getComplexUniques().count(); ++k)
        {
            out << entity_vec.at(i)->getEntity()->getComplexUniques().at(k).count() << "\n";
            for (int m = 0; m < entity_vec.at(i)->getEntity()->getComplexUniques().at(k).count(); ++m)
            {
                for (int s = 0; s < entity_vec.at(i)->getEntity()->getRowCount(); ++s)
                {
                    if (entity_vec.at(i)->getEntity()->getComplexUniques().at(k).at(m) == s)
                    {
                        out << entity_vec.at(i)->getEntity()->getRow(s)->getName() << " ";
                        break;
                    }
                }
            }
            out << "\n";
        }
    }

    out << connection_vec.size() << "\n";

    for (int i = 0; i < connection_vec.size(); ++i)
    {
        QString lname, rname;
        for (int j = 0; j < entity_vec.size(); ++j)
        {
            if (entity_vec.at(j)->getId() == connection_vec.at(i)->getConnection()->get_left_entity_id())
                lname = entity_vec.at(j)->getEntity()->getName();
            if (entity_vec.at(j)->getId() == connection_vec.at(i)->getConnection()->get_right_entity_id())
                rname = entity_vec.at(j)->getEntity()->getName();
        }
        out << lname << " " << rname << " ";

        out << connection_vec.at(i)->getConnection()->getLeftMinType()
            << " " << connection_vec.at(i)->getConnection()->getRightMinType()
            << " " << connection_vec.at(i)->getConnection()->getLeftMaxType()
            << " " << connection_vec.at(i)->getConnection()->getRightMaxType() << "\n";
    }

    file.close();
}

void CViewModel::conn_view_destroyed(CConnectionViewWidget *wid)
{
    for (int i = 0; i < connection_views.size(); ++i)
        {
            if (wid == connection_views.at(i))
                delete connection_views.takeAt(i);
        }
}

void CViewModel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::darkGray, 1));

    if (rect_selection)
    {
        painter.drawLine(rect_point, QPoint(rect_point.x(), rect_curr_point.y()));
        painter.drawLine(QPoint(rect_point.x(), rect_curr_point.y()), rect_curr_point);
        painter.drawLine(rect_curr_point, QPoint(rect_curr_point.x(), rect_point.y()));
        painter.drawLine(QPoint(rect_curr_point.x(), rect_point.y()), rect_point);
    }

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::black, 1));

    for (int i = 0; i < connection_vec.size(); ++i)
    {
        if (connection_vec.at(i)->get_points()->length() < 2)
            continue;

        CEntityView *en1 = NULL, *en2 = NULL;

        for (int j = 0; j < entity_vec.size(); ++j)
        {
            if (entity_vec.at(j)->getId() == connection_vec.at(i)->getConnection()->get_left_entity_id())
                en1 = entity_vec.at(j);
            if (entity_vec.at(j)->getId() == connection_vec.at(i)->getConnection()->get_right_entity_id())
                en2 = entity_vec.at(j);

            if (en1 && en2)
                break;
        }

        QPoint point;
        if (en1->x() + en1->width() < connection_vec.at(i)->get_points()->at(1).x())
            point.setX(en1->x() + en1->width() - 5);
        else
            point.setX(en1->x() + 5);
        point.setY(connection_vec.at(i)->get_points()->at(0).y());
        draw_connection_line(point,
                             connection_vec.at(i)->get_points()->at(1),
                             connection_vec.at(i)->getConnection()->getLeftMaxType(),
                             connection_vec.at(i)->getConnection()->getLeftMinType(), painter);

        if (en2->x() + en2->width() < connection_vec.at(i)->get_points()->at(connection_vec.at(i)->get_points()->size() - 2).x())
            point.setX(en2->x() + en2->width() - 5);
        else
            point.setX(en2->x() + 5);

        point.setY(connection_vec.at(i)->get_points()->last().y());

        draw_connection_line(point,
                             connection_vec.at(i)->get_points()->at(connection_vec.at(i)->get_points()->size() - 2),
                             connection_vec.at(i)->getConnection()->getRightMaxType(),
                             connection_vec.at(i)->getConnection()->getRightMinType(), painter);

        for(int j = 0; j < connection_vec.at(i)->get_points()->length() - 1; ++j)
            painter.drawLine(connection_vec.at(i)->get_points()->at(j), connection_vec.at(i)->get_points()->at(j + 1));
    }

    if (tmp_conn_entity)
    {
        QPoint p1;

        p1.setX(tmp_conn_entity->x() + tmp_conn_entity->width()/2);
        p1.setY(tmp_conn_entity->y() + tmp_conn_entity->height()/2);

        if (p1.y() == tmp_conn_point.y())
        {
            painter.drawLine(p1, tmp_conn_point);
        }
        else
        {
            painter.drawLine(p1, QPoint((p1.x() + tmp_conn_point.x())/2, p1.y()));
            painter.drawLine(QPoint((p1.x() + tmp_conn_point.x())/2, p1.y()),
                             QPoint((p1.x() + tmp_conn_point.x())/2, tmp_conn_point.y()));
            painter.drawLine(QPoint((p1.x() + tmp_conn_point.x())/2, tmp_conn_point.y()), tmp_conn_point);
        }
    }
}

void CViewModel::checkSize()
{
    int mwidth = -1, mheight = -1;

    if (this->data_model->is_PDM())
    {
        if (parent_area)
        {
            mwidth = parent_area->width();
            mheight = parent_area->height();
        }
    }
    else
    {
        if (mw)
        {
            mwidth = mw->width();
            mheight = mw->height();
        }
    }

    if (mwidth == -1 || mheight == -1)
        return;

    QSize max_size(0, 0);
    for (int i = 0; i < entity_vec.size(); ++i)
    {
        if (entity_vec.at(i)->x() + entity_vec.at(i)->width() + 60 > max_size.width())
            max_size.setWidth(entity_vec.at(i)->x() + entity_vec.at(i)->width() + 60);
        if (entity_vec.at(i)->y() + entity_vec.at(i)->height() + 60 > max_size.height())
            max_size.setHeight(entity_vec.at(i)->y() + entity_vec.at(i)->height() + 60);
    }

    bool width_changed = true, height_changed = true;

    if (mwidth > max_size.width())
    {
        max_size.setWidth(mwidth);
        width_changed = false;
    }
    if (mheight > max_size.height())
    {
        max_size.setHeight(mheight);
        height_changed = false;
    }

    if (width_changed || height_changed)
        this->resize(max_size);
}

void CViewModel::disable_all_tools()
{
    cursor_tool = false;
    create_entity_tool = false;
    remove_entity_tool = false;
    one_one_tool = false;
    one_many_tool = false;
    many_many_tool = false;
    agregate_tool = false;
}

void CViewModel::set_entityes_selectable(bool flag)
{
    clear_entity_selection();

    for (int i = 0; i < entity_vec.size(); ++i)
    {
        entity_vec.at(i)->selectable = flag;
    }
}

void CViewModel::draw_connection_line(QPoint p1, QPoint p2, CConnection::CONNECTION_MAX_TYPE max, CConnection::CONNECTION_MIN_TYPE min, QPainter &painter)
{
    painter.drawLine(p1, p2);
    QPainterPath path;

    if (max == CConnection::ONE)
    {

    }
    else
    if (max == CConnection::PHYSICAL)
    {
        if (p1.x() < p2.x())
        {
            painter.drawLine(QPoint(p1.x(), p1.y()), QPoint(p1.x() + 15, p1.y() + 7));
            painter.drawLine(QPoint(p1.x(), p1.y()), QPoint(p1.x() + 15, p1.y() - 7));
        }
        else
        {
            painter.drawLine(QPoint(p1.x(), p1.y()), QPoint(p1.x() - 15, p1.y() + 7));
            painter.drawLine(QPoint(p1.x(), p1.y()), QPoint(p1.x() - 15, p1.y() - 7));
        }
    }
    else
    if (max == CConnection::MANY)
    {
        if (p1.x() < p2.x())
        {
            painter.drawLine(QPoint(p1.x(), p1.y() + 7), QPoint(p1.x() + 15, p1.y()));
            painter.drawLine(QPoint(p1.x(), p1.y() - 7), QPoint(p1.x() + 15, p1.y()));
        }
        else
        {
            painter.drawLine(QPoint(p1.x(), p1.y() + 7), QPoint(p1.x() - 15, p1.y()));
            painter.drawLine(QPoint(p1.x(), p1.y() - 7), QPoint(p1.x() - 15, p1.y()));
        }
    }
    else
    if (max == CConnection::AGREGATE)
    {
        if (p1.x() < p2.x())
        {
            painter.drawLine(QPoint(p1.x(), p1.y() + 7), QPoint(p1.x() + 7, p1.y() + 7));
            painter.drawLine(QPoint(p1.x(), p1.y() - 7), QPoint(p1.x() + 7, p1.y() - 7));
            path.moveTo(p1.x() + 7, p1.y() + 7);
            path.lineTo(p1.x() + 15, p1.y());
            path.lineTo(p1.x() + 7, p1.y() - 7);
            path.lineTo(p1.x() + 7, p1.y() + 7);
            painter.drawPath(path);
            painter.fillPath(path, QBrush(Qt::white));
        }
        else
        {
            painter.drawLine(QPoint(p1.x(), p1.y() + 7), QPoint(p1.x() - 7, p1.y() + 7));
            painter.drawLine(QPoint(p1.x(), p1.y() - 7), QPoint(p1.x() - 7, p1.y() - 7));
            path.moveTo(p1.x() - 7, p1.y() + 7);
            path.lineTo(p1.x() - 15, p1.y());
            path.lineTo(p1.x() - 7, p1.y() - 7);
            path.lineTo(p1.x() - 7, p1.y() + 7);
            painter.drawPath(path);
            painter.fillPath(path, QBrush(Qt::white));
        }
    }

    if (min == CConnection::NONE)
    {
        if (p1.x() < p2.x())
        {
            path.addEllipse(QPoint(p1.x() + 20, p1.y()), 4, 4);
            painter.drawPath(path);
            painter.fillPath(path, QBrush(Qt::white));
        }
        else
        {
            path.addEllipse(QPoint(p1.x() - 20, p1.y()), 4, 4);
            painter.drawPath(path);
            painter.fillPath(path, QBrush(Qt::white));
        }
    }
    else
    if (min == CConnection::MANDATORY)
    {
        if (p1.x() < p2.x())
            painter.drawLine(QPoint(p1.x() + 20, p1.y() + 5), QPoint(p1.x() + 20, p1.y() - 5));
        else
            painter.drawLine(QPoint(p1.x() - 20, p1.y() + 5), QPoint(p1.x() - 20, p1.y() - 5));
    }
}

void CViewModel::setCurrentModel(CDataModel *model)
{

}

void CViewModel::linesIntersectionCulc()
{
    entityesLinesCheck();
/*
    for (int i = 0; i < connection_vec.size(); ++i)
    {
        for (int j = i + 1; j < connection_vec.size(); ++j)
        {
            linesIntersectionCheck(connection_vec.at(i), connection_vec.at(j));
        }
    }*/
}

void CViewModel::entityesLinesCheck()
{
    for (int i = 0; i < entity_vec.size(); ++i)
    {
        QVector<CConnectionView*> cons_left;
        QVector<CConnectionView*> cons_right;
        for (int j = 0; j < connection_vec.size(); ++j)
        {
            if (connection_vec.at(j)->getConnection()->get_left_entity_id() == entity_vec.at(i)->getId())
                if (connection_vec.at(j)->getPointsCntr() >= 2)
                    if (connection_vec.at(j)->get_points()->at(0).x() < connection_vec.at(j)->get_points()->at(1).x())
                        cons_right.append(connection_vec.at(j));
                    else
                        cons_left.append(connection_vec.at(j));
            if (connection_vec.at(j)->getConnection()->get_right_entity_id() == entity_vec.at(i)->getId())
                if (connection_vec.at(j)->getPointsCntr() >= 2)
                    if (connection_vec.at(j)->get_points()->at(connection_vec.at(j)->getPointsCntr() - 1).x() < connection_vec.at(j)->get_points()->at(connection_vec.at(j)->getPointsCntr() - 2).x())
                        cons_right.append(connection_vec.at(j));
                    else
                        cons_left.append(connection_vec.at(j));
        }

        for (int j = 0; j < cons_left.size(); ++j)
        {
            int max = j;
            for (int k = j + 1; k < cons_left.size(); ++k)
            {
                int y1, y2;
                if (cons_left.at(max)->getConnection()->get_left_entity_id() == entity_vec.at(i)->getId())
                    y1 = cons_left.at(max)->get_points()->at(cons_left.at(max)->getPointsCntr() - 1).y();
                else
                    y1 = cons_left.at(max)->get_points()->at(0).y();

                if (cons_left.at(k)->getConnection()->get_left_entity_id() == entity_vec.at(i)->getId())
                    y2 = cons_left.at(k)->get_points()->at(cons_left.at(k)->getPointsCntr() - 1).y();
                else
                    y2 = cons_left.at(k)->get_points()->at(0).y();

                if (y2 > y1)
                    max = k;
            }

            CConnectionView* tmp = cons_left[j];
            cons_left[j] = cons_left[max];
            cons_left[max] = tmp;
        }

        for (int j = 0; j < cons_right.size(); ++j)
        {
            int max = j;
            for (int k = j + 1; k < cons_right.size(); ++k)
            {
                int y1, y2;
                if (cons_right.at(max)->getConnection()->get_left_entity_id() == entity_vec.at(i)->getId())
                    y1 = cons_right.at(max)->get_points()->at(cons_right.at(max)->getPointsCntr() - 1).y();
                else
                    y1 = cons_right.at(max)->get_points()->at(0).y();

                if (cons_right.at(k)->getConnection()->get_left_entity_id() == entity_vec.at(i)->getId())
                    y2 = cons_right.at(k)->get_points()->at(cons_right.at(k)->getPointsCntr() - 1).y();
                else
                    y2 = cons_right.at(k)->get_points()->at(0).y();

                if (y2 > y1)
                    max = k;
            }

            CConnectionView* tmp = cons_right[j];
            cons_right[j] = cons_right[max];
            cons_right[max] = tmp;
        }

        int curr_y = entity_vec.at(i)->y() +entity_vec.at(i)->height()/2;
        curr_y += 15*(cons_left.size()/2);
        for (int j = 0; j < cons_left.size(); ++j)
        {
            if (cons_left.at(j)->getConnection()->get_left_entity_id() == entity_vec.at(i)->getId())
                cons_left.at(j)->shiftPoint(QPoint(entity_vec.at(i)->pos().x() + entity_vec.at(i)->width()/2, curr_y), 0);
            else
                cons_left.at(j)->shiftPoint(QPoint(entity_vec.at(i)->pos().x() + entity_vec.at(i)->width()/2, curr_y), cons_left.at(j)->getPointsCntr() - 1);
            curr_y -= 15;
        }
        curr_y = entity_vec.at(i)->y() +entity_vec.at(i)->height()/2;
        curr_y += 15*(cons_right.size()/2);
        for (int j = 0; j < cons_right.size(); ++j)
        {
            if (cons_right.at(j)->getConnection()->get_left_entity_id() == entity_vec.at(i)->getId())
                cons_right.at(j)->shiftPoint(QPoint(entity_vec.at(i)->pos().x() + entity_vec.at(i)->width()/2, curr_y), 0);
            else
                cons_right.at(j)->shiftPoint(QPoint(entity_vec.at(i)->pos().x() + entity_vec.at(i)->width()/2, curr_y), cons_right.at(j)->getPointsCntr() - 1);
            curr_y -= 15;
        }
    }
}

void CViewModel::linesIntersectionCheck(CConnectionView *con1, CConnectionView *con2)
{
    for (int i = 0; i < con1->get_points()->size() - 1; ++i)
    {
        for (int j = 0; j < con2->get_points()->size() - 1; ++j)
        {
            QPoint p11, p12, p21, p22;
            p11 = con1->get_points()->at(i);
            p12 = con1->get_points()->at(i + 1);
            p21 = con2->get_points()->at(j);
            p22 = con2->get_points()->at(j + 1);

            int range = fabs(p11.x() - p21.x());
            if ((p11.x() == p12.x()) && (p21.x() == p22.x()) && (range <= 8))
            {
                bool move = false;
                if (p21.y() >= p11.y() && p11.y() >= p22.y())
                    move = true;
                if (p21.y() >= p12.y() && p12.y() >= p22.y())
                    move = true;
                if (p21.y() <= p11.y() && p11.y() <= p22.y())
                    move = true;
                if (p21.y() <= p12.y() && p12.y() <= p22.y())
                    move = true;

                if (move)
                {
                    con1->shiftPoint(p12 + QPoint(15, 0), i+1);
                }
            }
        }
    }
}


