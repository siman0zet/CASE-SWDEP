#include "cuniquegroupdialog.h"
#include "ui_cuniquegroupdialog.h"
#include "../model/ctable.h"
#include "../model/cuniquegroup.h"
#include "../model/crow.h"

#include <QDebug>
#include <QLineEdit>

CUniqueGroupDialog::CUniqueGroupDialog(CTable *table, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CUniqueGroupDialog),
    _table(table),
    _currentUGroup(0),
    _countUgroup(0)
{
    ui->setupUi(this);
    synchronizeData();
}

CUniqueGroupDialog::~CUniqueGroupDialog()
{
    delete ui;
}

void CUniqueGroupDialog::on_pushCreate_clicked()
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

void CUniqueGroupDialog::on_pushDelete_clicked()
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

void CUniqueGroupDialog::on_comboBox_currentIndexChanged(const QString &arg1)
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

void CUniqueGroupDialog::synchronizeData()
{
    foreach (CUniqueGroup *ugroup, _table->uniqueGroups()) {
        _ugroups.insert(ugroup->name(), ugroup);
        ui->comboBox->addItem(ugroup->name());
    }
}

void CUniqueGroupDialog::updateCurrentGroupList()
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

void CUniqueGroupDialog::on_listWidget_itemClicked(QListWidgetItem *item)
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

void CUniqueGroupDialog::comboBoxEditingFinished()
{
    if(ui->comboBox->currentIndex() < 0)
        return;
    _ugroups.remove(_currentUGroup->name());
    _currentUGroup->setName(ui->comboBox->currentText());
    _ugroups.insert(_currentUGroup->name(), _currentUGroup);
}

void CUniqueGroupDialog::on_buttonBox_rejected()
{
    close();
}

void CUniqueGroupDialog::on_buttonBox_accepted()
{
    _table->setUniqueGroups(_ugroups.values());
    emit accepted();
    close();
}
