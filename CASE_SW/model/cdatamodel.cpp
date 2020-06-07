#include "cdatamodel.h"
#include "ctable.h"
#include "crelationship.h"
#include "cforeignrow.h"

CDataModel::CDataModel() :
    _physical(false),
    _countTables(0)
{
}

CDataModel *CDataModel::convertToPhysical()
{
    if(this->isPhysical())
        return this;

    CDataModel *physical = new CDataModel();
    physical->setPhysical(true);

    foreach (const CTable *table, _tables) {
        CTable *copyTable = new CTable(table);
        physical->addTable(copyTable);
    }

    foreach (const CRelationship *relationship, _relationships) {
        /*
           if(relationship->startType() == CRelationship::ONE &&
           relationship->endType() == CRelationship::ONE)
        {
            if(relationship->endMandatory())
            {
                physical->addForeingTableConnection(relationship->startTable(),
                                                    relationship->endTable());
            }
            else
            {
                if(relationship->startMandatory())
                    physical->addForeingTableConnection(relationship->endTable(),
                                                        relationship->startTable());
                else
                    physical->addConduitTableConnection(relationship->startTable(),
                                                        relationship->endTable());
            }
        }

        if(relationship->startType() == CRelationship::ONE &&
           relationship->endType() == CRelationship::MANY)
        {
            if(relationship->startMandatory())
                physical->addForeingTableConnection(relationship->endTable(),
                                                    relationship->startTable());
            else
                physical->addConduitTableConnection(relationship->startTable(),
                                                relationship->endTable());
        }

        if(relationship->startType() == CRelationship::ONE &&
           relationship->endType() == CRelationship::AGGREGATE)
            physical->addForeingTableConnection(relationship->endTable(),
                                                relationship->startTable(),
                                                true);
        */
        if(relationship->startType() == CRelationship::MANY &&
           relationship->endType() == CRelationship::MANY)
            physical->addConduitTableConnection(relationship->startTable(),
                                                relationship->endTable(),
                                                true);
        else  if(relationship->startType() == CRelationship::ONE &&
                 relationship->endType() == CRelationship::ONE && !relationship->endMandatory()
                 && !relationship->startMandatory())
        {
            physical->addConduitTableConnection(relationship->startTable(),
                                                relationship->endTable());
        }
        else
        {
            if(physical->_relationships.find(QString("%1_%2")
                                   .arg(relationship->startTable()->name())
                                   .arg(relationship->endTable()->name())) != physical->_relationships.end()) continue;
            CRelationship *relationship_new = new CRelationship(physical->tables().value(relationship->startTable()->name()),
                                                                physical->tables().value(relationship->endTable()->name()));
            relationship_new->setEndType(CRelationship::ARROW);
            physical->_relationships.insert(relationship_new->name(), relationship_new);
        }
    }

    return physical;
}

CTable *CDataModel::addTable()
{
    int id = ++_countTables;
    for(int i = 0; i < _tables.size(); i++)
    {
        CTable *table = _tables.values().at(i);
        if(table->name() == QString("Table_%1").arg(id))
        {
            id = ++_countTables;
            i = 0;
        }
    }
    CTable *table = new CTable(QString("Table_%1").arg(id));
    _tables.insert(table->name(), table);
    return table;
}

void CDataModel::addTable(CTable *table)
{
    _tables.insert(table->name(), table);
}

CRelationship *CDataModel::addRelationship(const QString &startName, const QString &endName)
{
    foreach (CRelationship *relationship, _relationships.values()) {
        if((relationship->startTable()->name() == startName ||
            relationship->endTable()->name() == startName) &&
           (relationship->startTable()->name() == endName ||
            relationship->endTable()->name() == endName))
        {
            return nullptr;    //relationship between this tables already exist
        }
    }
    CRelationship *relationship = new CRelationship(_tables.value(startName),
                                                    _tables.value(endName));
    _relationships.insert(relationship->name(), relationship);
    return relationship;
}

