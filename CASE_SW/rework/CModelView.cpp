#include "CModelView.h"
#include "mainwindow.h"
#include "CTableItem.h"
#include "CRelationshipItem.h"
#include "resizedialog.h"
#include "CDataModel.h"
#include "CTable.h"
#include "CRelationship.h"
#include "CRelationshipEditor.h"

#include <QMouseEvent>
#include <QMenu>

CModelView::CModelView(QWidget *parent) :
    QGraphicsView(parent),
    _parent((MainWindow *)parent),
    _scene(new QGraphicsScene(this)),
    _dataModel(new CDataModel()),
    _width(400),
    _height(300)

{
    this->setFocusPolicy(Qt::StrongFocus);
    this->setRenderHint(QPainter::Antialiasing, true);
    this->setDragMode(QGraphicsView::RubberBandDrag);
    this->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    this->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    this->setScene(_scene);
    QBrush brush (Qt::white, Qt::SolidPattern);
    _scene->setBackgroundBrush(brush);

    connect(_dataModel, SIGNAL(relationshipRemoved(int)), this, SLOT(removeRelationship(int)));
}

CModelView::~CModelView()
{
    delete _scene;
    delete _dataModel;
}

void CModelView::activateTool(CModelView::cursorToolType type)
{
    deactivateTools();
    _tools.insert(type, true);
}

void CModelView::addTable(const QPoint &pos)
{
    CTable *table = _dataModel->addTable();
    CTableItem *tableItem = new CTableItem(table);
    _tables.insert(table->id(), tableItem);

    tableItem->setPos(pos);
    _scene->addItem(tableItem);

    returnToPointer();
}

void CModelView::addRelationship(int startId, int endId)
{
    CRelationship *relationship = _dataModel->addRelationship(startId, endId);
    if(relationship != NULL)
    {
        if(_tools.value(ONE_ONE))
        {
            relationship->setStartType(CRelationship::ONE);
            relationship->setEndType(CRelationship::ONE);
            relationship->setStartMandatory(true);
            relationship->setEndMandatory(true);
        }
        if(_tools.value(ONE_MANY))
        {
            relationship->setStartType(CRelationship::ONE);
            relationship->setEndType(CRelationship::MANY);
            relationship->setStartMandatory(true);
            relationship->setEndMandatory(true);
        }
        if(_tools.value(MANY_MANY))
        {
            relationship->setStartType(CRelationship::MANY);
            relationship->setEndType(CRelationship::MANY);
            relationship->setStartMandatory(true);
            relationship->setEndMandatory(true);
        }
        if(_tools.value(AGGREGATE))
        {
            relationship->setStartType(CRelationship::ONE);
            relationship->setEndType(CRelationship::AGGREGATE);
            relationship->setStartMandatory(true);
            relationship->setEndMandatory(true);
        }
        CRelationshipItem *relationshipItem = new CRelationshipItem(_tables.value(startId),
                                                                    _tables.value(endId),
                                                                    relationship);
        _relationships.insert(relationship->id(), relationshipItem);
        _scene->addItem(relationshipItem);
    }
    _tables.value(startId)->setSelectedForRelation(false);
    _tables.value(endId)->setSelectedForRelation(false);
    _tablesToRelate.clear();
    returnToPointer();
}

void CModelView::removeItem(const QPoint &pos)
{
    QList<QGraphicsItem *> items;
    QGraphicsItem *item = _scene->itemAt(pos, this->transform());
    if(item)
    {
        items.append(item);
        removeItems(items);
    }
}

void CModelView::removeItems(QList<QGraphicsItem *> items)
{
    QList<CObject *> objects;
    foreach (QGraphicsItem *item, items) {
        CObjectItem *objectItem = (CObjectItem *)item;
        switch (objectItem->type()) {
        case CTableItem::Type:
            _tables.remove(objectItem->id());
            break;
        case CRelationshipItem::Type:
            _relationships.remove(objectItem->id());
            break;
        }
        objects.append(objectItem->object());
        _scene->removeItem(item);
    }
    _dataModel->removeObjects(objects);
    returnToPointer();
}

void CModelView::showResizeDialog()
{
    ResizeDialog* dialog = new ResizeDialog(_width, _height, this);
    connect(dialog, SIGNAL(dialogFinished(int,int)), this, SLOT(changeSize(int,int)));
    dialog->exec();
}

