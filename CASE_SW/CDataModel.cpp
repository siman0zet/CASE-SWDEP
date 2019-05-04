#include "CDataModel.h"

//#include <CViewModel.h>

CDataModel::CDataModel(QApplication *_app): app(_app)
{
    last_connection_id = -1;
    last_entity_id = -1;
    is_valid = true;
    error_code = 0;
    isPDM = false;
}

CDataModel::CDataModel(CDataModel &rhs)
{
    app = rhs.app;

    is_valid = true;
    error_code = 0;

    isPDM = rhs.isPDM;
    for (int i = 0; i < rhs.entity_vec.size(); ++i)
    {
        entity_vec.append(new CEntity(*rhs.entity_vec.at(i)));
    }
    for (int i = 0; i < rhs.connection_vec.size(); ++i)
    {
        connection_vec.append(new CConnection(*rhs.connection_vec.at(i)));
    }

    last_entity_id = rhs.last_entity_id;
    last_connection_id = rhs.last_connection_id;

    view = rhs.view;
}

CDataModel::~CDataModel()
{

}

int CDataModel::addEntity()
{
    last_entity_id += 1;
    entity_vec.append(new CEntity(last_entity_id, app));
    return entity_vec.last()->id;
}

void CDataModel::remEntity(int id)
{
    for (int i = 0; i < entity_vec.size(); ++i)
        {
            if (entity_vec[i]->id == id)
            {
                for (int j = 0; j < connection_vec.size(); ++j)
                {
                    if (connection_vec[j]->left_id == id || connection_vec[j]->right_id == id)
                        return;
                }
                delete entity_vec.at(i);
                entity_vec.remove(i);
                return;
            }
    }
}

void CDataModel::remEntityCascade(int id)
{

}

CEntity *CDataModel::getEntity(int id)
{
    for (int i = 0; i < entity_vec.size(); ++i)
        if (entity_vec.at(i)->getId() == id)
            return entity_vec.at(i);

    return NULL;
}

int CDataModel::addConnection(int entity_id_1, int entity_id_2)
{
    if (entity_id_1 == entity_id_2)
        return -1;

    bool en1 = false, en2 = false;
    for (int i = 0; i < entity_vec.size(); ++i)
    {
        if (entity_vec.at(i)->getId() == entity_id_1)
            en1 = true;
        if (entity_vec.at(i)->getId() == entity_id_2)
            en2 = true;

        if (en1 && en2)
            break;
    }

    if (!en1 || !en2)
        return -1;

    last_connection_id += 1;
    connection_vec.append(new CConnection(last_connection_id, entity_id_1, entity_id_2));
    return connection_vec.last()->id;
}

void CDataModel::remConnection(int id)
{
    for (int i = 0; i < connection_vec.size(); ++i)
        {
            if (connection_vec[i]->id == id)
            {
                delete connection_vec.at(i);
                connection_vec.remove(i);
                return;
            }
        }
}

CConnection *CDataModel::getConnection(int id)
{
    for (int i = 0; i < connection_vec.size(); ++i)
        if (connection_vec.at(i)->getId() == id)
            return connection_vec.at(i);

    return NULL;
}

void CDataModel::reset()
{

}

void CDataModel::loadFromFile(QString file_name)
{

}

void CDataModel::saveInFile(QString file_name)
{

}

