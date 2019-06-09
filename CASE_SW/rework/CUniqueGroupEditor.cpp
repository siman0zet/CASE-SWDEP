#include "CUniqueGroupEditor.h"
#include "ui_CUniqueGroupEditor.h"
#include "CTable.h"
#include "CUniqueGroup.h"
#include "CRow.h"

#include <QDebug>
#include <QLineEdit>

CUniqueGroupEditor::CUniqueGroupEditor(CTable *table, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CUniqueGroupEditor),
    _table(table),
    _currentUGroup(0),
    _countUgroup(0)
{
    ui->setupUi(this);
    synchronizeData();
}

CUniqueGroupEditor::~CUniqueGroupEditor()
{
    delete ui;
}

void CUniqueGroupEditor::on_pushCreate_clicked()
{
    int id = ++_countUgroup;
    for(int i = 0; i < _ugroups.count(); i++)
    {
        CUniqueGroup *ugroup = _ugroups.values().at(i);
        if(ugroup->name() == QString("group_%1").arg(id))
        {
            id = ++_countUgroup;
            i = 0;
        }
    }
    _currentUGroup = new CUniqueGroup(QString("group_%1").arg(id));
    _ugroups.insert(_currentUGroup->name(), _currentUGroup);
    ui->comboBox->addItem(_currentUGroup->name());

    int index = ui->comboBox->count() - 1;
    ui->comboBox->setCurrentIndex(index);
    _currentUGroup = _ugroups.value(ui->comboBox->currentText());
}

void CUniqueGroupEditor::on_pushDelete_clicked()
{
    ui->listWidget->clear();
    _ugroups.remove(_currentUGroup->name());
    ui->comboBox->removeItem(ui->comboBox->currentIndex());

    int index = ui->comboBox->count() - 1;
    if(index >= 0)
    {
        ui->comboBox->setCurrentIndex(index);
        _currentUGroup = _ugroups.value(ui->comboBox->currentText());
    }
    else
        _currentUGroup = 0;
}

void CUniqueGroupEditor::on_comboBox_currentIndexChanged(const QString &arg1)
{
    int index = ui->comboBox->count() - 1;
    if(index >= 0)
    {
        _currentUGroup = _ugroups.value(arg1);
        updateCurrentGroupList();
    }
    else
        _currentUGroup = 0;

    if(ui->comboBox->count() <= 0)
        ui->comboBox->setEditable(false);
    else
    {
        ui->comboBox->setEditable(true);
        connect(ui->comboBox->lineEdit(), SIGNAL(editingFinished()), this, SLOT(comboBoxEditingFinished()));
    }
}

void CUniqueGroupEditor::synchronizeData()
{
    foreach (CUniqueGroup *ugroup, _table->uniqueGroups()) {
        _ugroups.insert(ugroup->name(), ugroup);
        ui->comboBox->addItem(ugroup->name());
    }
}

void CUniqueGroupEditor::updateCurrentGroupList()
{
    ui->listWidget->clear();

    QList<CRow *> rows = _table->rows();
    QStringList strList;
    foreach (CRow *row, rows) {
        strList << row->name();
    }
    QListWidgetItem *item = 0;
    ui->listWidget->addItems(strList);
    for(int i = 0; i < ui->listWidget->count(); i++)
    {
        item = ui->listWidget->item(i);
        item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        item->setCheckState(Qt::Unchecked);
    }

    QList<QString> ugroupRows = _currentUGroup->rows();
    if(!ugroupRows.isEmpty())
    {
        QListWidgetItem *item = 0;
        foreach (QString name, ugroupRows) {
            for(int i = 0; i < ui->listWidget->count(); i++){
                item = ui->listWidget->item(i);
                if(name == item->text())
                {
                    item->setCheckState(Qt::Checked);
                    break;
                }
            }
        }
    }
}

void CUniqueGroupEditor::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if(item->checkState() == Qt::Checked)
    {
        item->setCheckState(Qt::Unchecked);
        _currentUGroup->removeRow(item->text());
    }
    else
    {
        item->setCheckState(Qt::Checked);
        _currentUGroup->addRow(item->text());
    }
}

void CUniqueGroupEditor::comboBoxEditingFinished()
{
    if(ui->comboBox->currentIndex() < 0)
        return;
    _ugroups.remove(_currentUGroup->name());
    _currentUGroup->setName(ui->comboBox->currentText());
    _ugroups.insert(_currentUGroup->name(), _currentUGroup);
}

void CUniqueGroupEditor::on_buttonBox_rejected()
{
    close();
}

void CUniqueGroupEditor::on_buttonBox_accepted()
{
    _table->setUniqueGroups(_ugroups.values());
    emit accepted();
    close();
}
