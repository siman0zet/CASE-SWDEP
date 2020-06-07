#include "../model/crow.h"
#include "../model/ctable.h"
#include "ctableeditor.h"
#include "../model/cdatamodel.h"
#include "ui_ctableeditor.h"
#include "../dialog/cuniquegroupdialog.h"

#include <QCheckBox>
#include <QComboBox>

CTableEditor::CTableEditor(CTable *table, CDataModel *dataModel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTableEditor),
    _countRow(0),
    _table(table),
    _dataModel(dataModel)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->sectionResizeMode(QHeaderView::ResizeToContents);

    synchronizeData();
}

CTableEditor::~CTableEditor()
{
    delete ui;
}

void CTableEditor::synchronizeData()
{
    ui->lineTableName->setText(_table->name());
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    foreach (CRow *row, _table->rows()) {
        addRow(row);
    }
}

void CTableEditor::addRow(CRow *row)
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);

    QTableWidgetItem *item1 = new QTableWidgetItem;
    item1->setText(row->name());
    ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, item1);

    QComboBox *comboBox = new QComboBox();
    comboBox->setEditable(true);
    comboBox->setInsertPolicy(QComboBox::NoInsert);
    comboBox->addItem("INTEGER", CRow::INTEGER);
    comboBox->addItem("FLOAT", CRow::FLOAT);
    comboBox->addItem("CHAR()", CRow::CHAR);
    comboBox->addItem("VARCHAR()", CRow::VARCHAR);
    comboBox->addItem("BOOLEAN", CRow::BOOLEAN);
    comboBox->addItem("DATE", CRow::DATE);
    comboBox->addItem("BLOB", CRow::BLOB);
    comboBox->setProperty("row", (int) ui->tableWidget->rowCount() - 1);
    comboBox->setProperty("col", (int) 1);
    comboBox->setCurrentIndex((int) row->type());
    connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(tableComboBoxIndexChanged(int)));
    connect(comboBox->lineEdit(), SIGNAL(editingFinished()), this, SLOT(tableComboBoxEditingFinished()));
    connect(comboBox, SIGNAL(activated(int)), this, SLOT(tableComboBoxIndexChanged(int)));
    ui->tableWidget->setCellWidget(comboBox->property("row").toInt(),
                                   comboBox->property("col").toInt(),
                                   comboBox);

    QList<bool> rowBools;
    rowBools << row->primaryKey() << row->notNull() << row->unique();
    for(int col = 2; col <= 4; col++)
    {
        QWidget *checkBoxWidget = createCheckBoxWidget(rowBools.at(col - 2),
                                                       ui->tableWidget->rowCount() - 1,
                                                       col);
        ui->tableWidget->setCellWidget(ui->tableWidget->rowCount() - 1,
                                       col,
                                       checkBoxWidget);
    }
}

QWidget *CTableEditor::createCheckBoxWidget(bool flag, int row, int col)
{
    QWidget *checkBoxWidget = new QWidget();
    QCheckBox *checkBox = new QCheckBox();
    QHBoxLayout *checkBoxLayout = new QHBoxLayout(checkBoxWidget);
    checkBoxLayout->addWidget(checkBox);
    checkBoxLayout->setAlignment(Qt::AlignCenter);
    checkBoxLayout->setContentsMargins(0, 0, 0, 0);
    checkBox->setChecked(flag);
    checkBox->setProperty("row", (int) row);
    checkBox->setProperty("col", (int) col);
    connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(tableCheckBoxToggled(bool)));
    return checkBoxWidget;
}

void CTableEditor::on_tableWidget_cellChanged(int row, int column)
{
    QTableWidgetItem *item = ui->tableWidget->item(row, column);
    QString name = item->text();
    item->setText(_table->changeRowName(row, name));
    emit dataChanged();
}

void CTableEditor::tableCheckBoxToggled(bool flag)
{
    QCheckBox *checkBox = (QCheckBox *)QObject::sender();
    switch (checkBox->property("col").toInt()) {
    case 2: //PK
        _table->row(checkBox->property("row").toInt())->setPrimaryKey(flag);
        break;
    case 3: //NN
        _table->row(checkBox->property("row").toInt())->setNotNull(flag);
        break;
    case 4: //UQ
        _table->row(checkBox->property("row").toInt())->setUnique(flag);
        break;
    default:
        break;
    }
    emit dataChanged();
}

void CTableEditor::tableComboBoxIndexChanged(int index)
{
    auto type = static_cast<CRow::DATA_TYPE>(index);
    QComboBox *comboBox = (QComboBox *)QObject::sender();
    _table->row(comboBox->property("row").toInt())->setSize(10);
    _table->row(comboBox->property("row").toInt())->setType(type);
    comboBox->lineEdit()->setText(_table->row(comboBox->property("row").toInt())->typeAsString());
    emit dataChanged();
}

void CTableEditor::tableComboBoxEditingFinished()
{
    QLineEdit *line = (QLineEdit *)QObject::sender();
    QComboBox *comboBox = (QComboBox *)line->parent();
    QString text = line->text();
    int size = text.size();
    comboBox->setCurrentIndex((int)_table->row(comboBox->property("row").toInt())->type());
    comboBox->lineEdit()->setText(_table->row(comboBox->property("row").toInt())->typeAsString());

    QRegExpValidator validator1;
    QRegExpValidator validator2;

    validator1.setRegExp(QRegExp("CHAR\\([0-9]+\\)", Qt::CaseInsensitive));
    validator2.setRegExp(QRegExp("VARCHAR\\([0-9]+\\)", Qt::CaseInsensitive));
    if(validator1.validate(text, size) == QValidator::Acceptable ||
       validator2.validate(text, size) == QValidator::Acceptable)
    {
        QRegExp regExp("\\((.+)\\)", Qt::CaseInsensitive);
        if(regExp.indexIn(text))
        {
            line->setText(text);
            int value = regExp.cap(1).toInt();
            _table->row(comboBox->property("row").toInt())->setSize(value);
            emit dataChanged();
        }
    }
}

void CTableEditor::on_pushAddRow_clicked()
{
    int id = ++_countRow;
    for(int i = 0; i < _table->rowCount(); i++)
    {
        CRow *row = _table->row(i);
        if(row->name() == QString("row_%1").arg(id))
        {
            id = ++_countRow;
            i = 0;
        }
    }
    CRow *row = new CRow(QString("row_%1").arg(id));
    _table->addRow(row);
    this->addRow(row);
    emit dataChanged();
}

void CTableEditor::on_pushRemoveRow_clicked()
{
    QModelIndexList indexes = ui->tableWidget->selectionModel()->selectedRows();
    QList<int> rows;
    foreach (QModelIndex index, indexes) {
        rows << index.row();
    }
    qSort(rows);
    for(int i = rows.size() - 1; i >=0; i--) {
        _table->removeRow(rows.at(i));
        ui->tableWidget->removeRow(rows.at(i));
    }
    emit dataChanged();
    synchronizeData();
}

void CTableEditor::on_pushUniques_clicked()
{
    CUniqueGroupDialog *dialog = new CUniqueGroupDialog(_table);
    connect(dialog, SIGNAL(accepted()), this, SLOT(synchronizeData()));
    dialog->show();
}

void CTableEditor::on_lineTableName_editingFinished()
{
    QString name = ui->lineTableName->text();
    ui->lineTableName->setText(_dataModel->changeTabelName(_table->name(), name));
    emit dataChanged();
}
