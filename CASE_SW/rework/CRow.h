#ifndef CROW_H
#define CROW_H

#include <QString>



class CRow
{
public:
    enum DATA_TYPE
    {
        INTEGER,
        FLOAT,
        CHAR,
        BOOLEAN,
        DATE,
        BLOB
    };

    explicit CRow(const QString &name);
    CRow(const QString &name,
         DATA_TYPE type = CRow::INTEGER,
         int size = -1,
         bool primaryKey = false,
         bool notNull = false,
         bool unique = false);

    QString name() const;
    void setName(const QString &name);
    DATA_TYPE type() const;
    void setType(const DATA_TYPE &type);
    int size() const;
    void setSize(int size);
    bool primaryKey() const;
    void setPrimaryKey(bool primaryKey);
    bool notNull() const;
    void setNotNull(bool notNull);
    bool unique() const;
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
