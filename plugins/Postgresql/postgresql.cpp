#include "postgresql.h"

Postgresql::Postgresql()
{
}

QString Postgresql::getVersion()
{
    return "1.0";
}

QString Postgresql::getName()
{
    return "Postgresql";
}

QString Postgresql::getQuery(CDataModel *model)
{
        QVector<CEntity*> tables = model->getTables();
        QVector<CEntity*> tablesf;
        QString script = "";

        int rep_cntr = 0;

        for(int i = 0; tables.size(); i++)
        {
            if (i >= tables.size())
                i = 0;
            if (rep_cntr == tables.size())
                return QString("Looped foreign keys error!");

            bool contin = false;
            for (int j = 0; j < tables[i]->getForeignRowCount(); ++j)
            {
                int id = tables[i]->getForeignRow(j)->getForeignTable();

                for (int k = 0; k < tables.size(); ++k)
                {
                    if (tables[k]->getId() == id)
                    {
                        ++rep_cntr;
                        contin = true;
                        break;
                    }
                }
                if (contin)
                    break;
            }

            if (contin)
                continue;

            if (tables[i]->getForeignRowCount() + tables[i]->getRowCount() == 0)
                continue;
            script = script + "CREATE TABLE " + tables[i]->getName() + " (";
            for(int j = 0; j < tables[i]->getRowCount(); j++)
            {
                if(j == tables[i]->getRowCount() + tables[i]->getForeignRowCount() - 1)
                {
                    script = script + tables[i]->getRow(j)->getName() + " " + tables[i]->getRow(j)->getStringType();
                    if (tables[i]->getRow(j)->isNotNull())
                    {
                        if(!tables[i]->getRow(j)->isPrimaryKey())
                            script = script + " NOT NULL";
                    }
                    if (tables[i]->getRow(j)->isUnique())
                    {
                        bool is_ung = false;

                        QVector<QVector<int> > uns = tables[i]->getComplexUniques();

                        for (int k = 0; k < uns.size(); ++k)
                        {
                            for (int z = 0; z < uns[k].size(); ++z)
                            {
                                if (uns[k][z] == j)
                                {
                                    is_ung = true;
                                    break;
                                }
                            }

                            if (is_ung)
                                break;
                        }

                        if(!tables[i]->getRow(j)->isPrimaryKey() && !is_ung)
                            script = script + " UNIQUE";
                    }

                }
                else
                {
                    script = script + tables[i]->getRow(j)->getName() + " " + tables[i]->getRow(j)->getStringType();
                    if (tables[i]->getRow(j)->isNotNull())
                    {
                        if(!tables[i]->getRow(j)->isPrimaryKey())
                            script = script + " NOT NULL";
                    }
                    if (tables[i]->getRow(j)->isUnique())
                    {
                        bool is_ung = false;

                        QVector<QVector<int> > uns = tables[i]->getComplexUniques();

                        for (int k = 0; k < uns.size(); ++k)
                        {
                            for (int z = 0; z < uns[k].size(); ++z)
                            {
                                if (uns[k][z] == j)
                                {
                                    is_ung = true;
                                    break;
                                }
                            }

                            if (is_ung)
                                break;
                        }

                        if(!tables[i]->getRow(j)->isPrimaryKey() && !is_ung)
                            script = script + " UNIQUE";
                    }
                    script = script + ",";
                }  
            }

            for(int j = 0; j < tables[i]->getForeignRowCount(); j++)
            {
                CEntity *table = NULL;
                for (int k = 0; k < tablesf.size(); ++k)
                {
                    if (tablesf[k]->getId() == tables[i]->getForeignRow(j)->getForeignTable())
                        table = tablesf[k];
                }

                QString attr = "!PRIMARY KEY IS NOT FOUND!";
                if (table)
                {
                    if (tables[i]->getForeignRow(j)->getForeignRowID() < table->getRowCount())
                    {
                        attr = table->getRow(tables[i]->getForeignRow(j)->getForeignRowID())->getName();
                        tables[i]->getForeignRow(j)->setForeignRowName(attr);
                    }
                    else
                    if (tables[i]->getForeignRow(j)->getForeignRowID() < table->getRowCount() + table->getForeignRowCount())
                    {
                        attr = table->getForeignRow(
                                    tables[i]->getForeignRow(j)->getForeignRowID() - table->getRowCount())->getName();
                        tables[i]->getForeignRow(j)->setForeignRowName(attr);
                    }
                }

                attr = tables[i]->getForeignRow(j)->getName() + " " + tables[i]->getForeignRow(j)->getStringType();

                if (tables[i]->getForeignRow(j)->isNotNull())
                {
                    if(!tables[i]->getForeignRow(j)->isPrimaryKey())
                        attr += " NOT NULL";
                }
                if (tables[i]->getForeignRow(j)->isUnique())
                {
                    if(!tables[i]->getForeignRow(j)->isPrimaryKey())
                        attr += " UNIQUE";
                }

                if (j == tables[i]->getForeignRowCount() - 1)
                {
                    if (table)
                        script = script + " " + attr +
                            " REFERENCES " + table->getName() + "(" + tables[i]->getForeignRow(j)->getForeignRowName() + ")" +
                            + " ON DELETE CASCADE ON UPDATE CASCADE";
                    else script = script + " " + attr +
                            " REFERENCES " + "NOT FOUND" + "(" + "NOT FOUND" + ")" +
                            + " ON DELETE CASCADE ON UPDATE CASCADE";
                }
                else
                {
                    if (table)
                        script = script + " " + attr +
                            " REFERENCES " + table->getName() + "(" + tables[i]->getForeignRow(j)->getForeignRowName() + ")"
                                + " ON DELETE CASCADE ON UPDATE CASCADE";
                    else script = script + " " + attr +
                            " REFERENCES " + "NOT FOUND"  + "(" + "NOT FOUND" + ")" +
                            " ON DELETE CASCADE ON UPDATE CASCADE";
                    script = script + ",";
                }
            }

            QVector<QVector<int> > uns = tables[i]->getComplexUniques();
            for (int k = 0; k < uns.size(); ++k)
            {
                script = script + ", UNIQUE(";
                for (int z = 0; z < uns[k].size(); ++z)
                {
                    script += tables[i]->getRow(uns[k][z])->getName();
                    if (z != uns[k].size()-1)
                        script += ", ";
                }
                script = script + ")";
            }

            script = script + ", PRIMARY KEY(";
            for (int attr = 0; attr < tables[i]->getRowCount(); attr++)
            {
                if(tables[i]->getRow(attr)->isPrimaryKey())
                {
                    script = script + tables[i]->getRow(attr)->getName()+',';
                }
            }
            for (int attr = 0; attr < tables[i]->getForeignRowCount(); attr++)
            {
                if(tables[i]->getForeignRow(attr)->isPrimaryKey())
                {
                    script = script + tables[i]->getForeignRow(attr)->getForeignRowName()+',';
                }
            }
            script.resize(script.size()-1);
            script += ")";
            script += ");\n";

            rep_cntr = 0;
            tablesf.append(tables.takeAt(i));
            --i;
        }

        return script;
}
