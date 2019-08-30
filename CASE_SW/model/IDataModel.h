#ifndef IDATAMODEL_H
#define IDATAMODEL_H

#include <QObject>

class CTable;

class IDataModel : public QObject
{
    Q_OBJECT

public:
    virtual QMap<QString, CTable *> tables() const = 0;
    virtual QList<CTable *> tablesSortedByReference() const = 0;

};
#endif // IDATAMODEL_H