void CDataModel::addRelationship(CRelationship *relationship)
{
    _relationships.insert(relationship->name(), relationship);
}

void CDataModel::removeObjects(const QList<CObject *> &objects)
{
    _objectsToRemove = objects;
    foreach (CObject *object, _objectsToRemove) {
        if(object->type() == CRelationship::Type){
            removeRelationship(object->name());
            continue;
        }
        if(object->type() == CTable::Type)
        {
            removeTable(object->name());
            continue;
        }
    }
    _objectsToRemove.clear();
}

void CDataModel::flipTables(const QString &relationshipName)
{
    CTable *table = _relationships.value(relationshipName)->startTable();
    _relationships.value(relationshipName)->setStartTable(_relationships.value(relationshipName)->endTable());
    _relationships.value(relationshipName)->setEndTable(table);
}

void CDataModel::changeRelationshipTable(const QString &relationshipName, const QString &tableName, bool start)
{
    if(start)
    {
        _relationships.value(relationshipName)->setStartTable(_tables.value(tableName));
    }
    else
    {
        _relationships.value(relationshipName)->setEndTable(_tables.value(tableName));
    }
}

QString CDataModel::changeTabelName(const QString &oldName, const QString &newName)
{
    foreach(CTable *table, _tables)
    {
        if(table->name() == newName)
            return oldName;
    }
    _tables.value(oldName)->setName(newName);
    _tables.insert(newName, _tables.value(oldName));
    _tables.remove(oldName);
    emit tableNameChanged(oldName, newName);
    foreach (CRelationship *relationship, _tables.value(newName)->relationships()) {
        _relationships.remove(relationship->name());
        if (relationship->startTable() == _tables.value(newName))
                relationship->setName(QString("%1_%2")
                                      .arg(newName)
                                      .arg(relationship->endTable()->name()));

        if (relationship->endTable() == _tables.value(newName))
                relationship->setName(QString("%1_%2")
                                      .arg(relationship->startTable()->name())
                                      .arg(newName));
        _relationships.insert(relationship->name(), relationship);
    }
    return newName;
}
/*
void CDataModel::addForeingTableConnection(const CTable *startTable, const CTable *endTable, bool identifying)
{
    if(_relationships.find(QString("%1_%2")
                           .arg(startTable->name())
                           .arg(endTable->name())) != _relationships.end())
        return;

    CTable *_startTable = new CTable(startTable);
    CTable *_endTable = new CTable(endTable);
    if(_tables.find(startTable->name()) != _tables.end())
        _startTable = _tables.value(startTable->name());
    if(_tables.find(endTable->name()) != _tables.end())
        _endTable = _tables.value(endTable->name());
    //-----------------------------------------------------------------
    QList<CRow *> endTablePK = _endTable->primaryKey();    
    QList<CForeignRow *> endTableFK = _endTable->foreingRows();
    if(!endTablePK.isEmpty()) {
        foreach (CRow *row, endTablePK) {
            CForeignRow *fRow = new CForeignRow(row, _endTable->name());
            if(identifying)
                fRow->setPrimaryKey(true);
            _startTable->addForeignRow(fRow);
        }
    }
    else if(!endTableFK.isEmpty())
    {
        foreach (CForeignRow *frow, endTableFK) {
            _startTable->addForeignRow(frow);
        }
    }
    else return;
    //-----------------------------------------------------------------

    _tables.insert(_startTable->name(), _startTable);
    _tables.insert(_endTable->name(), _endTable);

    CRelationship *relationship = new CRelationship(_startTable, _endTable);
    relationship->setEndType(CRelationship::ARROW);
    _relationships.insert(relationship->name(), relationship);
}
*/
void CDataModel::addConduitTableConnection(const CTable *startTable, const CTable *endTable, bool identifying)
{
    CTable *_startTable = new CTable(startTable);
    CTable *_endTable = new CTable(endTable);
    if(_tables.find(startTable->name()) != _tables.end())
        _startTable = _tables.value(startTable->name());
    if(_tables.find(endTable->name()) != _tables.end())
        _endTable = _tables.value(endTable->name());

    CTable *_conduitTable = new CTable(QString("%1_%2")
                                       .arg(_startTable->name())
                                       .arg(_endTable->name()));

    if(_relationships.find(QString("%1_%2")
                           .arg(_conduitTable->name())
                           .arg(_startTable->name())) != _relationships.end())
        return;
    if(_relationships.find(QString("%1_%2")
                           .arg(_conduitTable->name())
                           .arg(_endTable->name())) != _relationships.end())
        return;
    //-----------------------------------------------------------------
    QList<CRow *> endTablePK = _endTable->primaryKey();
    QList<CForeignRow *> endTableFK = _endTable->foreingRows();
    if(!endTablePK.isEmpty())
        foreach (CRow *row, endTablePK) {
            CForeignRow *fRow = new CForeignRow(row, _endTable->name());
            fRow->setPrimaryKey(true);
            _conduitTable->addForeignRow(fRow);
        }
    else if(!endTableFK.isEmpty())
        foreach (CForeignRow *frow, endTableFK) {
            _conduitTable->addForeignRow(frow);
        }
    else return;
    //-----------------------------------------------------------------
    QList<CRow *> startTablePK = _startTable->primaryKey();
    QList<CForeignRow *> startTableFK = _startTable->foreingRows();
    if(!startTablePK.isEmpty())
        foreach (CRow *row, startTablePK) {
            CForeignRow *fRow = new CForeignRow(row, _startTable->name());
            if(identifying)
                fRow->setPrimaryKey(true);
            _conduitTable->addForeignRow(fRow);
        }
    else if(!startTableFK.isEmpty())
        foreach (CForeignRow *frow, startTableFK) {
            _conduitTable->addForeignRow(frow);
        }
    else return;
    //-----------------------------------------------------------------
    _tables.insert(_startTable->name(), _startTable);
    _tables.insert(_endTable->name(), _endTable);
    _tables.insert(_conduitTable->name(), _conduitTable);

    CRelationship *relationshipStart = new CRelationship(_conduitTable, _startTable);
    relationshipStart->setEndType(CRelationship::ARROW);
    _relationships.insert(relationshipStart->name(), relationshipStart);

    CRelationship *relationshipEnd = new CRelationship(_conduitTable, _endTable);
    relationshipEnd->setEndType(CRelationship::ARROW);
    _relationships.insert(relationshipEnd->name(), relationshipEnd);
}

