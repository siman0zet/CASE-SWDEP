#ifndef CROW_H
#define CROW_H

#include "irow.h"
#include <QString>

class CRow : public IRow
{
public:
    enum DATA_TYPE
    {
        INTEGER,
        FLOAT,
        CHAR,
        VARCHAR,
        BOOLEAN,
        DATE,
        BLOB
    };

    explicit CRow(const QString &name);
    explicit CRow(const CRow *row);

    QString name() const override;
    void setName(const QString &name);
    DATA_TYPE type() const;
    QString typeAsString() const override;
    void setType(const DATA_TYPE &type);
    void setTypeFromString(QString type);
    int size() const;
    void setSize(int size);
    bool primaryKey() const override;
    void setPrimaryKey(bool primaryKey);
    bool notNull() const override;
    void setNotNull(bool notNull);
    bool unique() const override;
    void setUnique(bool unique);

private:
    QString _name;
    DATA_TYPE _type;
    int _size;

    bool _primaryKey;
    bool _notNull;
    bool _unique;
};

#endif // CROW_H
