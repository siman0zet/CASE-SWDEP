#ifndef CPLUGININTERFACE_H
#define CPLUGININTERFACE_H

#include <QString>
#include <QObject>

#include <QtCore/qplugin.h>
#include <QtPlugin>

#include "model/idatamodel.h"
#include "model/itable.h"
#include "model/irow.h"
#include "model/iforeingrow.h"
#include "model/cdatamodel.h"

class CPluginInterface : public QObject
{
public:
    CPluginInterface() :
        _dataModel(0),
        _table(0),
        _row(0),
        _foreignRow(0)
    {

    }

    virtual QString name() = 0;
    virtual QString version() = 0;
    virtual QString query(IDataModel *dataModel) = 0;

protected:
    IDataModel *_dataModel;
    ITable *_table;
    IRow *_row;
    IForeingRow *_foreignRow;

};

QT_BEGIN_NAMESPACE

Q_DECLARE_INTERFACE(CPluginInterface, "CPluginInterface/Plugin/1.0")

QT_END_NAMESPACE

#endif // CPLUGININTERFACE_H
