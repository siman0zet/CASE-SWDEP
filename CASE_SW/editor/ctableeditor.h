#ifndef CTABLEEDITOR_H
#define CTABLEEDITOR_H

#include <QWidget>

namespace Ui {
class CTableEditor;
}

class CTable;
class CDataModel;
class CRow;

class CTableEditor : public QWidget
{
    Q_OBJECT

public:
    CTableEditor(CTable *table, CDataModel *dataModel, QWidget *parent = 0);
    ~CTableEditor();

private slots:
    void on_tableWidget_cellChanged(int row, int column);
    void tableCheckBoxToggled(bool flag);
    void tableComboBoxIndexChanged(int index);
    void tableComboBoxEditingFinished();
    void on_pushAddRow_clicked();
    void on_pushRemoveRow_clicked();
    void on_pushUniques_clicked();
    void on_lineTableName_editingFinished();
    void synchronizeData();

private:
    Ui::CTableEditor *ui;

    int _countRow;

    CTable *_table;
    CDataModel *_dataModel;

    void addRow(CRow *row);
    QWidget *createCheckBoxWidget(bool flag, int row, int col);

signals:
    void dataChanged();
};

#endif // CTABLEEDITOR_H
