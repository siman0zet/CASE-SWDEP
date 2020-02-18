#ifndef COBJECT_H
#define COBJECT_H

#include <QString>
class QTextStream;

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
    virtual QString exportDataToText() const = 0;
    virtual void importFromTextStream(QTextStream &) = 0;
    virtual QString name() const;
    virtual void setName(const QString &name);
};

#endif // COBJECT_H
