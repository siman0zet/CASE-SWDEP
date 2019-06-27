#ifndef ITABLE_H
#define ITABLE_H


#include "CObject.h"

class CRow;

class ITable : public CObject
{
public:
    explicit ITable(const QString &name) :
        CObject(name)
    { }

    virtual QList<CRow *> rows() const = 0;
};

#endif // ITABLE_H
