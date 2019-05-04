#include "CDetailedEntityView.h"

#include "CViewModel.h"
#include "CEntityView.h"
#include "CDetailedEntityWidget.h"

CDetailedEntityView::CDetailedEntityView(CDetailedEntityWidget* _parent,
                                         CViewModel *_view_model, CEntityView *_entity): QTableView(_parent),
    entity(_entity), parent(_parent), view_model(_view_model)
{
    model = entity->getEntity();
    setModel(model);

    show();
}

CDetailedEntityView::~CDetailedEntityView()
{

}

void CDetailedEntityView::rowCntrChanged(int oldCount, int newCount)
{
    rowCountChanged(oldCount, newCount);
}

