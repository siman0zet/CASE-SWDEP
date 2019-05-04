#ifndef CDETAILEDENTITYVIEW_H
#define CDETAILEDENTITYVIEW_H

#include <QTableView>

#include "CEntity.h"

/* CDetailedEntityView - виджет для более детального отображения сущности */

class CEntityView;
class CViewModel;
class CDetailedEntityWidget;

class CDetailedEntityView : public QTableView
{
public:
    CDetailedEntityView(CDetailedEntityWidget *_parent, CViewModel* _view_model, CEntityView* _entity);
    ~CDetailedEntityView();

    void rowCntrChanged(int oldCount, int newCount);

private:

    CEntityView* entity;
    CDetailedEntityWidget* parent;
    CViewModel* view_model;
    CEntity* model;
};

#endif // CDETAILEDENTITYVIEW_H
