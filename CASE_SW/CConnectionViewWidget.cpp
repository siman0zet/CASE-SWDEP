#include "CConnectionViewWidget.h"

#include "CViewModel.h"
#include "CConnectionView.h"

CConnectionViewWidget::CConnectionViewWidget(CViewModel *_parent, CConnectionView *con_view):
    connection_view(con_view), parent(_parent)
{
    left_entity = new QComboBox(this);
    left_lower_num = new QComboBox(this);
    left_upper_num = new QComboBox(this);

    right_entity = new QComboBox(this);
    right_lower_num = new QComboBox(this);
    right_upper_num = new QComboBox(this);

    left_entity_text = new QLabel(this);
    left_lower_num_text = new QLabel(this);
    left_upper_num_text = new QLabel(this);

    right_entity_text = new QLabel(this);
    right_lower_num_text = new QLabel(this);
    right_upper_num_text = new QLabel(this);

    this->setGeometry(20, 20, 400, 180);
    left_entity->setGeometry(10, 25, 150, 25);
    left_lower_num->setGeometry(10, 75, 150, 25);
    left_upper_num->setGeometry(10, 125, 150, 25);

    right_entity->setGeometry(225, 25, 150, 25);
    right_lower_num->setGeometry(225, 75, 150, 25);
    right_upper_num->setGeometry(225, 125, 150, 25);

    left_entity_text->setGeometry(10, 5, 150, 16);
    left_lower_num_text->setGeometry(10, 55, 150, 16);
    left_upper_num_text->setGeometry(10, 105, 150, 16);

    right_entity_text->setGeometry(225, 5, 150, 16);
    right_lower_num_text->setGeometry(225, 55, 150, 16);
    right_upper_num_text->setGeometry(225, 105, 150, 16);

    left_entity_text->setText(tr("Left Entity"));
    left_lower_num_text->setText(tr("Left lower number"));
    left_upper_num_text->setText(tr("Left upper number"));

    right_entity_text->setText(tr("Right Entity"));
    right_lower_num_text->setText(tr("Right lower number"));
    right_upper_num_text->setText(tr("Right upper number"));
    this->show();

    connect(left_entity, SIGNAL(currentIndexChanged(QString)), this, SLOT(left_entity_changed(QString)));
    connect(right_entity, SIGNAL(currentIndexChanged(QString)), this, SLOT(right_entity_changed(QString)));

    QStringList lower_nums;

    lower_nums << tr("Mandatory") << tr("Optional");

    left_lower_num->addItems(lower_nums);
    right_lower_num->addItems(lower_nums);

    connect(left_lower_num, SIGNAL(currentIndexChanged(int)), this, SLOT(left_lower_num_changed(int)));
    connect(right_lower_num, SIGNAL(currentIndexChanged(int)), this, SLOT(right_lower_num_changed(int)));

    QStringList upper_nums;

    upper_nums << tr("One") << tr("Many") << tr("Agregate");

    left_upper_num->addItems(upper_nums);
    right_upper_num->addItems(upper_nums);

    connect(left_upper_num, SIGNAL(currentIndexChanged(int)), this, SLOT(left_upper_num_changed(int)));
    connect(right_upper_num, SIGNAL(currentIndexChanged(int)), this, SLOT(right_upper_num_changed(int)));

    inserted = false;

    this->setMaximumSize(400, 180);
    this->setMinimumSize(400, 180);

    update_state();
}

CConnectionView *CConnectionViewWidget::getConnection()
{
    return connection_view;
}



void CConnectionViewWidget::closeEvent(QCloseEvent *event)
{
    emit closing(this);
}

