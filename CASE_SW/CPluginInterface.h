#ifndef CPLUGININTERFACE_H
#define CPLUGININTERFACE_H

#include <QString>
#include <QObject>

#include <QtCore/qplugin.h>
#include <QtPlugin>

#include "model/IDataModel.h"
#include "model/ITable.h"
#include "model/IRow.h"
#include "model/CDataModel.h"

class CPluginInterface : public QObject
{
public:   
    virtual QString name() = 0;
    virtual QString version() = 0;
    virtual QString query(IDataModel *dataModel) = 0;

protected:
    IDataModel *_dataModel;
    ITable *_table;
    IRow *_row;

};

QT_BEGIN_NAMESPACE

Q_DECLARE_INTERFACE(CPluginInterface, "CPluginInterface/Plugin/1.0")

QT_END_NAMESPACE

#endif // CPLUGININTERFACE_H