CDataModel *CDataModel::convertToPDM()
{
    CDataModel *PDM = new CDataModel(*this);

    PDM->isPDM = true;

    if (entity_vec.size() <= 0)
    {
        PDM->setValid(false);
        PDM->error_code = 2;
        return PDM;
    }
    /*if (!PrimaryKeysCheck())
    {
        PDM->setValid(false);
        PDM->error_code = 1;
        return PDM;
    }*/

    QVector<CConnection*> connections = PDM->connection_vec;
    QVector<CConnection*> primary_key_cons;


    for (int i = 0; i < connections.size(); ++i)
    {
        if (connections.at(i)->getLeftMaxType() == CConnection::ONE &&
                connections.at(i)->getRightMaxType() == CConnection::ONE)
            if (connections.at(i)->getLeftMinType() == CConnection::MANDATORY &&
                    connections.at(i)->getRightMinType() == CConnection::MANDATORY)
            {
                CEntity *en1, *en2;

                for (int j = 0; j < PDM->entity_vec.size(); ++j)
                {
                    if (PDM->entity_vec.at(j)->getId() == connections.at(i)->get_left_entity_id())
                        en1 = PDM->entity_vec.at(j);
                    if (PDM->entity_vec.at(j)->getId() == connections.at(i)->get_right_entity_id())
                        en2 = PDM->entity_vec.at(j);
                }

                for (int j = 0; j < connections.size(); ++j)
                {
                    if (j == i)
                        continue;

                    if (connections.at(j)->get_right_entity_id() == en2->getId())
                    {
                        PDM->reconnect(connections.at(j)->getId(), connections.at(j)->get_left_entity_id(), en1->getId());
                    }
                    else
                    if (connections.at(j)->get_left_entity_id() == en2->getId())
                    {
                        PDM->reconnect(connections.at(j)->getId(), en1->getId(), connections.at(j)->get_right_entity_id());
                    }
                }

                PDM->addRows(en1, en2);
                PDM->remConnection(connections.at(i)->getId());
                connections.takeAt(i);
                PDM->remEntity(en2->getId());
                --i;
            }
    }

    for (int i = 0; i < connections.size(); ++i)
    {
        bool is_pk = false;

        if (connections.at(i)->getLeftMaxType() == CConnection::AGREGATE
                || connections.at(i)->getRightMaxType() == CConnection::AGREGATE)
            is_pk = true;

        if (is_pk)
        {
            primary_key_cons.append(connections.takeAt(i));
            --i;
        }
    }

    bool loops_result = loop_check(primary_key_cons);

    while (primary_key_cons.size())
    {
        QVector<CConnection*> curr_cons_group;
        int prev_size;

        curr_cons_group.append(primary_key_cons.takeFirst());

        do
        {
            prev_size =  curr_cons_group.size();

            for (int i = 0; i < primary_key_cons.size(); ++i)
            {
                int conn_id = -1;
                bool left = true;

                for (int j = 0; j < curr_cons_group.size(); ++j)
                {
                    int id1, id2;
                    if (primary_key_cons.at(i)->getLeftMaxType() == CConnection::AGREGATE)
                        id1 = primary_key_cons.at(i)->get_right_entity_id();
                    else
                        id1 = primary_key_cons.at(i)->get_left_entity_id();

                    if (curr_cons_group.at(j)->getLeftMaxType() == CConnection::AGREGATE)
                        id2 = curr_cons_group.at(j)->get_left_entity_id();
                    else
                        id2 = curr_cons_group.at(j)->get_right_entity_id();

                    if (id1 == id2)
                    {
                        conn_id = j;
                        left = true;
                        break;
                    }

                    if (primary_key_cons.at(i)->getLeftMaxType() == CConnection::AGREGATE)
                        id1 = primary_key_cons.at(i)->get_left_entity_id();
                    else
                        id1 = primary_key_cons.at(i)->get_right_entity_id();

                    if (curr_cons_group.at(j)->getLeftMaxType() == CConnection::AGREGATE)
                        id2 = curr_cons_group.at(j)->get_right_entity_id();
                    else
                        id2 = curr_cons_group.at(j)->get_left_entity_id();

                    if (id1 == id2)
                    {
                        conn_id = j;
                        left = false;
                    }
                }

                if (conn_id != -1)
                {
                    if (left)
                    {
                        curr_cons_group.insert(conn_id, primary_key_cons.takeAt(i));
                        --i;
                    }
                    else
                    {
                        curr_cons_group.insert(conn_id+1, primary_key_cons.takeAt(i));
                        --i;
                    }
                }
            }
        }
        while (prev_size != curr_cons_group.size());

        for (int i = 0; i < curr_cons_group.size(); ++i)
        {
            CEntity *en1, *en2;

            if (curr_cons_group.at(i)->getLeftMaxType() == CConnection::AGREGATE)
                for (int j = 0; j < PDM->entity_vec.size(); ++j)
                {
                    if (PDM->entity_vec.at(j)->getId() == curr_cons_group.at(i)->get_left_entity_id())
                    {
                        en2 = PDM->entity_vec.at(j);
                    }
                    if (PDM->entity_vec.at(j)->getId() == curr_cons_group.at(i)->get_right_entity_id())
                    {
                        en1 = PDM->entity_vec.at(j);
                    }
                }
            else
                for (int j = 0; j < PDM->entity_vec.size(); ++j)
                {
                    if (PDM->entity_vec.at(j)->getId() == curr_cons_group.at(i)->get_left_entity_id())
                    {
                        en1 = PDM->entity_vec.at(j);
                    }
                    if (PDM->entity_vec.at(j)->getId() == curr_cons_group.at(i)->get_right_entity_id())
                    {
                        en2 = PDM->entity_vec.at(j);
                    }
                }

            if (curr_cons_group.at(i)->getLeftMaxType() == CConnection::AGREGATE)
            {
                curr_cons_group.at(i)->setRightMaxType(CConnection::PHYSICAL);
                curr_cons_group.at(i)->setLeftMaxType(CConnection::ONE);
            }
            else
            {
                curr_cons_group.at(i)->setRightMaxType(CConnection::ONE);
                curr_cons_group.at(i)->setLeftMaxType(CConnection::PHYSICAL);
            }

            curr_cons_group.at(i)->setLeftMinType(CConnection::EMPTY);
            curr_cons_group.at(i)->setRightMinType(CConnection::EMPTY);

            for (int j = 0; j < en1->getRowCount(); ++j)
            {
                if (en1->getRow(j)->isPrimaryKey())
                {
                    en2->addForeignRow(en1->getId(), j);
                    en2->getForeignRow(en2->getForeignRowCount() - 1)->setName(/*en1->name + */en1->getRow(j)->getName());
                    en2->getForeignRow(en2->getForeignRowCount() - 1)->setType(en1->getRow(j)->getType(),
                                                                               en1->getRow(j)->getCount());
                    en2->getForeignRow(en2->getForeignRowCount() - 1)->setPrimaryKey(true);
                    en2->getForeignRow(en2->getForeignRowCount() - 1)->setNotNull(true);
                    en2->getForeignRow(en2->getForeignRowCount() - 1)->setForeignName(en1->getName());
                }
            }
            for (int j = 0; j < en1->getForeignRowCount(); ++j)
            {
                if (en1->getForeignRow(j)->isPrimaryKey())
                {
                    en2->addForeignRow(en1->getId(), en1->getRowCount() + j);
                    en2->getForeignRow(en2->getForeignRowCount() - 1)->setName(/*en1->name + */en1->getForeignRow(j)->getName());
                    en2->getForeignRow(en2->getForeignRowCount() - 1)->setType(en1->getForeignRow(j)->getType(),
                                                                               en1->getForeignRow(j)->getCount());
                    en2->getForeignRow(en2->getForeignRowCount() - 1)->setPrimaryKey(true);
                    en2->getForeignRow(en2->getForeignRowCount() - 1)->setNotNull(true);
                    en2->getForeignRow(en2->getForeignRowCount() - 1)->setForeignName(en1->getName());
                }
            }
        }

        curr_cons_group.clear();
    }

    for (int i = 0; i < connections.size(); ++i)
    {
        CEntity *en1, *en2;

        for (int j = 0; j < PDM->entity_vec.size(); ++j)
        {
            if (connections.at(i)->get_left_entity_id() == PDM->entity_vec.at(j)->getId())
                en1 = PDM->entity_vec.at(j);
            if (connections.at(i)->get_right_entity_id() == PDM->entity_vec.at(j)->getId())
                en2 = PDM->entity_vec.at(j);
        }

        if (connections.at(i)->getLeftMaxType() == CConnection::ONE &&
                connections.at(i)->getRightMaxType() == CConnection::ONE)
        {
            if (connections.at(i)->getLeftMinType() == CConnection::NONE &&
                    connections.at(i)->getRightMinType() == CConnection::NONE)
            {
                PDM->addEntity();
                CEntity *en = PDM->entity_vec.last();

                en->setName(en1->getName() + en2->getName());

                PDM->addKeys(en, en1);
                PDM->addKeys(en, en2, false, true);

                PDM->addConnection(en1->getId(), en->getId());
                PDM->connection_vec.last()->setLeftMaxType(CConnection::PHYSICAL);
                PDM->connection_vec.last()->setRightMaxType(CConnection::ONE);
                PDM->connection_vec.last()->setLeftMinType(CConnection::EMPTY);
                PDM->connection_vec.last()->setRightMinType(CConnection::EMPTY);

                PDM->addConnection(en2->getId(), en->getId());
                PDM->connection_vec.last()->setLeftMaxType(CConnection::PHYSICAL);
                PDM->connection_vec.last()->setRightMaxType(CConnection::ONE);
                PDM->connection_vec.last()->setLeftMinType(CConnection::EMPTY);
                PDM->connection_vec.last()->setRightMinType(CConnection::EMPTY);

                PDM->remConnection(connections.at(i)->getId());
                connections.takeAt(i);
                --i;
            }
            else
            if (connections.at(i)->getLeftMinType() == CConnection::NONE &&
                    connections.at(i)->getRightMinType() == CConnection::MANDATORY)
            {
                PDM->addKeys(en1, en2, false);

                connections.at(i)->setLeftMaxType(CConnection::ONE);
                connections.at(i)->setRightMaxType(CConnection::PHYSICAL);
                connections.at(i)->setLeftMinType(CConnection::EMPTY);
                connections.at(i)->setRightMinType(CConnection::EMPTY);
            }
            else
            if (connections.at(i)->getLeftMinType() == CConnection::MANDATORY &&
                    connections.at(i)->getRightMinType() == CConnection::NONE)
            {
                PDM->addKeys(en2, en1, false);

                connections.at(i)->setLeftMaxType(CConnection::PHYSICAL);
                connections.at(i)->setRightMaxType(CConnection::ONE);
                connections.at(i)->setLeftMinType(CConnection::EMPTY);
                connections.at(i)->setRightMinType(CConnection::EMPTY);
            }
        }
        else
        if (connections.at(i)->getLeftMaxType() == CConnection::MANY &&
                connections.at(i)->getRightMaxType() == CConnection::MANY)
        {
            CEntity *en;

            PDM->addEntity();
            en = PDM->entity_vec.last();
            en->setName(en1->getName() + en2->getName());

            PDM->addKeys(en, en1);
            PDM->addKeys(en, en2);

            PDM->addConnection(en1->getId(), en->getId());
            PDM->connection_vec.last()->setLeftMaxType(CConnection::PHYSICAL);
            PDM->connection_vec.last()->setRightMaxType(CConnection::ONE);
            PDM->connection_vec.last()->setLeftMinType(CConnection::EMPTY);
            PDM->connection_vec.last()->setRightMinType(CConnection::EMPTY);

            PDM->addConnection(en2->getId(), en->getId());
            PDM->connection_vec.last()->setLeftMaxType(CConnection::PHYSICAL);
            PDM->connection_vec.last()->setRightMaxType(CConnection::ONE);
            PDM->connection_vec.last()->setLeftMinType(CConnection::EMPTY);
            PDM->connection_vec.last()->setRightMinType(CConnection::EMPTY);

            PDM->remConnection(connections.at(i)->getId());
            connections.takeAt(i);
            --i;
        }
        else
        if (connections.at(i)->getLeftMaxType() == CConnection::MANY &&
                connections.at(i)->getRightMaxType() == CConnection::ONE)
        {
            if ((connections.at(i)->getLeftMinType() == CConnection::MANDATORY &&
                 connections.at(i)->getRightMinType() == CConnection::MANDATORY) ||
                 (connections.at(i)->getLeftMinType() == CConnection::NONE &&
                  connections.at(i)->getRightMinType() == CConnection::MANDATORY))
            {
                PDM->addKeys(en1, en2, false);

                connections.at(i)->setLeftMaxType(CConnection::ONE);
                connections.at(i)->setRightMaxType(CConnection::PHYSICAL);
                connections.at(i)->setLeftMinType(CConnection::EMPTY);
                connections.at(i)->setRightMinType(CConnection::EMPTY);
            }
            else
            if ((connections.at(i)->getLeftMinType() == CConnection::NONE &&
                    connections.at(i)->getRightMinType() == CConnection::NONE) ||
                    (connections.at(i)->getLeftMinType() == CConnection::MANDATORY &&
                     connections.at(i)->getRightMinType() == CConnection::NONE))
            {
                PDM->addEntity();
                CEntity *en = PDM->entity_vec.last();

                en->setName(en1->getName() + en2->getName());

                PDM->addKeys(en, en1);
                PDM->addKeys(en, en2, false, true);

                PDM->addConnection(en1->getId(), en->getId());
                PDM->connection_vec.last()->setLeftMaxType(CConnection::PHYSICAL);
                PDM->connection_vec.last()->setRightMaxType(CConnection::ONE);
                PDM->connection_vec.last()->setLeftMinType(CConnection::EMPTY);
                PDM->connection_vec.last()->setRightMinType(CConnection::EMPTY);

                PDM->addConnection(en2->getId(), en->getId());
                PDM->connection_vec.last()->setLeftMaxType(CConnection::PHYSICAL);
                PDM->connection_vec.last()->setRightMaxType(CConnection::ONE);
                PDM->connection_vec.last()->setLeftMinType(CConnection::EMPTY);
                PDM->connection_vec.last()->setRightMinType(CConnection::EMPTY);

                PDM->remConnection(connections.at(i)->getId());
                connections.takeAt(i);
                --i;
            }
        }
        else
        if (connections.at(i)->getLeftMaxType() == CConnection::ONE &&
                connections.at(i)->getRightMaxType() == CConnection::MANY)
        {
            if ((connections.at(i)->getLeftMinType() == CConnection::MANDATORY &&
                    connections.at(i)->getRightMinType() == CConnection::MANDATORY) ||
                    (connections.at(i)->getLeftMinType() == CConnection::MANDATORY &&
                     connections.at(i)->getRightMinType() == CConnection::NONE))
            {
                PDM->addKeys(en2, en1, false);

                connections.at(i)->setLeftMaxType(CConnection::PHYSICAL);
                connections.at(i)->setRightMaxType(CConnection::ONE);
                connections.at(i)->setLeftMinType(CConnection::EMPTY);
                connections.at(i)->setRightMinType(CConnection::EMPTY);
            }
            else
            if ((connections.at(i)->getLeftMinType() == CConnection::NONE &&
                 connections.at(i)->getRightMinType() == CConnection::NONE) ||
                 (connections.at(i)->getLeftMinType() == CConnection::NONE &&
                  connections.at(i)->getRightMinType() == CConnection::MANDATORY))
            {
                PDM->addEntity();
                CEntity *en = PDM->entity_vec.last();

                en->setName(en1->getName() + en2->getName());

                PDM->addKeys(en, en2);
                PDM->addKeys(en, en1, false, true);

                PDM->addConnection(en1->getId(), en->getId());
                PDM->connection_vec.last()->setLeftMaxType(CConnection::PHYSICAL);
                PDM->connection_vec.last()->setRightMaxType(CConnection::ONE);
                PDM->connection_vec.last()->setLeftMinType(CConnection::EMPTY);
                PDM->connection_vec.last()->setRightMinType(CConnection::EMPTY);

                PDM->addConnection(en2->getId(), en->getId());
                PDM->connection_vec.last()->setLeftMaxType(CConnection::PHYSICAL);
                PDM->connection_vec.last()->setRightMaxType(CConnection::ONE);
                PDM->connection_vec.last()->setLeftMinType(CConnection::EMPTY);
                PDM->connection_vec.last()->setRightMinType(CConnection::EMPTY);

                PDM->remConnection(connections.at(i)->getId());
                connections.takeAt(i);
                --i;
            }
        }
    }

    for (int i = 0; i < PDM->entity_vec.size(); ++i)
    {
        PDM->entity_vec.at(i)->set_editable(false);
    }

    return PDM;
}

