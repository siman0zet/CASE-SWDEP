#include "CModelView.h"
#include "mainwindow.h"
#include "CEntityItem.h"
#include "CRelationshipItem.h"
#include "resizedialog.h"
#include "CDataModel.h"
#include "CEntity.h"
#include "CRelationship.h"

#include <QMouseEvent>
#include <QDebug>
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

void CModelView::addEntity(const QPoint &pos)
{
    CEntity *entity = _dataModel->addEntity();
    CEntityItem *entityItem = new CEntityItem(entity);
    _entities.insert(entity->id(), entityItem);

    entityItem->setPos(pos);
    _scene->addItem(entityItem);

    returnToPointer();
}

void CModelView::addRelationship(int startId, int endId)
{
    // Here somewhere should be stated with relationship we create
    // either 1, 2, 3, 4 or default (from context menu action)
    CRelationship *relationship = _dataModel->addRelationship(startId, endId);
    if(relationship != NULL)
    {
        CRelationshipItem *relationshipItem = new CRelationshipItem(_entities.value(startId),
                                                                    _entities.value(endId));
        _relationships.insert(relationship->id(), relationshipItem);
        _scene->addItem(relationshipItem);
    }

    _entities.value(startId)->setSelectedForRelation(false);
    _entities.value(endId)->setSelectedForRelation(false);
    _entitiesToRelate.clear();
    returnToPointer();
}

void CModelView::showResizeDialog()
{
    ResizeDialog* dialog = new ResizeDialog(_width, _height, this);
    connect(dialog, SIGNAL(dialogFinished(int,int)), this, SLOT(changeSize(int,int)));
    dialog->exec();
}

void CModelView::changeSize(int w, int h)
{
    _width = w;
    _height = h;
    this->setFixedSize(_width + 2 * this->frameWidth(), _height + 2 * this->frameWidth());
    this->setSceneRect(0, 0, _width, _height);
}

void CModelView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        auto target = this->_scene->itemAt(event->pos(), QTransform());
        CEntityItem *targetEntity = dynamic_cast<CEntityItem *>(target);
        int count = countSelectedEntities();
        if(!targetEntity)
        {
            qDebug() << "empty space";

            if(count > 2)
            {
                qDebug() << "selected > 2";
                showContextMenu(event->pos(), true);
                return;
            }
            if(count == 2)
            {
                qDebug() << "selected = 2";
                showContextMenu(event->pos(), true, true);
                return;
            }
            if(count == 1)
            {
                qDebug() << "selected = 1";
                showTableContextMenu(event->pos(), "some table");
                return;
            }
            qDebug() << "selected = 0";
            showContextMenu(event->pos(), false);
            return;
        }
        else
        {
            qDebug() << "some target";
            if(targetEntity->isSelected())
            {
                if(count > 2)
                {
                    qDebug() << "selected > 2";
                    showContextMenu(event->pos(), true);
                    return;
                }
                if(count == 2)
                {
                    qDebug() << "selected = 2";
                    showContextMenu(event->pos(), true, true);
                    return;
                }
                if(count == 1)
                {
                    qDebug() << "selected = 1";
                    showTableContextMenu(event->pos(), "some table");
                    return;
                }
            }
            else
            {
                _scene->clearSelection();
                targetEntity->setSelected(true);
                qDebug() << "selected = 1";
                showTableContextMenu(event->pos(), "some table");
                return;
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
            this->addEntity(event->pos());
            return;
        }
        if(_tools.value(ONE_ONE) ||
           _tools.value(ONE_MANY) ||
           _tools.value(MANY_MANY) ||
           _tools.value(AGGREGATE))
        {
            QGraphicsItem *item = _scene->itemAt(event->pos(), this->transform());
            if(item)
            {
                if(item->type() == CEntityItem::Type)
                {
                    CEntityItem *entityItem = dynamic_cast<CEntityItem *>(item);
                    if(entityItem)
                    {
                        _entitiesToRelate.append(entityItem->id());
                        entityItem->setSelectedForRelation(true);
                        return;
                    }
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
        if(_entitiesToRelate.size() == 2)
        {
            addRelationship(_entitiesToRelate.first(), _entitiesToRelate.last());
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
    _entitiesToRelate.clear();
}

int CModelView::countSelectedEntities()
{
    int count = 0;
    foreach (CEntityItem *entity, _entities) {
        if(entity->isSelected())
            count++;
    }
    return count;
}

void CModelView::showContextMenu(const QPoint &pos, bool isEnabled, bool relationshipOption)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction actionRelate("Create Relationship", this);
    if(relationshipOption)
    {
        actionRelate.setEnabled(isEnabled);
        connect(&actionRelate, SIGNAL(triggered(bool)), this, SLOT(addRelationship()));
        contextMenu.addAction(&actionRelate);
    }

    QAction actionChangeColor("[TODO] Change color", this);
    actionChangeColor.setEnabled(false);
//    connect(&actionDelete, SIGNAL(triggered(bool)), this, SLOT(remove_selected_entityes()));
    contextMenu.addAction(&actionChangeColor);

    QAction actionDelete("Delete", this);
//    connect(&actionDelete, SIGNAL(triggered(bool)), this, SLOT(remove_selected_entityes()));
    actionDelete.setEnabled(isEnabled);
    contextMenu.addAction(&actionDelete);

    contextMenu.exec(mapToGlobal(pos));
}

void CModelView::showTableContextMenu(const QPoint &pos, QString tableName)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction actionEdit(QString("Edit '%1'").arg(tableName), this);
//    connect(&actionEdit, SIGNAL(triggered(bool)), this, SLOT(show_entity_detail()));
    contextMenu.addAction(&actionEdit);

    QAction actionChangeColor("[TODO] Change color", this);
    actionChangeColor.setEnabled(false);
//    connect(&actionDelete, SIGNAL(triggered(bool)), this, SLOT(remove_selected_entityes()));
    contextMenu.addAction(&actionChangeColor);

    QAction actionDelete("Delete", this);
//    connect(&actionDelete, SIGNAL(triggered(bool)), this, SLOT(remove_selected_entityes()));
    contextMenu.addAction(&actionDelete);

    contextMenu.exec(mapToGlobal(pos));
}

void CModelView::returnToPointer()
{
    activateTool(POINTER);
    if(_parent != 0)
        _parent->activateEditAction(POINTER);
}

void CModelView::addRelationship()
{
    CEntityItem *startItem = dynamic_cast<CEntityItem *>(_scene->selectedItems().first());
    CEntityItem *endItem = dynamic_cast<CEntityItem *>(_scene->selectedItems().last());
    if(startItem && endItem && startItem != endItem)
    {
        int startId = startItem->id();
        int endId = endItem->id();
        addRelationship(startId, endId);
    }
}
