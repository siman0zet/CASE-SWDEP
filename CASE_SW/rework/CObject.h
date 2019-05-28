#ifndef COBJECT_H
#define COBJECT_H


class CObject
{
public:
    explicit CObject(int id);
    int id() const;

protected:
    int _id;
};

#endif // COBJECT_H