bool CDataModel::PrimaryKeysCheck()
{
    for (int i = 0; i < entity_vec.size(); ++i)
    {
        bool pk = false;
        for (int j = 0; j < entity_vec.at(i)->getRowCount(); ++j)
        {
            if (entity_vec.at(i)->getRow(j)->isPrimaryKey())
            {
                pk = true;
                break;
            }
        }

        if (!pk)
            return false;
    }

    return true;
}

QString CDataModel::makeScript()
{
    if (!isPDM)
        return QString();

    if (looping())
        return QString();

    QString script;

    QVector<CEntity*> basic_entityes, entityes = entity_vec;

    for (int i = 0; i < entityes.size(); ++i)
    {
        if (entityes.at(i)->getForeignRowCount() == 0)
        {
            basic_entityes.append(entityes.takeAt(i));
            --i;
        }
    }

    for (int i = 0; i < entityes.size(); ++i)
    {
        int pos = 0;
        for (int j = 0; j < entityes.at(i)->getForeignRowCount(); ++j)
        {
            for (int k = 0; k < entityes.size(); ++k)
            {
                if (entityes.at(k)->getId() == entityes.at(i)->getForeignRow(j)->getForeignTable())
                {
                    if (k > pos)
                        pos = k;
                    break;
                }
            }
        }

        if (pos > i)
        {
            CEntity *tmp = entityes.takeAt(i);
            entityes.insert(pos, tmp);
            --i;
        }
    }

    QString curr_str;

    for (int i = 0; i < basic_entityes.size(); ++i)
    {
        curr_str += "create table ";
        curr_str += basic_entityes.at(i)->getName();
        curr_str += " ( ";

        for (int j = 0; j < basic_entityes.at(i)->getRowCount(); ++j)
        {
            curr_str += basic_entityes.at(i)->getRow(j)->getName();

            if (basic_entityes.at(i)->getRow(j)->getType() == CRow::INTEGER)
            {
                curr_str += " integer ";
            }
            else
            if (basic_entityes.at(i)->getRow(j)->getType() == CRow::FLOAT)
            {
                curr_str += " float ";
            }
            else
            if (basic_entityes.at(i)->getRow(j)->getType() == CRow::CHAR)
            {
                if (basic_entityes.at(i)->getRow(j)->getCount() <= 0)
                {
                    curr_str += " char ";
                }
                else
                {
                    curr_str += " char(";

                    QString num;
                    num.setNum(basic_entityes.at(i)->getRow(j)->getCount());

                    curr_str += num;
                    curr_str += ") ";
                }
            }

            if (j != basic_entityes.at(i)->getRowCount() - 1)
                curr_str += " , ";
        }

        bool first = true;
        curr_str += " primary key (";
        for (int j = 0; j < basic_entityes.at(i)->getRowCount(); ++j)
        {
            if (basic_entityes.at(i)->getRow(j)->isPrimaryKey())
            {
                if (!first)
                {
                    curr_str += " , ";
                }
                first = false;
                curr_str += basic_entityes.at(i)->getRow(j)->getName();
            }
        }
        curr_str += ")";

        curr_str += ");\n";

        script += curr_str;
        curr_str.clear();
    }

    for (int i = 0; i < entityes.size(); ++i)
    {
        curr_str += "create table ";
        curr_str += entityes.at(i)->getName();
        curr_str += " ( ";

        for (int j = 0; j < entityes.at(i)->getRowCount(); ++j)
        {
            curr_str += entityes.at(i)->getRow(j)->getName();

            if (entityes.at(i)->getRow(j)->getType() == CRow::INTEGER)
            {
                curr_str += " integer ";
            }
            else
            if (entityes.at(i)->getRow(j)->getType() == CRow::FLOAT)
            {
                curr_str += " float ";
            }
            else
            if (entityes.at(i)->getRow(j)->getType() == CRow::CHAR)
            {
                if (entityes.at(i)->getRow(j)->getCount() <= 0)
                {
                    curr_str += " char ";
                }
                else
                {
                    curr_str += " char(";

                    QString num;
                    num.setNum(entityes.at(i)->getRow(j)->getCount());

                    curr_str += num;
                    curr_str += ") ";
                }
            }

            if (j != entityes.at(i)->getRowCount() + entityes.at(i)->getForeignRowCount() - 1)
                curr_str += " , ";
        }

        for (int j = 0; j < entityes.at(i)->getForeignRowCount(); ++j)
        {
            curr_str += entityes.at(i)->getForeignRow(j)->getName();

            if (entityes.at(i)->getForeignRow(j)->getType() == CRow::INTEGER)
            {
                curr_str += " integer ";
            }
            else
            if (entityes.at(i)->getForeignRow(j)->getType() == CRow::FLOAT)
            {
                curr_str += " float ";
            }
            else
            if (entityes.at(i)->getForeignRow(j)->getType() == CRow::CHAR)
            {
                if (entityes.at(i)->getForeignRow(j)->getCount() <= 0)
                {
                    curr_str += " char ";
                }
                else
                {
                    curr_str += " char(";

                    QString num;
                    num.setNum(entityes.at(i)->getForeignRow(j)->getCount());

                    curr_str += num;
                    curr_str += ") ";
                }
            }

            QString table;
            for (int k = 0; k < entity_vec.size(); ++k)
            {
                if (entity_vec.at(k)->getId() == entityes.at(i)->getForeignRow(j)->getForeignTable())
                {
                    table = entity_vec.at(k)->getName();
                    break;
                }
            }

            curr_str += " references from ";
            curr_str += table;
            curr_str += " ";

            if (j != entityes.at(i)->getForeignRowCount() - 1)
                curr_str += " , ";
        }
    }

    return script;
}