void CModelView::flipTables(int id)
{
    _dataModel->flipTables(id);

    CTableItem *startItem = _relationships.value(id)->startItem();
    _relationships.value(id)->setStartItem(_relationships.value(id)->endItem());
    _relationships.value(id)->setStartItem(startItem);

    _relationships.value(id)->updatePosition();
}

void CModelView::changeSize(int w, int h)
{
    _width = w;
    _height = h;
    this->setFixedSize(_width + 2 * this->frameWidth(), _height + 2 * this->frameWidth());
    this->setSceneRect(0, 0, _width, _height);
}

void CModelView::removeRelationship(int id)
{
    foreach (CTableItem *tableItem, _tables.values()) {
        tableItem->removeRelationship(id);
    }
    // if relationship was already deleted in cascade
    if(_relationships.value(id) != 0)
    {
        _scene->removeItem(_relationships.value(id));
        delete _relationships.value(id);
        _relationships.remove(id);
    }
}

void CModelView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        CObjectItem *target = (CObjectItem *)this->_scene->itemAt(event->pos(), QTransform());
        if(target)
        {
            _scene->clearSelection();
            target->setSelected(true);
            if(target->type() == CTableItem::Type)
            {
                showTableContextMenu(event->pos(), target->name());
                return;
            }
            if(target->type() == CRelationshipItem::Type)
            {
                showRelationshipContextMenu(event->pos());
                return;
            }
        }
        else
        {
            //empty space
            QList<QGraphicsItem *> items = _scene->selectedItems();
            int selectedItemsCount = items.size();
            switch (selectedItemsCount) {
            case 0:
                {
                    showContextMenu(event->pos(), false);
                    break;
                }
            case 1:
                {
                    if(items.last()->type() == CRelationshipItem::Type)
                    {
                        showRelationshipContextMenu(event->pos());
                    }
                    else
                    {
                        showTableContextMenu(event->pos(), dynamic_cast<CObjectItem *>(items.last())->name());
                    }
                    break;
                }
            case 2:
                {
                    if(CTableItem::Type == items.first()->type() && CTableItem::Type == items.last()->type())
                        showContextMenu(event->pos(), true, true, true);
                    else
                        showContextMenu(event->pos(), true);
                    break;
                }
            default:
                {
                    bool sameItems = true;
                    foreach (QGraphicsItem *item, _scene->selectedItems()) {
                        if(item->type() == CRelationshipItem::Type)
                        {
                            sameItems = false;
                            break;
                        }
                    }
                    if(!sameItems)
                    {
                        int tableCount = 0;
                        foreach (QGraphicsItem *item, items) {
                            if(CTableItem::Type == item->type())
                                tableCount++;
                        }
                        if(tableCount == 2)
                        {
                            showContextMenu(event->pos(), true, false, true);
                            return;
                        }
                    }
                    showContextMenu(event->pos(), true, sameItems);
                    break;
                }
            }
        }
    }
    else
    {
        if(_tools.value(POINTER))
        {
            QGraphicsView::mousePressEvent(event);
            return;
        }
        if(_tools.value(CREATE))
        {
            this->addTable(event->pos());
            return;
        }
        if(_tools.value(DELETE))
        {
            this->removeItem(event->pos());
            return;
        }
        if(_tools.value(ONE_ONE) ||
           _tools.value(ONE_MANY) ||
           _tools.value(MANY_MANY) ||
           _tools.value(AGGREGATE))
        {
            CTableItem *item = (CTableItem *)_scene->itemAt(event->pos(), this->transform());
            if(item)
            {
                if(item->type() == CTableItem::Type)
                {
                    _tablesToRelate.append(item->id());
                    item->setSelectedForRelation(true);
                    return;
                }
            }
        }
    }
}

