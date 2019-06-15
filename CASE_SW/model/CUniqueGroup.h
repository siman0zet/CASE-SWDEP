#ifndef CUNIQUEGROUP_H
#define CUNIQUEGROUP_H

#include <QList>

class QString;

class CUniqueGroup
{
public:
    explicit CUniqueGroup(const QString &name);
    explicit CUniqueGroup(const CUniqueGroup* uGroup);

    QString name() const;
    void setName(const QString &name);

    QList<QString> rows() const;
    void addRow(const QString &row);
    void removeRow(const QString &row);

    void setRows(const QList<QString> &rows);

private:
    QString _name;
    QList<QString> _rows;

};

#endif // CUNIQUEGROUP_H