void CConnectionViewWidget::update_state()
{
    entity_vec = parent->getEntityes();

    QStringList leftList, rightList;

    int curr_left_entity, curr_right_entity;

    for (int i = 0; i < entity_vec.size(); ++i)
    {
        if (connection_view->getConnection()->get_left_entity_id() == entity_vec.at(i)->getId())
        {
            curr_left_entity = leftList.size();
            leftList << entity_vec.at(i)->getEntity()->getName();
        }
        else if (connection_view->getConnection()->get_right_entity_id() == entity_vec.at(i)->getId())
        {
            curr_right_entity = rightList.size();
            rightList << entity_vec.at(i)->getEntity()->getName();
        }
        else
        {
            leftList << entity_vec.at(i)->getEntity()->getName();
            rightList << entity_vec.at(i)->getEntity()->getName();
        }
    }
    left_entity->clear();
    right_entity->clear();

    left_entity->insertItems(0, leftList);
    right_entity->insertItems(0, rightList);

    inserted = true;

    left_entity->setCurrentIndex(curr_left_entity);
    right_entity->setCurrentIndex(curr_right_entity);

    update_left_lower_nums();
    update_right_lower_nums();

    update_left_upper_nums();
    update_right_upper_nums();
}

void CConnectionViewWidget::update_left_upper_nums()
{
    if (connection_view->getConnection()->getLeftMaxType() == CConnection::ONE)
    {
        left_upper_num->setCurrentIndex(0);
        if (right_upper_num->count() < 2)
        {
            right_upper_num->insertItem(1, tr("Many"));
            right_upper_num->insertItem(2, tr("Agregate"));
        }
        if (right_upper_num->count() < 3)
            right_upper_num->insertItem(2, tr("Agregate"));
    }
    else
    if (connection_view->getConnection()->getLeftMaxType() == CConnection::MANY)
    {
        left_upper_num->setCurrentIndex(1);
        if (right_upper_num->count() < 2)
            right_upper_num->insertItem(1, tr("Many"));
        else
            right_upper_num->removeItem(2);
    }
    else
    if (connection_view->getConnection()->getLeftMaxType() == CConnection::AGREGATE)
    {
        left_upper_num->setCurrentIndex(2);
        right_upper_num->removeItem(2);
        right_upper_num->removeItem(1);
    }
}

void CConnectionViewWidget::update_right_upper_nums()
{
    if (connection_view->getConnection()->getRightMaxType() == CConnection::ONE)
    {
        right_upper_num->setCurrentIndex(0);
        if (left_upper_num->count() < 2)
        {
            left_upper_num->insertItem(1, tr("Many"));
            left_upper_num->insertItem(2, tr("Agregate"));
        }
        if (left_upper_num->count() < 3)
            left_upper_num->insertItem(2, tr("Agregate"));
    }
    else
    if (connection_view->getConnection()->getRightMaxType() == CConnection::MANY)
    {
        right_upper_num->setCurrentIndex(1);
        if (left_upper_num->count() < 2)
            left_upper_num->insertItem(1, tr("Many"));
        else
            left_upper_num->removeItem(2);
    }
    else
    if (connection_view->getConnection()->getRightMaxType() == CConnection::AGREGATE)
    {
        right_upper_num->setCurrentIndex(2);
        left_upper_num->removeItem(2);
        left_upper_num->removeItem(1);
    }
}

void CConnectionViewWidget::update_left_lower_nums()
{
    if (connection_view->getConnection()->getRightMaxType() != CConnection::AGREGATE)
    {
        if (left_lower_num->count() < 2)
            left_lower_num->insertItem(1, tr("Optional"));

        if (connection_view->getConnection()->getLeftMinType() == CConnection::MANDATORY)
            left_lower_num->setCurrentIndex(0);
        else
            left_lower_num->setCurrentIndex(1);
    }
    else
    {
        left_lower_num->setCurrentIndex(0);
        left_lower_num->removeItem(1);
    }
}

void CConnectionViewWidget::update_right_lower_nums()
{
    if (connection_view->getConnection()->getLeftMaxType() != CConnection::AGREGATE)
    {
        if (right_lower_num->count() < 2)
            right_lower_num->insertItem(1, tr("Optional"));

        if (connection_view->getConnection()->getRightMinType() == CConnection::MANDATORY)
            right_lower_num->setCurrentIndex(0);
        else
            right_lower_num->setCurrentIndex(1);
    }
    else
    {
        right_lower_num->setCurrentIndex(0);
        right_lower_num->removeItem(1);
    }
}

