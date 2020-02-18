#include "cmodelview.h"
#include "crelationshipitem.h"
#include "ctableitem.h"
#include "pmodelview.h"

#include <model/cdatamodel.h>
#include <model/crelationship.h>
#include <model/ctable.h>

#include <dialog/resizedialog.h>

PModelView::PModelView(CModelView *cModelView, QWidget *parent) :
    QGraphicsView(parent),
    _scene(new QGraphicsScene(this)),
    _width(400),
    _height(300),
    _dataModel(0),
    _name(cModelView->name())
{
    this->setCacheMode(QGraphicsView::CacheBackground);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setRenderHint(QPainter::Antialiasing, true);
    this->setDragMode(QGraphicsView::RubberBandDrag);
    this->setOptimizationFlags(QGraphicsView::DontSavePainterState |
                               QGraphicsView::DontAdjustForAntialiasing);
    this->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->changeSize(cModelView->width(), cModelView->height());

    CDataModel *pModel = cModelView->dataModel()->convertToPhysical();
    if(pModel != 0)
    {
        populateScene(pModel, cModelView);
        _dataModel = pModel;
    }

    this->setScene(_scene);
    QBrush brush (Qt::white, Qt::SolidPattern);
    _scene->setBackgroundBrush(brush);
}

PModelView::~PModelView()
{
    delete _scene;
}

void PModelView::changeSize(int w, int h)
{
    _width = w;
    _height = h;
    this->setFixedSize(_width + 2 * this->frameWidth(), _height + 2 * this->frameWidth());
    this->setSceneRect(0, 0, _width, _height);
}

QString PModelView::name() const
{
    return _name;
}

void PModelView::showResizeDialog()
{
    ResizeDialog *dialog = new ResizeDialog(_width, _height, this);
    connect(dialog, SIGNAL(dialogFinished(int,int)), this, SLOT(changeSize(int,int)));
    dialog->exec();
}

CDataModel *PModelView::dataModel() const
{
    return _dataModel;
}

void PModelView::populateScene(CDataModel *pModel, CModelView *cModelView)
{
    foreach (CTable *table, pModel->tables()) {
        if(cModelView->tables().contains(table->name()))
//        QMap<QString, CTableItem *>::const_iterator i = cModelView->tables().find(table->name());
//        if (i != cModelView->tables().end() && i.key() == table->name())
        {
            CTableItem *cTableItem = cModelView->tables().value(table->name());
            CTableItem *pTableItem = new CTableItem(table);
            pTableItem->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
            pTableItem->setPos(cTableItem->pos());
            _tables.insert(pTableItem->name(), pTableItem);
            _scene->addItem(pTableItem);
        }
    }
    foreach (CTable *table, pModel->tables()) {
//        QMap<QString, CTableItem *>::const_iterator i = cModelView->tables().find(table->name());
//        if (i == cModelView->tables().end() && i.key() == table->name())
        if(!cModelView->tables().contains(table->name()))
        {
            // If no table analog was found in the conceptual model
            // then we came across conduit table
            // We execute this in separate foreach and not in the same one
            // to ensure that all tables with conceptual conterparts have been added
            CTableItem *pTableItem = new CTableItem(table);
            pTableItem->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
            pTableItem->setWidth(200);

            // Conduit table always has only 2 relationships
            // And the start table of each relationship is the conduit table itself
            QString conduitStartItemName = table->relationships().first()->endTable()->name();
            QString conduitEndItemName = table->relationships().last()->endTable()->name();
            CTableItem *conduitStartItem = _tables.value(conduitStartItemName);
            CTableItem *conduitEndItem = _tables.value(conduitEndItemName);
            pTableItem->setPos(calculateConduitPoint(conduitStartItem, conduitEndItem));

            _tables.insert(pTableItem->name(), pTableItem);
            _scene->addItem(pTableItem);
        }
    }
    foreach (CRelationship *relationship, pModel->relationships()) {
        CRelationshipItem *relationshipItem = new CRelationshipItem(
                    _tables.value(relationship->startTable()->name()),
                    _tables.value(relationship->endTable()->name()),
                    relationship);
        relationshipItem->setCacheMode(QGraphicsItem::DeviceCoordinateCache);
        _relationships.insert(relationshipItem->name(), relationshipItem);
        _scene->addItem(relationshipItem);
    }
}

QPointF PModelView::calculateConduitPoint(CTableItem *startItem, CTableItem *endItem)
{
    const int conduitWidth = 200;
    QPointF startPoint = QPointF(startItem->pos().x() + startItem->width(), startItem->pos().y());

    int width = 100 + conduitWidth + 100;
    int height = startItem->height();

    QRectF rect(startPoint.x(), startPoint.y(), width, height);

    endItem->setPos(rect.topRight());

    return QPointF(startPoint.x() + rect.width() / 2 - conduitWidth / 2,
                   rect.topLeft().y());
}