void CModelView::mouseReleaseEvent(QMouseEvent *event)
{
    if(_tools.value(ONE_ONE) ||
       _tools.value(ONE_MANY) ||
       _tools.value(MANY_MANY) ||
       _tools.value(AGGREGATE))
    {
        if(_tablesToRelate.size() == 2)
        {
            addRelationship(_tablesToRelate.first(), _tablesToRelate.last());
            return;
        }
    }
    else
    {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void CModelView::mouseDoubleClickEvent(QMouseEvent *event)
{
    CObjectItem *target = (CObjectItem *)this->_scene->itemAt(event->pos(), QTransform());
    if(target)
        showObjectEditor(target);
    QGraphicsView::mouseDoubleClickEvent(event);
}

void CModelView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
}

void CModelView::deactivateTools()
{
    for (int i = POINTER; i <= AGGREGATE; i++)
    {
        _tools.insert(static_cast<cursorToolType>(i), false);
    }
    if(_tablesToRelate.size())
    {
        foreach (int id, _tablesToRelate) {
            _tables.value(id)->setSelectedForRelation(false);
        }
        _tablesToRelate.clear();
    }
}

void CModelView::showContextMenu(const QPoint &pos, bool isEnabled, bool colorOption, bool relationOption)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction actionRelate("Create Relationship", this);
    if(relationOption)
    {
        actionRelate.setEnabled(isEnabled);
        connect(&actionRelate, SIGNAL(triggered(bool)), this, SLOT(addRelationship()));
        contextMenu.addAction(&actionRelate);
    }

    QAction actionChangeColor("[TODO] Change Color", this);
    if(colorOption)
    {
        actionChangeColor.setEnabled(false);
//        connect(&actionDelete, SIGNAL(triggered(bool)), this, SLOT(remove_selected_entityes()));
        contextMenu.addAction(&actionChangeColor);
    }

    QAction actionDelete("Delete", this);
    connect(&actionDelete, SIGNAL(triggered(bool)), this, SLOT(removeItems()));
    actionDelete.setEnabled(isEnabled);
    contextMenu.addAction(&actionDelete);

    contextMenu.exec(mapToGlobal(pos));
}

void CModelView::showTableContextMenu(const QPoint &pos, QString tableName)
{
    QMenu contextMenu(tr("Table context menu"), this);

    QAction actionEdit(QString("Edit '%1'").arg(tableName), this);
    connect(&actionEdit, SIGNAL(triggered(bool)), this, SLOT(showObjectEditor()));
    contextMenu.addAction(&actionEdit);

    QAction actionChangeColor("[TODO] Change Color", this);
    actionChangeColor.setEnabled(false);
//    connect(&actionDelete, SIGNAL(triggered(bool)), this, SLOT(remove_selected_entityes()));
    contextMenu.addAction(&actionChangeColor);

    QAction actionDelete("Delete", this);
    connect(&actionDelete, SIGNAL(triggered(bool)), this, SLOT(removeItems()));
    contextMenu.addAction(&actionDelete);

    contextMenu.exec(mapToGlobal(pos));
}

void CModelView::showRelationshipContextMenu(const QPoint &pos)
{
    QMenu contextMenu(tr("Relationship context menu"), this);

    QAction actionEdit("Edit Relationship");
    connect(&actionEdit, SIGNAL(triggered(bool)), this, SLOT(showObjectEditor()));
    contextMenu.addAction(&actionEdit);

    QAction actionDelete("Delete", this);
    connect(&actionDelete, SIGNAL(triggered(bool)), this, SLOT(removeItems()));
    contextMenu.addAction(&actionDelete);

    contextMenu.exec(mapToGlobal(pos));
}

void CModelView::returnToPointer()
{
    activateTool(POINTER);
    if(_parent != 0)
        _parent->activateEditAction(POINTER);
}

void CModelView::showObjectEditor(CObjectItem *objectItem)
{
    if(objectItem->editor() != NULL)
    {
        objectItem->editor()->show();
        objectItem->editor()->raise();
        objectItem->editor()->activateWindow();
    }
    else
    {
        switch (objectItem->type()) {
        case CTableItem::Type:
            {
                break;
            }
        case CRelationshipItem::Type:
            {
                CRelationshipEditor *editor = new CRelationshipEditor((CRelationship *)objectItem->object(), this);
                connect(editor, SIGNAL(dataChanged()), this, SLOT(update()));
                objectItem->setEditor((QWidget *)editor);
                editor->show();
                break;
            }
        }
    }
}

void CModelView::addRelationship()
{
    CObjectItem *startItem = (CObjectItem *)_scene->selectedItems().first();
    CObjectItem *endItem = (CObjectItem *)_scene->selectedItems().last();
    if(startItem && endItem && startItem != endItem)
    {
        int startId = startItem->id();
        int endId = endItem->id();
        addRelationship(startId, endId);
    }
}

void CModelView::removeItems()
{
    if(_scene->selectedItems().size())
        removeItems(_scene->selectedItems());
}

void CModelView::showObjectEditor()
{
    if(_scene->selectedItems().size())
    {
        CObjectItem *objectItem = (CObjectItem *)_scene->selectedItems().last();
        showObjectEditor(objectItem);
    }
}