void CConnectionViewWidget::left_entity_changed(QString _name)
{
    int left_id;
    QString old_name;

    for (int i = 0; i < entity_vec.size(); ++i)
    {
        if (_name == entity_vec.at(i)->getEntity()->getName())
        {
            left_id = entity_vec.at(i)->getId();
            break;
        }
    }

    for (int i = 0; i < entity_vec.size(); ++i)
    {
        if (connection_view->getConnection()->get_left_entity_id() == entity_vec.at(i)->getId())
        {
            old_name = entity_vec.at(i)->getEntity()->getName();
            break;
        }
    }

    if (left_id != connection_view->getConnection()->get_left_entity_id())
        if (inserted)
            parent->reconnect(connection_view->getId(), left_id,
                          connection_view->getConnection()->get_right_entity_id());

    if (inserted)
    {
        for (int i = 0; i < right_entity->count(); ++i)
            if (right_entity->itemText(i) == _name)
                right_entity->setItemText(i, old_name);
    }

    parent->update();
}

void CConnectionViewWidget::right_entity_changed(QString _name)
{
    int right_id;
    QString old_name;

    for (int i = 0; i < entity_vec.size(); ++i)
    {
        if (_name == entity_vec.at(i)->getEntity()->getName())
        {
            right_id = entity_vec.at(i)->getId();
            break;
        }
    }

    for (int i = 0; i < entity_vec.size(); ++i)
    {
        if (connection_view->getConnection()->get_right_entity_id() == entity_vec.at(i)->getId())
        {
            old_name = entity_vec.at(i)->getEntity()->getName();
            break;
        }
    }

    if (right_id != connection_view->getConnection()->get_right_entity_id())
        if (inserted)
            parent->reconnect(connection_view->getId(), connection_view->getConnection()->get_left_entity_id(),
                          right_id);

    if (inserted)
    {
        for (int i = 0; i < left_entity->count(); ++i)
            if (left_entity->itemText(i) == _name)
                left_entity->setItemText(i, old_name);
    }

    parent->update();
}

void CConnectionViewWidget::left_lower_num_changed(int ind)
{
    if (ind == 0)
        connection_view->getConnection()->setLeftMinType(CConnection::MANDATORY);
    else
        connection_view->getConnection()->setLeftMinType(CConnection::NONE);
    connection_view->update_state();
    parent->update();

    update_left_lower_nums();
}

void CConnectionViewWidget::right_lower_num_changed(int ind)
{
    if (ind == 0)
        connection_view->getConnection()->setRightMinType(CConnection::MANDATORY);
    else
        connection_view->getConnection()->setRightMinType(CConnection::NONE);
    connection_view->update_state();
    parent->update();

    update_right_lower_nums();
}

void CConnectionViewWidget::left_upper_num_changed(int ind)
{
    if (ind == 0)
        connection_view->getConnection()->setLeftMaxType(CConnection::ONE);
    if (ind == 1)
        connection_view->getConnection()->setLeftMaxType(CConnection::MANY);
    if (ind == 2)
        connection_view->getConnection()->setLeftMaxType(CConnection::AGREGATE);
    connection_view->update_state();
    parent->update();

    update_left_upper_nums();
    update_right_lower_nums();
}

void CConnectionViewWidget::right_upper_num_changed(int ind)
{
    if (ind == 0)
        connection_view->getConnection()->setRightMaxType(CConnection::ONE);
    if (ind == 1)
        connection_view->getConnection()->setRightMaxType(CConnection::MANY);
    if (ind == 2)
        connection_view->getConnection()->setRightMaxType(CConnection::AGREGATE);
    connection_view->update_state();
    parent->update();

    update_right_upper_nums();
    update_left_lower_nums();
}