void CDataModel::reconnect(int conn_id, int entity_id_1, int entity_id_2)
{
    if (entity_id_1 == entity_id_2)
        return;

    CConnection* con = NULL;

    for (int i = 0; i < connection_vec.size(); ++i)
    {
        if (connection_vec.at(i)->getId() == conn_id)
        {
            con = connection_vec.at(i);
            break;
        }
    }

    if (!con)
        return;

    con->setConnection(entity_id_1, entity_id_2);
}

void CDataModel::setValid(bool flag)
{
    is_valid = flag;
}

bool CDataModel::isValid()
{
    return is_valid;
}

bool CDataModel::is_PDM()
{
    return isPDM;
}

int CDataModel::getErrorCode()
{
    return error_code;
}

void CDataModel::removeAll()
{
    for (int i = 0; i < connection_vec.size(); ++i)
        delete connection_vec.at(i);
    connection_vec.clear();
    for (int i = 0; i < entity_vec.size(); ++i)
        delete entity_vec.at(i);
    entity_vec.clear();
}



void CDataModel::setLastEntityId(int _id)
{
    last_entity_id = _id;
}

bool CDataModel::tearing_check(QVector<CConnection *> &vec)
{

}

bool CDataModel::loop_check(QVector<CConnection *> &vec)
{

}

