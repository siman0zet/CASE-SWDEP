#include "CEntity.h"

#include <QApplication>

CEntity::CEntity(int _id, QApplication *_app): app(_app)
{
    id = _id;
    is_editable = true;
}

CEntity::CEntity(CEntity &rhs)
{
    is_editable = rhs.is_editable;
    id = rhs.id;
    name = rhs.name;
    row_vec = rhs.row_vec;
    foreign_row_vec = rhs.foreign_row_vec;
    app = rhs.app;

    CmplxUnique = rhs.CmplxUnique;
}

CEntity::~CEntity()
{

}

void CEntity::setName(QString _name)
{
    name = _name;
}

int CEntity::addRow() //check
{
    row_vec.append(CRow());
    return row_vec.size() - 1;
}

void CEntity::remRow(int i)
{
    row_vec.remove(i);
    for (int k = 0; k < CmplxUnique.size(); ++k)
    {
        for (int j = 0; j < CmplxUnique[k].size(); ++j)
        {
            if (i == CmplxUnique[k][j])
            {
                CmplxUnique[k].removeAt(j);
                --j;
            }
            else
            if (CmplxUnique[k][j] > i)
            {
                CmplxUnique[k][j] = CmplxUnique[k][j] - 1;
            }
        }
    }
}

int CEntity::rowCount(const QModelIndex &parent) const
{
    return getRowCount() + getForeignRowCount();
}

int CEntity::columnCount(const QModelIndex &parent) const
{
    int count = 5;

    if (this->getForeignRowCount() > 0)
        ++count;

    ++count;

    return  count;
}

QVariant CEntity::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
            return QVariant();

    bool is_size_field = true;

    if (index.row() < getForeignRowCount())
    {
        if (index.column() == 0)
            return foreign_row_vec.at(index.row()).getName();
        else
        if (index.column() == 1)
        {
            if (foreign_row_vec.at(index.row()).getType() == CRow::INTEGER)
                return QVariant("integer");
            else
            if (foreign_row_vec.at(index.row()).getType() == CRow::FLOAT)
                return QVariant("float");
            else
            if (foreign_row_vec.at(index.row()).getType() == CRow::CHAR)
            {
                QString rtrn("char ");
                QString num;
                num.setNum(foreign_row_vec.at(index.row()).getCount());

                return rtrn;
            }
            else
            if (foreign_row_vec.at(index.row()).getType() == CRow::BOOLEAN)
                return QVariant("boolean");
            else
            if (foreign_row_vec.at(index.row()).getType() == CRow::DATE)
                return QVariant("date");
            else
            if (foreign_row_vec.at(index.row()).getType() == CRow::BLOB)
                return QVariant("blob");
        }
        else
        if (index.column() == 2)
        {
                if (foreign_row_vec.at(index.row()).getType() == CRow::CHAR)
                    return foreign_row_vec.at(index.row()).getCount();
                else
                    return QVariant();
        }
        else
        if (index.column() == 3)
        {
                if (foreign_row_vec.at(index.row()).isPrimaryKey())
                    return true;
                else
                    return false;
        }
        else
        if (index.column() == 4)
        {
                if (foreign_row_vec.at(index.row()).isNotNull())
                    return true;
                else
                    return false;
        }
        else
        if (is_size_field && index.column() == 5)
        {
            return foreign_row_vec.at(index.row()).isUnique();
        }
        else
        if (index.column() == 6)
        {
            return foreign_row_vec.at(index.row()).getForeignName();
        }
        else return QVariant();
    }
    else
    {
        if (index.column() == 0)
            return row_vec.at(index.row() - getForeignRowCount()).getName();
        else
        if (index.column() == 1)
        {
            if (row_vec.at(index.row() - getForeignRowCount()).getType() == CRow::INTEGER)
                return QVariant("integer");
            else
            if (row_vec.at(index.row() - getForeignRowCount()).getType() == CRow::FLOAT)
                return QVariant("float");
            else
            if (row_vec.at(index.row() - getForeignRowCount()).getType() == CRow::CHAR)
            {
                QString rtrn("char ");
                QString num;
                num.setNum(row_vec.at(index.row() - getForeignRowCount()).getCount());

                return rtrn;
            }
            else
            if (row_vec.at(index.row() - getForeignRowCount()).getType() == CRow::BOOLEAN)
                return QVariant("boolean");
            else
            if (row_vec.at(index.row() - getForeignRowCount()).getType() == CRow::DATE)
                return QVariant("date");
            else
            if (row_vec.at(index.row() - getForeignRowCount()).getType() == CRow::BLOB)
                return QVariant("blob");
        }
        else
        if (index.column() == 2)
        {
            if (is_size_field)
            {
                if (row_vec.at(index.row() - getForeignRowCount()).getType() == CRow::CHAR)
                    return row_vec.at(index.row() - getForeignRowCount()).getCount();
                else
                    return QVariant();
            }
            else
            if (row_vec.at(index.row() - getForeignRowCount()).isPrimaryKey())
                return true;
            else
                return false;
        }
        else
        if (index.column() == 3)
        {
            if (is_size_field)
            {
                if (row_vec.at(index.row() - getForeignRowCount()).isPrimaryKey())
                    return true;
                else
                    return false;
            }
            else
            if (row_vec.at(index.row() - getForeignRowCount()).isNotNull())
                return true;
            else
                return false;
        }
        else
        if (index.column() == 4)
        {
            if (is_size_field)
            {
                if (row_vec.at(index.row() - getForeignRowCount()).isNotNull())
                    return true;
                else
                    return false;
            }
            else
                return QVariant();
        }
        else
        if (index.column() == 5)
        {
            return row_vec.at(index.row() - getForeignRowCount()).isUnique();
        }
        else return QVariant();
    }
}

