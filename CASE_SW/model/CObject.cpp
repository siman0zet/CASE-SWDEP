#include "CObject.h"

CObject::CObject(int id) :
    _id(id),
    _name("")
{
    
}

int CObject::id() const
{
    return _id;
}

int CObject::type() const
{
    return Type;
}

QString CObject::name() const
{
    return _name;
}

void CObject::setName(const QString &name)
{
    _name = name;
}