bool CDataModel::looping()
{

}

void CDataModel::addKeys(CEntity *to, CEntity *from, bool isPK, bool isUNIQ, bool isNNULL)
{
    for (int j = 0; j < from->getRowCount(); ++j)
    {
        if (from->getRow(j)->isPrimaryKey())
        {
            to->addForeignRow(from->getId(), j);
            to->getForeignRow(to->getForeignRowCount() - 1)->setName(/*from->name + */from->getRow(j)->getName());
            to->getForeignRow(to->getForeignRowCount() - 1)->setType(from->getRow(j)->getType(),
                                                                       from->getRow(j)->getCount());
            to->getForeignRow(to->getForeignRowCount() - 1)->setUnique(isUNIQ);
            to->getForeignRow(to->getForeignRowCount() - 1)->setNotNull(isNNULL);
            to->getForeignRow(to->getForeignRowCount() - 1)->setPrimaryKey(isPK);
            to->getForeignRow(to->getForeignRowCount() - 1)->setForeignName(from->getName());
        }
    }
    for (int j = 0; j < from->getForeignRowCount(); ++j)
    {
        if (from->getForeignRow(j)->isPrimaryKey())
        {
            to->addForeignRow(from->getId(), from->getRowCount() + j);
            to->getForeignRow(to->getForeignRowCount() - 1)->setName(/*from->name + */from->getForeignRow(j)->getName());
            to->getForeignRow(to->getForeignRowCount() - 1)->setType(from->getForeignRow(j)->getType(),
                                                                       from->getForeignRow(j)->getCount());
            to->getForeignRow(to->getForeignRowCount() - 1)->setUnique(isUNIQ);
            to->getForeignRow(to->getForeignRowCount() - 1)->setNotNull(isNNULL);
            to->getForeignRow(to->getForeignRowCount() - 1)->setPrimaryKey(isPK);
            to->getForeignRow(to->getForeignRowCount() - 1)->setForeignName(from->getName());
        }
    }
}

