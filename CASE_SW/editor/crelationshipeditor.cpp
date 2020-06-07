#include "../model/crelationship.h"
#include "crelationshipeditor.h"
#include "../model/ctable.h"
#include "../view/cmodelview.h"
#include "ui_crelationshipeditor.h"
#include "../model/cforeignrow.h"

#include <QDebug>

CRelationshipEditor::CRelationshipEditor(CRelationship *relationship, CModelView *modelView, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CRelationshipEditor),
    _relationship(relationship),
    _modelView(modelView)
{
    ui->setupUi(this);
    synchronizeData();
}

CRelationshipEditor::~CRelationshipEditor()
{
    delete ui;
    _relationship = NULL;
}

CRelationship *CRelationshipEditor::relationship() const
{
    return _relationship;
}

void CRelationshipEditor::synchronizeData()
{
    if(_relationship->startType() == CRelationship::ONE &&
       _relationship->endType() == CRelationship::ONE)
        ui->radioOneOne->setChecked(true);

    if(_relationship->startType() == CRelationship::ONE &&
       _relationship->endType() == CRelationship::MANY)
        ui->radioOneMany->setChecked(true);

    if(_relationship->startType() == CRelationship::MANY &&
       _relationship->endType() == CRelationship::MANY)
        ui->radioManyMany->setChecked(true);

    if(_relationship->startType() == CRelationship::ONE &&
       _relationship->endType() == CRelationship::AGGREGATE)
        ui->radioAggregate->setChecked(true);

    ui->startTableName->setText(_relationship->startTable()->name());
    ui->endTableName->setText(_relationship->endTable()->name());

    ui->starTableMandatory->setChecked(_relationship->startMandatory());
    ui->endTableMandatory->setChecked(_relationship->endMandatory());
}

void CRelationshipEditor::on_radioOneOne_toggled(bool checked)
{
    if(checked)
    {
        _relationship->setStartType(CRelationship::ONE);
        _relationship->setEndType(CRelationship::ONE);
        if(_relationship->endMandatory()) {
            DeleteOldForeginRows(_relationship->startTable(), _relationship->endTable());
            UpdateTable(_relationship->startTable(), _relationship->endTable(), false);
        }
        else if(_relationship->startMandatory()) {
            DeleteOldForeginRows(_relationship->endTable(), _relationship->startTable());
            UpdateTable(_relationship->endTable(), _relationship->startTable(), false);
        }
        else {
            ClearOldFR();
        }
    }
    emit dataChanged();
}

void CRelationshipEditor::on_radioOneMany_toggled(bool checked)
{
    if(checked)
    {
        _relationship->setStartType(CRelationship::ONE);
        _relationship->setEndType(CRelationship::MANY);
        if(_relationship->startMandatory())
        {
            DeleteOldForeginRows(_relationship->startTable(), _relationship->endTable());
            UpdateTable(_relationship->startTable(), _relationship->endTable(), false);
        }
        else
        {
            DeleteOldForeginRows(_relationship->endTable(), _relationship->startTable());
            UpdateTable(_relationship->endTable(), _relationship->startTable(), false);
        }
    }
    emit dataChanged();
}

void CRelationshipEditor::on_radioManyMany_toggled(bool checked)
{
    if(checked)
    {
        ClearOldFR();
        _relationship->setStartType(CRelationship::MANY);
        _relationship->setEndType(CRelationship::MANY);
    }
    emit dataChanged();
}

void CRelationshipEditor::on_radioAggregate_toggled(bool checked)
{
    if(checked)
    {
        DeleteOldForeginRows(_relationship->startTable(), _relationship->endTable());
        _relationship->setStartType(CRelationship::ONE);
        _relationship->setEndType(CRelationship::AGGREGATE);
        UpdateTable(_relationship->startTable(), _relationship->endTable(), true);
    }
    emit dataChanged();
}

