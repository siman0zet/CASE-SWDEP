#ifndef CPLUGININTERFACE_H
#define CPLUGININTERFACE_H

#include <QString>
#include <QObject>

#include <QtCore/qplugin.h>
#include <QtPlugin>

#include "model/CDataModel.h"

class CPluginInterface : public QObject
{
public:
    virtual QString getName() = 0;
    virtual QString getVersion() = 0;
    virtual QString getQuery(CDataModel *model) = 0;
    //virtual QVector<loaded_type> getTypes() = 0;
};

QT_BEGIN_NAMESPACE

Q_DECLARE_INTERFACE(CPluginInterface,"CPluginInterface/Plugin/1.0")

QT_END_NAMESPACE

#endif // CPLUGININTERFACE_H
