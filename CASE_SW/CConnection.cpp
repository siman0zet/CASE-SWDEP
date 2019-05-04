#include "CConnection.h"

CConnection::CConnection(int _id, int entity_id_1, int entity_id_2)
{
    id = _id;
    left_id = entity_id_1;
    right_id = entity_id_2;

    left_upper_num = ONE;
    right_upper_num = ONE;
    left_lower_num = NONE;
    right_lower_num = NONE;
}

CConnection::~CConnection()
{

}

void CConnection::setLeftMaxType(CConnection::CONNECTION_MAX_TYPE _type)
{
    if (_type != ONE && right_upper_num == AGREGATE || _type == AGREGATE && right_upper_num == MANY)
        left_upper_num = ONE;
    else
        left_upper_num = _type;
}

void CConnection::setRightMaxType(CConnection::CONNECTION_MAX_TYPE _type)
{
    if (_type != ONE && left_upper_num == AGREGATE || _type == AGREGATE && left_upper_num == MANY)
        right_upper_num = ONE;
    else
        right_upper_num = _type;
}

void CConnection::setLeftMinType(CConnection::CONNECTION_MIN_TYPE _type)
{
    left_lower_num = _type;
}

void CConnection::setRightMinType(CConnection::CONNECTION_MIN_TYPE _type)
{
    right_lower_num = _type;
}

int CConnection::get_left_entity_id()
{
    return left_id;
}

int CConnection::get_right_entity_id()
{
    return right_id;
}

CConnection::CONNECTION_MAX_TYPE CConnection::getLeftMaxType()
{
    return left_upper_num;
}

CConnection::CONNECTION_MAX_TYPE CConnection::getRightMaxType()
{
    return right_upper_num;
}

CConnection::CONNECTION_MIN_TYPE CConnection::getLeftMinType()
{
    return left_lower_num;
}

CConnection::CONNECTION_MIN_TYPE CConnection::getRightMinType()
{
    return right_lower_num;
}

int CConnection::getId() const
{
    return id;
}

void CConnection::setConnection(int entity_id_1, int entity_id_2)
{
    left_id = entity_id_1;
    right_id = entity_id_2;
}
