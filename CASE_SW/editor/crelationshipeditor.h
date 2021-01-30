#ifndef CRELATIONSHIPEDITOR_H
#define CRELATIONSHIPEDITOR_H

#include <QWidget>

class CRelationship;
class CModelView;
class CTable;

namespace Ui {
class CRelationshipEditor;
}

class CRelationshipEditor : public QWidget
{
    Q_OBJECT

public:
    CRelationshipEditor(CRelationship* relationship, CModelView* modelView, QWidget *parent = 0);
    ~CRelationshipEditor();

    CRelationship *relationship() const;
    //-----------------------
    void CheckAndUpdateForeginRows(bool PK);
    void DeleteOldForeginRows(CTable *startTable, CTable *endTable);
    void UpdateTable(CTable *startTable, CTable *endTable, bool PK);
    void ClearOldFR();
    int CheckRelationship();
    void ChooseClearTables(int numRel);
    void ChooseUpdateTables(int numRel);
    //-----------------------

private slots:
    void on_radioOneOne_toggled(bool checked);
    void on_radioOneMany_toggled(bool checked);
    void on_radioManyMany_toggled(bool checked);
    void on_radioAggregate_toggled(bool checked);
    void on_starTableMandatory_toggled(bool checked);
    void on_endTableMandatory_toggled(bool checked);
    void on_flipTables_clicked();
    void on_startTableChange_clicked();
    void on_endTableChange_clicked();
    void on_startTableEdit_clicked();
    void on_endTableEdit_clicked();

private:
    Ui::CRelationshipEditor *ui;

    CRelationship *_relationship;
    CModelView *_modelView;

    void synchronizeData();

signals:
    void dataChanged();
};

#endif // CRELATIONSHIPEDITOR_H
