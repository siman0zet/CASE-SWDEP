#ifndef MYSQL_H
#define MYSQL_H

#include <CPluginInterface.h>

class MySQL : public CPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "CPluginInterface/Plugin/1.0")
    Q_INTERFACES(CPluginInterface)

public:
    QString name() override;
    QString version() override;
    QString query(IDataModel *dataModel) override;
};

#endif // MYSQL_H
