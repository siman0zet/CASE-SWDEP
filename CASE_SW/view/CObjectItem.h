#ifndef COBJECTITEM_H
#define COBJECTITEM_H

#include <QGraphicsItem>

class CObject;

class CObjectItem : public QGraphicsItem
{
public:
    enum { Type = UserType };

    explicit CObjectItem(CObject *object);
    virtual ~CObjectItem();

    int type() const override;

    int id();
    QString name();
    CObject *object() const;
    QWidget *editor() const;
    void setEditor(QWidget *editor);

protected:
    CObject *_object;
    QWidget *_editor;

};

#endif // COBJECTITEM_H
