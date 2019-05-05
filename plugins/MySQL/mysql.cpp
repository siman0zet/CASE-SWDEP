#include "mysql.h"


MySQL::MySQL()
{
}

QString MySQL::getName()
{
    return "MySQL";
}

QString MySQL::getVersion()
{
    return "0.3";
}

QString MySQL::getQuery(CDataModel *model)
{
    QVector<CEntity*> tables = model->getTables();
    QString script = "";

    /*
     * CREATE TABLE [IF NOT EXIST] table_name(
     *     column_name data_type(length) [NOT NULL],
     *     PRIMARY KEY (column_name)
     * ) ENGINE = InnoDB;
     */

    for(int i = 0; i < tables.size(); i++)
    {
        QString columnList = "";
        QString primaryKey = "";

        for (int j = 0; j < tables[i]->getRowCount(); j++)
        {
            QString indicators = "";
            if(tables[i]->getRow(j)->isNotNull() && !(tables[i]->getRow(j)->isPrimaryKey()))
                indicators += " NOT NULL";

            if(tables[i]->getRow(j)->isPrimaryKey())
                primaryKey += QString("`%1`, ")
                        .arg(tables[i]->getRow(j)->getName());

            columnList += QString("    `%1` %2%3,\n")
                    .arg(tables[i]->getRow(j)->getName())
                    .arg(tables[i]->getRow(j)->getStringType())
                    .arg(indicators);
        }
        primaryKey.remove(primaryKey.size() - 2, 2);
        if(columnList != "" || primaryKey != "")
        {
            script += QString("CREATE TABLE IF NOT EXIST `%1`(\n"
                      "%2"
                      "    PRIMARY KEY (%3)\n"
                      ") ENGINE = InnoDB;\n\n")
                    .arg(tables[i]->getName())
                    .arg(columnList)
                    .arg(primaryKey);
        }
    }

    return script;
}
