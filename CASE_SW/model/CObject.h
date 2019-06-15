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
    virtual QString exportDataToText() const;
    virtual void importFromTextStream(QTextStream &) {}
    QString name() const;
    void setName(const QString &name);
};

#endif // COBJECT_H
