#ifndef CDETAILEDENTITYWIDGET_H
#define CDETAILEDENTITYWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>
#include <QComboBox>
#include <QLabel>
#include "CDetailedEntityView.h"
#include <QStyledItemDelegate>
#include <QCheckBox>
#include <QPainter>
#include <QApplication>
#include <QListView>

#include "CUniquesEditorWidget.h"

class CComboBoxDelegate : public QStyledItemDelegate
{
public:
    QWidget* createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
    {
        QStringList ls;
        ls << "integer" << "char" << "float" << "boolean" << "date" << "blob";
        QComboBox *cb = new QComboBox(parent);
        cb->addItems(ls);
        return cb;
    }

    void setEditorData(QWidget * editor, const QModelIndex & index) const
    {
        QComboBox *cb = static_cast<QComboBox*> (editor);

        QVariant v = index.data();

        if (!v.toString().compare("integer", Qt::CaseInsensitive))
            cb->setCurrentText("integer");
        else
        if (!v.toString().compare("char", Qt::CaseInsensitive))
            cb->setCurrentText("char");
        else
        if (!v.toString().compare("float", Qt::CaseInsensitive))
            cb->setCurrentText("float");
        else
        if (!v.toString().compare("boolean", Qt::CaseInsensitive))
            cb->setCurrentText("boolean");
        else
        if (!v.toString().compare("date", Qt::CaseInsensitive))
            cb->setCurrentText("date");
        else
        if (!v.toString().compare("blob", Qt::CaseInsensitive))
            cb->setCurrentText("blob");
    }

    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
    {
        QComboBox *cb = static_cast<QComboBox*> (editor);
        model->setData(index, cb->currentText());
    }

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
    {
        editor->setGeometry(option.rect);
    }
};

class CCheckBoxDelegate : public QStyledItemDelegate

{
public:
    QWidget* createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
    {
        QCheckBox *cb = new QCheckBox(parent);
        return cb;
    }

    void setEditorData(QWidget * editor, const QModelIndex & index) const
    {
        QCheckBox *cb = static_cast<QCheckBox*> (editor);

        cb->setChecked(index.data().toBool());
    }

    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
    {
        QCheckBox *cb = static_cast<QCheckBox*> (editor);
        model->setData(index, cb->isChecked());
    }

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
    {
        editor->setGeometry(option.rect.x() + option.rect.width()/2 - 10, option.rect.y(), option.rect.width(), option.rect.height());
    }

    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
    {
        bool checkValue;

        QStyleOptionButton BtnStyle;
        BtnStyle.state = 0;
        BtnStyle.state = QStyle::State_Enabled;

        if (!index.flags().testFlag(Qt::ItemIsEnabled))
           BtnStyle.state = QStyle::State_None;



        if(index.data().toBool() == true)
        {
        BtnStyle.state |= QStyle::State_On;
        checkValue = true;
        }else{
        BtnStyle.state |= QStyle::State_Off;
        checkValue = false;
        }


        BtnStyle.direction = QApplication::layoutDirection();
        BtnStyle.rect = QRect(option.rect.x() + option.rect.width()/2 - 10, option.rect.y(), option.rect.width(), option.rect.height());
        QApplication::style()->drawControl(QStyle::CE_CheckBox,&BtnStyle,painter );
        QApplication::style()->drawControl(QStyle::CE_CheckBox,&BtnStyle,painter );
    }

};

class CEntityView;
class CViewModel;

class CDetailedEntityWidget : public QWidget
{
    Q_OBJECT
public:
    CDetailedEntityWidget(CViewModel *_parent, CEntityView *_entity, bool is_editable = true);
    ~CDetailedEntityWidget();

    CEntityView* getEntity();

public slots:

    void addRow();
    void remRow();
    void uEditor();
    void entity_name_changed(QString _name);

signals:

    void closing(CDetailedEntityWidget* ptr);

protected:

    void closeEvent(QCloseEvent * event);

private:

    CViewModel* parent;
    CEntityView* entity;

    CDetailedEntityView* view;
    QLabel* entity_name;
    QLineEdit* line_edit;

    CComboBoxDelegate del;
    CCheckBoxDelegate cbdel;


    QPushButton *add_button, *rem_button, *unique_editor;
};

#endif // CDETAILEDENTITYWIDGET_H
