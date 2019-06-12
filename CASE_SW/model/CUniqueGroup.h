#ifndef CUNIQUEGROUP_H
#define CUNIQUEGROUP_H

#include <QList>

class QString;

class CUniqueGroup
{
public:
    CUniqueGroup(QString name);

    QString name() const;
    void setName(const QString &name);

    QList<QString> rows() const;
    void addRow(const QString &row);
    void removeRow(const QString &row);

private:
    QString _name;
    QList<QString> _rows;

};

#endif // CUNIQUEGROUP_H
