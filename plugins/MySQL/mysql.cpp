#include "mysql.h"

QString MySQL::name()
{
    return "MySQL";
}

QString MySQL::version()
{
    return "8.0.13";
}

QString MySQL::query(IDataModel *dataModel)
{
    _dataModel = dataModel;
    QMap<QString, CTable *> tables = _dataModel->tables();
    QString script = "";

    /*
     * CREATE TABLE [IF NOT EXIST] table_name(
     *     column_name data_type(length) [NOT NULL],
     *     PRIMARY KEY (column_name)
     * ) ENGINE = InnoDB;
     */

    foreach (CTable *table, tables) {
        _table = (ITable *)table;
        QString columnList = "";
        QString primaryKey = "";

        foreach (CRow *row, _table->rows()) {
            _row = (IRow *)row;
            QString constraint = "";
            if(_row->notNull() && !(_row->primaryKey()))
                constraint += " NOT NULL";
            if(_row->primaryKey())
                primaryKey += QString("`%1`, ")
                        .arg(_row->name());
            columnList += QString("    `%1` %2%3,\n")
                    .arg(_row->name())
                    .arg(_row->typeAsString())
                    .arg(constraint);
        }
        if(primaryKey != "")
        {
            primaryKey.remove(primaryKey.size() - 2, 2);
            columnList += QString("    PRIMARY KEY (%1)\n")
                    .arg(primaryKey);
        }
        else
            columnList.remove(columnList.size() - 2, 1);

        if(columnList != "")
        {
            script += QString("CREATE TABLE IF NOT EXIST `%1` (\n"
                      "%2"
                      ");\n\n")
                    .arg(_table->name())
                    .arg(columnList)
                    .arg(primaryKey);
        }
    }
    return script;
}