//------------------------------------------------------------------------------
void CRelationshipEditor::ClearOldFR()
{
    DeleteOldForeginRows(_relationship->startTable(), _relationship->endTable());
    DeleteOldForeginRows(_relationship->endTable(), _relationship->startTable());
}
void CRelationshipEditor::DeleteOldForeginRows(CTable *startTable, CTable *endTable)
{
    QList<CForeignRow *> TableFRows = endTable->foreingRows();
    foreach(CForeignRow *fRow, TableFRows){
        if(fRow->tableName() == startTable->name() || startTable->SearchFROnName(fRow->name()))
            endTable->removeForeginRow(fRow);
    }
    TableFRows.clear();
}
void CRelationshipEditor::UpdateTable(CTable *startTable, CTable *endTable, bool PK)
{
    QList<CForeignRow *> startTableFK = startTable->foreingRows();
    QList<CRow *> startTablePK = startTable->primaryKey();
    if(startTablePK.isEmpty() && startTableFK.isEmpty())
        return;
    foreach (CRow *row, startTablePK) {
        CForeignRow *fRow = new CForeignRow(row, startTable->name());
        if(PK)
            fRow->setPrimaryKey(true);
        endTable->addForeignRow(fRow);
    }
    foreach (CForeignRow *frow, startTableFK) {
        endTable->addForeignRow(frow);
    }
}
int CRelationshipEditor::CheckRelationship()
{
    if(_relationship->startType() == CRelationship::ONE &&
        _relationship->endType() == CRelationship::ONE)
        return 1;
    else if(_relationship->startType() == CRelationship::ONE &&
        _relationship->endType() == CRelationship::MANY)
        return 2;
    else if(_relationship->startType() == CRelationship::MANY &&
        _relationship->endType() == CRelationship::MANY)
        return 3;
    else if(_relationship->startType() == CRelationship::ONE &&
        _relationship->endType() == CRelationship::AGGREGATE)
        return 4;
    return 0;
}
void CRelationshipEditor::ChooseUpdateTables(int numRel)
{
    switch (numRel) {
    case 1:
        if(_relationship->endMandatory())
            UpdateTable(_relationship->startTable(), _relationship->endTable(), false);
        else if(_relationship->startMandatory())
            UpdateTable(_relationship->endTable(), _relationship->startTable(), false);
        else ClearOldFR();
        break;
    case 2:
        if(_relationship->startMandatory())
            UpdateTable(_relationship->startTable(), _relationship->endTable(), false);
        else UpdateTable(_relationship->endTable(), _relationship->startTable(), false);
        break;
    case 3:
    case 4:
    default:
        break;
    }
}

void CRelationshipEditor::ChooseClearTables(int numRel)
{
    switch (numRel) {
    case 1:
        if(_relationship->endMandatory())
            DeleteOldForeginRows(_relationship->startTable(), _relationship->endTable());
        else if(_relationship->startMandatory())
            DeleteOldForeginRows(_relationship->endTable(), _relationship->startTable());
        break;
    case 2:
        if(_relationship->startMandatory())
            DeleteOldForeginRows(_relationship->startTable(), _relationship->endTable());
        else DeleteOldForeginRows(_relationship->endTable(), _relationship->startTable());
        break;
    case 3:
    case 4:
    default:
        break;
    }
}
//------------------------------------------------------------------------------
void CRelationshipEditor::on_starTableMandatory_toggled(bool checked)
{
    ChooseClearTables(CheckRelationship());
    _relationship->setStartMandatory(checked);
    ChooseUpdateTables(CheckRelationship());
    emit dataChanged();
}

void CRelationshipEditor::on_endTableMandatory_toggled(bool checked)
{
    ChooseClearTables(CheckRelationship());
    _relationship->setEndMandatory(checked);
    ChooseUpdateTables(CheckRelationship());
    emit dataChanged();
}

void CRelationshipEditor::on_flipTables_clicked()
{
    ChooseClearTables(CheckRelationship());
    _modelView->flipTables(_relationship->name());
    ChooseUpdateTables(CheckRelationship());
    synchronizeData();
}

void CRelationshipEditor::on_startTableChange_clicked()
{
    ChooseClearTables(CheckRelationship());
    _modelView->showChangeTableDialog(_relationship->name(), true);
    ChooseUpdateTables(CheckRelationship());
    synchronizeData();
}

void CRelationshipEditor::on_endTableChange_clicked()
{
    ChooseClearTables(CheckRelationship());
    _modelView->showChangeTableDialog(_relationship->name(), false);
    ChooseUpdateTables(CheckRelationship());
    synchronizeData();
}

void CRelationshipEditor::on_startTableEdit_clicked()
{
    _modelView->showObjectEditor((CObjectItem *)_modelView->tableItem(_relationship->startTable()->name()));
}

void CRelationshipEditor::on_endTableEdit_clicked()
{
    _modelView->showObjectEditor((CObjectItem *)_modelView->tableItem(_relationship->endTable()->name()));
}
