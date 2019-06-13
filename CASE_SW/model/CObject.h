#ifndef COBJECT_H
#define COBJECT_H

#include <QString>

class CObject
{
protected:
    QString _name;
    static const int USER_TYPE = 65536;

public:
    enum { Type = USER_TYPE};

    explicit CObject(const QString &name);
    virtual ~CObject() {}
    virtual int type() const;
    QString name() const;
    void setName(const QString &name);
};

#endif // COBJECT_H
