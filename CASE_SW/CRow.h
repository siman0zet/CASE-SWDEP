#ifndef CROW_H
#define CROW_H

/* Строка, запись, поле. Содержит флаги (Not null, Primary key) и тип. Поле count используется для
 * массивных типов, например char(8), 8 - count. */

#include <QString>
#include <QDebug>


class CRow
{
public:
    enum DATA_TYPE {INTEGER, FLOAT, CHAR, BOOLEAN, DATE, BLOB};

    struct loaded_type
    {
        QString type_name;
        CRow::DATA_TYPE type;
        int size;
    };

    CRow()
    {
        NotNull = false;
        PrimaryKey = false;
        unique = true;
        type = INTEGER;
        count = -1;
        is_loaded_typed = false;
    }

    ~CRow()
    {

    }

    void setNotNull(bool flag)
    {
        NotNull = flag;
    }

    void setPrimaryKey(bool flag)
    {
        PrimaryKey = flag;

        if (flag)
        {
            NotNull = true;
            unique = true;
        }
    }

    void setUnique(bool flag)
    {
        unique = flag;
    }

    void setName(QString _name)
    {
        name = _name;
    }

    void setType(CRow::DATA_TYPE _type, int _count)
    {
        is_loaded_typed = false;
        type = _type;

        if (_count > 0)
            count = _count;
        else
            count = 1;
    }

    void setLoadedType(loaded_type lt)
    {
        ltype = lt;
    }

    bool isNotNull() const
    {
        return NotNull;
    }

    bool isPrimaryKey() const
    {
        return PrimaryKey;
    }

    bool isUnique() const
    {
        return unique;
    }

    CRow::DATA_TYPE getType() const
    {
        if (is_loaded_typed)
        {
            return ltype.type;
        }
        return type;
    }

    int getCount() const
    {
        return count;
    }

    QString getName() const
    {
        return name;
    }

    QString getStringType()
    {
        if (is_loaded_typed)
        {
            if (ltype.size > 0)
            {
                QString str = " (";
                QString num;
                num.setNum(count);
                str += num;
                str += ")";
                return ltype.type_name + str;
            }
            else
                return ltype.type_name;
        }

        if (type == INTEGER)
            return "INTEGER";
        else
        if (type == FLOAT)
            return "FLOAT";
        else
        if (type == CHAR)
        {
            QString str = "CHAR (";
            QString num;
            num.setNum(count);
            str += num;
            str += ")";
            return str;
        }
        else
        if (type == BOOLEAN)
        {
            return "BOOLEAN";
        }
        else
        if (type == DATE)
        {
            qDebug() << "!!!";
            return "DATE";
        }
        else
        if (type == BLOB)
        {
            qDebug() << "???";
            return "BLOB";
        }
        return QString();
    }

protected:
    bool NotNull, PrimaryKey, unique;

    DATA_TYPE type;
    int count;

    QString name;

    loaded_type ltype;
    bool is_loaded_typed;
};

#endif // CROW_H
