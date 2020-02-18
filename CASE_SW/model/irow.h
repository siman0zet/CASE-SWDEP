#ifndef IROW_H
#define IROW_H

class QString;

class IRow
{
public:
    virtual QString name() const = 0;
    virtual QString typeAsString() const = 0;
    virtual bool primaryKey() const = 0;
    virtual bool notNull() const = 0;
    virtual bool unique() const = 0;
};

#endif // IROW_H
