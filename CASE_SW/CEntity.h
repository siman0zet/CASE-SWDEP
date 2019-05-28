#ifndef CENTITY_H
#define CENTITY_H

//#include "CRow.h"
#include "CForeignRow.h"

class QApplication;

#include <QVector>
#include <QString>
#include <QDebug>
#include <QAbstractTableModel>

/* Класс сущности, представляет собой совокупность строк. Некоторые методы помещены в поле private,
 * чтобы они были доступны только из CDataModel. */

class CEntity : public QAbstractTableModel
{
    friend class CDataModel;
public:
    CEntity(int id, QApplication *_app);
    CEntity(CEntity &rhs);
    ~CEntity();

    void setName(QString _name); // Задать имя сущности
    QString getName() // Получить имя сущности
    {
        return name;
    }

    int addRow(); // Добавляет строку, возвращает её порядковый номер
    void remRow(int i); // Удаляет строку с номером i

    int getRowCount() const // Возвращает колличество обычных строк
    {
        return row_vec.size();
    }

    CRow *getRow(int i) // Возвращает указатель на i-ую строку
    {
        return &(row_vec[i]);
    }

    int getForeignRowCount() const // Возвращает колличество строк с внешним ключом
    {
        return foreign_row_vec.size();
    }

    CForeignRow *getForeignRow(int i) // Возвращает указатель на i-ую строку с внешним ключом
    {
        return &(foreign_row_vec[i]);
    }

    int getId() const
    {
        return id;
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());

    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());

    Qt::ItemFlags flags(const QModelIndex & index) const;

    void set_editable(bool flag);

    void cmplxURowDeleted(int row);

    void addCmplxUniquesGroup();
    bool addToCmplxUniqueGroup(int gr, int row);

    bool remCmplxUniquesGroup(int gr);
    bool remFromCmplxUniquesGroup(int gr, int row);

    QVector<QVector<int> > getComplexUniques() {return CmplxUnique;}

private:
    int addForeignRow(int table_id, int row_id); // Добавляет строку с внешним ключом, возвращает её порядковый номер
    void remForeignRow(int i); // Удаляет строку с внешним ключом с номером i

    int id;
    QString name;
    bool is_editable;

    QVector<CRow> row_vec;
    QVector<CForeignRow> foreign_row_vec;

    QApplication *app;

    QVector<QVector<int> > CmplxUnique;
};

#endif // CENTITY_H
