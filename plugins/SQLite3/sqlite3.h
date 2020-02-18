#ifndef POSTGRESQL_H
#define POSTGRESQL_H

#include <cplugininterface.h>

class SQLite : public CPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "CPluginInterface/Plugin/1.0")
    Q_INTERFACES(CPluginInterface)

public:
    QString name() override;
    QString version() override;
    QString query(IDataModel *dataModel) override;
};

#endif // POSTGRESQL_H
