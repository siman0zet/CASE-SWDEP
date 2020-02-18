#include "sqlite3.h"

QString SQLite::version()
{
    return "3.28";
}

QString SQLite::name()
{
    return "SQLite";
}

QString SQLite::query(IDataModel *dataModel)
{
    _dataModel = dataModel;
    QString script = "";

    foreach (CTable *table, _dataModel->tablesSortedByReference()) {
        _table = (ITable *)table;
        if(_table->foreingRows().size() != 0)
        {
            script += "PRAGMA foreign_keys = ON;\n\n";
            break;
        }
    }

    /*
     * CREATE TABLE [IF NOT EXIST] table_name(
     *     column_name data_type(length) [NOT NULL] [UNIQUE],
     *     FOREIGN KEY (column_name1, ... column_nameN)
     *     REFENCES table_name (table_column_name1, ... table_column_nameN)
     *     ON DELETE action
     *     ON UPDATE action
     *     PRIMARY KEY (column_name)
     * );
     */

    foreach (CTable *table, _dataModel->tablesSortedByReference()) {
        _table = (ITable *)table;
        QString tableBody = "";
        QString primaryKey = "";

        foreach (CRow *row, _table->rows()) {
            _row = (IRow *)row;
            QString constraint = "";
            if(_row->unique() && !(_row->primaryKey()))
                constraint += " UNIQUE";
            if(_row->notNull() && !(_row->primaryKey()))
                constraint += " NOT NULL";
            if(_row->primaryKey())
                primaryKey += QString("%1, ")
                        .arg(_row->name());
            tableBody += QString("%1 %2%3, ")
                    .arg(_row->name())
                    .arg(_row->typeAsString())
                    .arg(constraint);
        }
        // TABLE | (FOREIGN ROW, ROW)
        QMap<QString, QList<QPair<QString, QString>>> fRowsToTables;
        foreach (CForeignRow *foreignRow, _table->foreingRows()) {
            _foreignRow = (IForeingRow *)foreignRow;
            tableBody += QString("%1 %2, ")
                    .arg(_foreignRow->name())
                    .arg(_foreignRow->typeAsString());
            if(_foreignRow->primaryKey())
                primaryKey += QString("%1, ")
                        .arg(_foreignRow->name());
            if(fRowsToTables.contains(_foreignRow->tableName()))
            {
                QPair<QString, QString> pair(_foreignRow->name()
                                             ,_foreignRow->tableRowName());
                QList<QPair<QString, QString>> list;
                list = fRowsToTables.value(_foreignRow->tableName());
                list.append(pair);
                fRowsToTables.insert(_foreignRow->tableName(), list);
            }
            else
            {
                QPair<QString, QString> pair(_foreignRow->name()
                                             ,_foreignRow->tableRowName());
                QList<QPair<QString, QString>> list;
                list.append(pair);
                fRowsToTables.insert(_foreignRow->tableName(), list);
            }
        }
        tableBody.remove(tableBody.size() - 2, 2);
        if(primaryKey != "")
        {
            primaryKey.remove(primaryKey.size() - 2, 2);
            tableBody += QString(", PRIMARY KEY (%1)")
                    .arg(primaryKey);
        }
        if(!fRowsToTables.isEmpty())
        {
            tableBody += ", ";
            for(int i = 0; i < fRowsToTables.size(); i++)
            {
                QString fRowList, tRowList;
                for(int j = 0; j < fRowsToTables.values().at(i).size(); j++)
                {
                    fRowList += fRowsToTables.values().at(i).at(j).first + ", ";
                    tRowList += fRowsToTables.values().at(i).at(j).second + ", ";
                }
                fRowList.remove(fRowList.size() - 2, 2);
                tRowList.remove(tRowList.size() - 2, 2);
                tableBody += QString(" FOREIGN KEY (%3)"
                                     " REFERENCES %1 (%2)"
                                     " ON DELETE RESTRICT"
                                     " ON UPDATE RESTRICT, ")
                        .arg(fRowsToTables.keys().at(i))
                        .arg(tRowList)
                        .arg(fRowList);
            }
            tableBody.remove(tableBody.size() - 2, 2);
        }
        if(tableBody != "")
        {
            script += QString("CREATE TABLE IF NOT EXIST %1 ("
                      "%2"
                      ");\n\n")
                    .arg(_table->name())
                    .arg(tableBody);
        }
    }
    return script;
}
