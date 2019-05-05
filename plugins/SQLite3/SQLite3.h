#ifndef POSTGRESQL_H
#define POSTGRESQL_H

#include <CPluginInterface.h>

class SQLite : public CPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "CPluginInterface/Plugin/1.0")
    Q_INTERFACES(CPluginInterface)

public:
    SQLite();

    QString getName() override;
    QString getVersion() override;
    QString getQuery(CDataModel *model) override;
};

#endif // POSTGRESQL_H
