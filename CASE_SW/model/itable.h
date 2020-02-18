#ifndef ITABLE_H
#define ITABLE_H


#include "cobject.h"

class CRow;
class CForeignRow;

class ITable : public CObject
{
public:
    explicit ITable(const QString &name) :
        CObject(name)
    { }

    virtual QList<CRow *> rows() const = 0;
    virtual QList<CForeignRow *> foreingRows() const = 0;
};

#endif // ITABLE_H
