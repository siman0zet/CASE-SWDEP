#ifndef IFOREINGROW_H
#define IFOREINGROW_H

class QString;

class IForeingRow
{
public:
    virtual QString name() const = 0;
    virtual QString typeAsString() const = 0;
    virtual QString tableName() const = 0;
    virtual QString tableRowName() const = 0;
    virtual bool primaryKey() const = 0;
};

#endif // IFOREINGROW_H
