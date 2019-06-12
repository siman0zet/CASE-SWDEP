#ifndef COBJECT_H
#define COBJECT_H

#include <QString>

class CObject
{
protected:
    int _id;
    QString _name;
    static const int USER_TYPE = 65536;

public:
    enum { Type = USER_TYPE};

    explicit CObject(int id);
    virtual ~CObject() {}
    int id() const;
    virtual int type() const;
    QString name() const;
    void setName(const QString &name);
};

#endif // COBJECT_H