void CDataModel::addRows(CEntity *to, CEntity *from)
{
    for (int j = 0; j < from->getRowCount(); ++j)
    {
            to->addRow();
            to->getRow(to->getRowCount() - 1)->setName(from->name + from->getRow(j)->getName());
            to->getRow(to->getRowCount() - 1)->setType(from->getRow(j)->getType(),
                                                                       from->getRow(j)->getCount());
            to->getRow(to->getRowCount() - 1)->setPrimaryKey(false);
            to->getRow(to->getRowCount() - 1)->setNotNull(from->getRow(j)->isNotNull());
            to->getRow(to->getRowCount() - 1)->setUnique(from->getRow(j)->isUnique());
    }
    for (int j = 0; j < from->getForeignRowCount(); ++j)
    {
            to->addRow();
            to->getRow(to->getRowCount() - 1)->setName(from->name + from->getForeignRow(j)->getName());
            to->getRow(to->getRowCount() - 1)->setType(from->getForeignRow(j)->getType(),
                                                                       from->getForeignRow(j)->getCount());
            to->getRow(to->getRowCount() - 1)->setPrimaryKey(false);
            to->getRow(to->getRowCount() - 1)->setNotNull(from->getForeignRow(j)->isNotNull());
            to->getRow(to->getRowCount() - 1)->setUnique(from->getForeignRow(j)->isUnique());
    }
}