QVariant CEntity::headerData(int section, Qt::Orientation orientation, int role) const
{
    bool is_size_field = true;

    if (orientation == Qt::Vertical)
        return QVariant();
    if (role != Qt::DisplayRole)
        return QVariant();

    if (section == 0)
    {
        if (app)
            return app->translate("Name", "Entity");
        return tr("Name");
    }
    else
    if (section == 1)
    {
        if (app)
            return app->translate("Type", "Entity");
        return tr("Type");
    }
    else
    if (section == 2)
    {
        if (is_size_field)
        {
            if (app)
                return app->translate("Count", "Entity");
            return tr("Count");
        }
        else
            return tr("Primary key");
    }
    else
    if (section == 3)
    {
        if (is_size_field)
        {
            if (app)
                return app->translate("Primary key", "Entity");
            return tr("Primary key");
        }
        else
            return tr("Not null");
    }
    else
    if (section == 4)
    {
        if (is_size_field)
        {
            if (app)
                return app->translate("Not null", "Entity");
            return tr("Not null");
        }
    }
    else
    if (section == 5)
    {
        if (app)
            return app->translate("Unique", "Entity");
        return tr("Unique");
    }
    else
    if (section == 6)
    {
        if (this->getForeignRowCount() > 0)
        {
            if (app)
                return app->translate("Foreign table", "Entity");
            return tr("Foreign table");
        }
    }
    return QVariant();
}

bool CEntity::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(createIndex(rowCount(), 0), rowCount(), rowCount() + count);
    while (count)
    {
        --count;
        addRow();
    }

    endInsertRows();
}

bool CEntity::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool is_size_field = true;

    CRow* row;
    if (index.row() < this->getRowCount())
        row = getRow(index.row());
    else
        row = getForeignRow(index.row() - getRowCount());

    if (index.column() == 0)
    {
        if (value.toString().length() <= 0)
            return false;

        if (value.toString().indexOf(' ') != -1)
            return false;

        row->setName(value.toString());
    }
    else
    if (index.column() == 1)
    {
        QString str = value.toString();

        if (QString::compare(str, "integer", Qt::CaseInsensitive) == 0)
        {
            row->setType(CRow::INTEGER, -1);
        }
        if (QString::compare(str, "float", Qt::CaseInsensitive) == 0)
        {
            row->setType(CRow::FLOAT, -1);
        }
        if (QString::compare(str, "char", Qt::CaseInsensitive) == 0)
        {
            row->setType(CRow::CHAR, 1);
        }
        if (QString::compare(str, "boolean", Qt::CaseInsensitive) == 0)
        {
            row->setType(CRow::BOOLEAN, -1);
        }
        if (QString::compare(str, "date", Qt::CaseInsensitive) == 0)
        {
            row->setType(CRow::DATE, -1);
        }
        if (QString::compare(str, "blob", Qt::CaseInsensitive) == 0)
        {
            row->setType(CRow::BLOB, -1);
        }

        bool is_size_field1 = false;
        for (int i = 0; i < getRowCount(); ++i)
        {
            if (this->getRow(i)->getType() == CRow::CHAR)
            {
                is_size_field1 = true;
                break;
            }
        }

        for (int i = 0; i < getForeignRowCount(); ++i)
        {
            if (this->getForeignRow(i)->getType() == CRow::CHAR)
            {
                is_size_field1 = true;
                break;
            }
        }

        if (is_size_field != is_size_field1)
        {
            emit headerDataChanged(Qt::Horizontal, 0, 3);
        }
    }
    else
    if (index.column() == 2)
    {
        if (is_size_field)
        {
            if (row->getType() != CRow::CHAR)
                return false;
            else
                row->setType(CRow::CHAR, value.toInt());
        }
        else
            row->setPrimaryKey(value.toBool());
    }
    else
    if (index.column() == 3)
    {
        if (is_size_field)
            row->setPrimaryKey(value.toBool());
        else
            row->setNotNull(value.toBool());
    }
    else
    if (index.column() == 4 && is_size_field)
    {
        row->setNotNull(value.toBool());
    }
    else
    if (index.column() == 5)
    {
        row->setUnique(value.toBool());
    }
    else
        return false;


    emit dataChanged(index, index);

    return true;
}

