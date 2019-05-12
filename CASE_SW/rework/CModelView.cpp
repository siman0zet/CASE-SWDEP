#include "CModelView.h"
#include "CEntityItem.h"
#include "resizedialog.h"

#include <QMouseEvent>
#include <QDebug>
#include <QMenu>

CModelView::CModelView(QWidget *parent) :
    QGraphicsView(parent),
    _scene(new QGraphicsScene(this)),
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

    addEntity();
}

CModelView::~CModelView()
{
    delete _scene;
}

void CModelView::activateTool(CModelView::cursorToolType type)
{
    deactivateTools();
    _tools.insert(type, true);
}

void CModelView::addEntity()
{
    QGraphicsItem *item = new CEntityItem();
    item->setPos(20, 20);
    _scene->addItem(item);
    _entities.insert(1, item);

    QGraphicsItem *item2 = new CEntityItem();
    item2->setPos(100, 250);
    _scene->addItem(item2);
    _entities.insert(2, item2);

    QGraphicsItem *item3 = new CEntityItem();
    item3->setPos(200, 15);
    _scene->addItem(item3);
    _entities.insert(3, item3);
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
                showTableRelationshipContextMenu(event->pos());
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
                    showTableRelationshipContextMenu(event->pos());
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
    QGraphicsView::mousePressEvent(event);
}

void CModelView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
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
}

int CModelView::countSelectedEntities()
{
    int count = 0;
    foreach (QGraphicsItem *entity, _entities) {
        if(entity->isSelected())
            count++;
    }
    return count;
}

void CModelView::showContextMenu(const QPoint &pos, bool isEnabled)
{
    QMenu contextMenu(tr("Context menu"), this);

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

    QAction actionDelete("Delete", this);
//    connect(&actionDelete, SIGNAL(triggered(bool)), this, SLOT(remove_selected_entityes()));
    contextMenu.addAction(&actionDelete);

    contextMenu.exec(mapToGlobal(pos));
}

void CModelView::showTableRelationshipContextMenu(const QPoint &pos)
{
    QMenu contextMenu(tr("Context menu"), this);

    QAction actionRelate("Create Relationship", this);
//    connect(&actionRelate, SIGNAL(triggered(bool)), this, SLOT(connect_selected_entityes()));
    contextMenu.addAction(&actionRelate);

    QAction actionDelete("Delete", this);
//    connect(&actionDelete, SIGNAL(triggered(bool)), this, SLOT(remove_selected_entityes()));
    contextMenu.addAction(&actionDelete);

    contextMenu.exec(mapToGlobal(pos));
}
