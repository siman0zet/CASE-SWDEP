#ifndef CUNIQUEGROUPEDITOR_H
#define CUNIQUEGROUPEDITOR_H

#include <QMap>
#include <QWidget>

namespace Ui {
class CUniqueGroupDialog;
}

class CTable;
class CUniqueGroup;
class QListWidgetItem;

class CUniqueGroupDialog : public QWidget
{
    Q_OBJECT

public:
    CUniqueGroupDialog(CTable *table, QWidget *parent = 0);
    ~CUniqueGroupDialog();

private slots:
    void on_pushCreate_clicked();
    void on_pushDelete_clicked();
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void comboBoxEditingFinished();
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();

private:
    Ui::CUniqueGroupDialog *ui;

    CTable *_table;
    CUniqueGroup *_currentUGroup;
    QMap<QString, CUniqueGroup *> _ugroups;
    int _countUgroup;

    void synchronizeData();
    void updateCurrentGroupList();

signals:
    void accepted();
};

#endif // CUNIQUEGROUPEDITOR_H