bool CEntity::removeRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || row >= row_vec.size())
        return false;

    if (count < 1)
        return false;

    emit beginRemoveRows(parent, row, (row + count - 1)%row_vec.size());

    for (int i = 0; i < count; ++i)
    {
        if (row >= row_vec.size())
            break;
        row_vec.remove(row);

        for (int k = 0; k < CmplxUnique.size(); ++k)
        {
            for (int j = 0; j < CmplxUnique[k].size(); ++j)
            {
                if (row == CmplxUnique[k][j])
                {
                    CmplxUnique[k].removeAt(j);
                    --j;
                }
                else
                if (CmplxUnique[k][j] > row)
                {
                    CmplxUnique[k][j] = CmplxUnique[k][j] - 1;
                }
            }
        }
    }

    emit endRemoveRows();

    return true;
}

Qt::ItemFlags CEntity::flags(const QModelIndex &index) const
{    
    if (!is_editable && index.column() != 0)
        return 0;

    bool is_size_field = true;


    int fk = 5;
    if (is_size_field)
        ++fk;

    if (is_size_field && index.column() == 2)
    {
        if (index.row() < this->getRowCount())
            if (this->row_vec[index.row()].getType() != CRow::CHAR)
                return Qt::ItemIsSelectable;
    }

    if ((index.column() == 4 || index.column() == 5) && this->row_vec[index.row()].isPrimaryKey())
    {
        return Qt::ItemIsSelectable;
    }

    if (index.column() != fk)
    {
        return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    return Qt::ItemIsSelectable;
}

void CEntity::set_editable(bool flag)
{
    is_editable = flag;
}

void CEntity::cmplxURowDeleted(int row)
{
    for (int i = 0; i < CmplxUnique.size(); ++i)
    {
        for (int j = 0; j < CmplxUnique.at(i).size(); ++j)
        {
            if (CmplxUnique.at(i).at(j) == row)
            {
                CmplxUnique[i].removeAt(j);
                --j;
            }
            else
            if (CmplxUnique.at(i).at(j) > row)
            {
                CmplxUnique[i][j] = CmplxUnique.at(i).at(j) - 1;
            }
        }
    }
}

void CEntity::addCmplxUniquesGroup()
{
    CmplxUnique.append(QVector<int>());
}

bool CEntity::addToCmplxUniqueGroup(int gr, int row)
{
    if (row >= this->row_vec.size())
        return false;

    if (gr >= CmplxUnique.size())
        return false;

    for (int i = 0; i < CmplxUnique.size(); ++i)
    {
        for (int j = 0; j < CmplxUnique.at(i).size(); ++j)
        {
            if (CmplxUnique.at(i).at(j) == row)
            {
                return false;
            }
        }
    }

    this->row_vec[row].setUnique(true);

    CmplxUnique[gr].append(row);

    return true;
}

bool CEntity::remCmplxUniquesGroup(int gr)
{
    if (gr >= CmplxUnique.size())
        return false;

    while(CmplxUnique[gr].size())
    {
        remFromCmplxUniquesGroup(gr, CmplxUnique[gr][0]);
    }

    CmplxUnique.removeAt(gr);

    return true;
}

bool CEntity::remFromCmplxUniquesGroup(int gr, int row)
{
    if (gr >= CmplxUnique.size())
        return false;

    int r;

    for (int i = 0; i < CmplxUnique[gr].size(); ++i)
    {
        if (row == CmplxUnique[gr][i])
        {
            r = i;
            break;
        }
    }

    CmplxUnique[gr].removeAt(r);

    if (row < this->row_vec.size())
        this->row_vec[row].setUnique(false);

    return true;
}

int CEntity::addForeignRow(int table_id, int row_id) //check
{
    foreign_row_vec.append(CForeignRow());
    foreign_row_vec.last().setForeignTable(table_id, row_id);
    return foreign_row_vec.size() - 1;
}

void CEntity::remForeignRow(int i)
{
    foreign_row_vec.remove(i);
}