QList<CTable *> CDataModel::listTables() const
{
    return _tables.values();
}

bool CDataModel::isPhysical() const
{
    return _physical;
}

void CDataModel::setPhysical(bool physical)
{
    _physical = physical;
}

QMap<QString, CTable *> CDataModel::tables() const
{
    return _tables;
}

QMap<QString, CRelationship *> CDataModel::relationships() const
{
    return _relationships;
}

QList<CTable *> CDataModel::tablesSortedByReference() const
{
    QList<CTable *> sortedTables;
    foreach (CTable *table, _tables.values()) {
        sortTablesByReference(table, sortedTables);
    }
    return sortedTables;
}

void CDataModel::sortTablesByReference(CTable *table, QList<CTable *> &list) const
{
    if(!list.contains(table))
    {
        if(table->foreignRowCount() == 0)
            list.append(table);
        else
        {
            foreach (CForeignRow *foreignRow, table->foreingRows()) {
                sortTablesByReference(_tables.value(foreignRow->tableName()), list);
            }
            list.append(_tables.value(table->name()));
        }
    }
}

void CDataModel::removeTable(const QString &name)
{
    delete _tables.value(name);
    _tables.remove(name);
}

void CDataModel::removeRelationship(const QString &name)
{
    delete _relationships.value(name);
    _relationships.remove(name);
}
