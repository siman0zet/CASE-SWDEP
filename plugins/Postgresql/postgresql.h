#ifndef POSTGRESQL_H
#define POSTGRESQL_H

//#include <../../src/CASE_SW/CPluginInterface.h>
#include <CPluginInterface.h>

class Postgresql : public CPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "CPluginInterface/Plugin/1.0")
    Q_INTERFACES(CPluginInterface)

public:
    Postgresql();

    QString getName();
    QString getVersion();
    QString getQuery(CDataModel *model);

    ~Postgresql() {}
};

#endif // POSTGRESQL_H
