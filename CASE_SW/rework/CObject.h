#ifndef COBJECT_H
#define COBJECT_H

class CObject
{
protected:
    int _id;
    static const int USER_TYPE = 65536;

public:
    enum { Type = USER_TYPE};

    explicit CObject(int id);
    virtual ~CObject() {}
    int id() const;
    virtual int type();
};

#endif // COBJECT_H
