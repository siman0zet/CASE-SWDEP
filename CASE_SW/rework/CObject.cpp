#include "CObject.h"

CObject::CObject(int id) :
    _id(id)
{

}

int CObject::id() const
{
    return _id;
}
