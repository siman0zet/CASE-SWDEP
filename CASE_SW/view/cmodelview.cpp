#include "cmodelview.h"
#include "../main/mainwindow.h"
#include "ctableitem.h"
#include "crelationshipitem.h"
#include "../dialog/resizedialog.h"
#include "../model/cdatamodel.h"
#include "../model/ctable.h"
#include "../model/crelationship.h"
#include "../editor/crelationshipeditor.h"
#include "../dialog/changetabledialog.h"
#include "../editor/ctableeditor.h"
#include "../model/cforeignrow.h"

#include <QMouseEvent>
#include <QMenu>
#include <QTextStream>

CModelView::CModelView(const QString &name, const QString &path, QWidget *parent) :
    QGraphicsView(parent),
    _name(name),
    _path(path),
    _parent((MainWindow *)parent),
    _scene(new QGraphicsScene(this)),
    _dataModel(new CDataModel()),
    _width(400),
    _height(300)

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

    this->setScene(_scene);
    QBrush brush (Qt::white, Qt::SolidPattern);
    _scene->setBackgroundBrush(brush);

    connect(_dataModel, SIGNAL(tableNameChanged(QString,QString)), this, SLOT(changeTableName(QString,QString)));
}

CModelView::~CModelView()
{
    delete _scene;
    delete _dataModel;
}

bool CModelView::saveToFile(const QString &path) const
{
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    QTextStream outputStream(&file);
    QString text = this->exportToText();
    text.replace("\n", "\r\n");
    outputStream << text;
    file.close();
    return true;
}

bool CModelView::loadFromFile(const QString &path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QTextStream inputStream(&file);
    this->importFromTextStream(inputStream);
    file.close();
    return true;
}

void CModelView::activateTool(const TOOL_TYPE &type)
{
    deactivateTools();
    _tools.insert(type, true);
}

void CModelView::addTable(const QPoint &pos)
{
    CTable *table = _dataModel->addTable();
    CTableItem *tableItem = new CTableItem(table);
    _tables.insert(table->name(), tableItem);

    tableItem->setPos(pos);
    _scene->addItem(tableItem);

    returnToPointer();
}

void CModelView::addRelationship(const QString &startName, const QString &endName)
{
    if(startName == endName)
        return;
    CRelationship *relationship = _dataModel->addRelationship(startName, endName);
    if(relationship != NULL)
    {
        if(_tools.value(ONE_ONE))
        {
            relationship->setStartType(CRelationship::ONE);
            relationship->setEndType(CRelationship::ONE);
            relationship->setStartMandatory(true);
            relationship->setEndMandatory(true);
            addForeginRows(relationship->startTable(), relationship->endTable(),false);    //+
        }
        if(_tools.value(ONE_MANY))
        {
            relationship->setStartType(CRelationship::ONE);
            relationship->setEndType(CRelationship::MANY);
            relationship->setStartMandatory(true);
            relationship->setEndMandatory(true);
            addForeginRows(relationship->startTable(), relationship->endTable(), false);    //+
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
            addForeginRows(relationship->startTable(), relationship->endTable(), true);    //+
        }


        CRelationshipItem *relationshipItem = new CRelationshipItem(_tables.value(startName),
                                                                    _tables.value(endName),
                                                                    relationship);
        _relationships.insert(relationship->name(), relationshipItem);
        _scene->addItem(relationshipItem);
    }
    _tables.value(startName)->setSelectedForRelation(false);
    _tables.value(endName)->setSelectedForRelation(false);
    _tablesToRelate.clear();
    returnToPointer();
}

//----------------
void CModelView::addForeginRows(CTable *startTable, CTable *endTable, bool PK)
{
    QList<CRow *> startTablePK = startTable->primaryKey();
    QList<CForeignRow *> startTableFK = startTable->foreingRows();
    if(startTablePK.isEmpty() && startTableFK.isEmpty())
        return;
    foreach (CRow *row, startTablePK) {
        CForeignRow *fRow = new CForeignRow(row, startTable->name());
        if(PK)
            fRow->setPrimaryKey(true);
        endTable->addForeignRow(fRow);
    }
    foreach (CForeignRow *frow, startTableFK) {
        endTable->addForeignRow(frow);
    }
}
//----------------

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
    // Sort objects so that any relationship that needs to be deleted (cascade included)
    // is deleted before any table
    QList<CObjectItem *> sortList;
    foreach (QGraphicsItem *item, items) {
        CObjectItem *object = (CObjectItem *)item;
        if(CTableItem::Type == object->type())
        {
            foreach (CRelationshipItem *relationship, _tables.value(object->name())->relationships()) {
                int index = sortList.indexOf(relationship);
                if(index != -1)
                    sortList.removeAt(index);
                sortList.push_front(relationship);
            }
            sortList.push_back(object);
            continue;
        }
        if(CRelationshipItem::Type == object->type())
        {
            int index = sortList.indexOf(object);
            if(index != -1)
                sortList.removeAt(index);
            sortList.push_front(object);
            continue;
        }
    }
    QList<CObject *> objects;
    //----------------------------------------
    //QList<CForeignRow *> TableFRows;
    //CTable *tmpTable;
    //----------------------------------------
    foreach (CObjectItem *objectItem, sortList) {
        switch (objectItem->type()) {
        case CRelationshipItem::Type:
            //----------------------------------------
            if(_relationships.value(objectItem->name())->relationship()->startType() == CRelationship::ONE &&
                    _relationships.value(objectItem->name())->relationship()->endType() == CRelationship::ONE)
            {
                if(_relationships.value(objectItem->name())->relationship()->endMandatory())
                {
                    RemoveForeginRows(_tables.value(_relationships.value(objectItem->name())->startItem()->name())->table(),
                                      _tables.value(_relationships.value(objectItem->name())->endItem()->name())->table());
                }
                else
                {
                       if(_relationships.value(objectItem->name())->relationship()->startMandatory())
                           RemoveForeginRows(_tables.value(_relationships.value(objectItem->name())->endItem()->name())->table(),
                                             _tables.value(_relationships.value(objectItem->name())->startItem()->name())->table());
                       else break;
                }
            }
            if(_relationships.value(objectItem->name())->relationship()->startType() == CRelationship::ONE &&
               _relationships.value(objectItem->name())->relationship()->endType() == CRelationship::MANY)
            {
                if(_relationships.value(objectItem->name())->relationship()->startMandatory())
                    RemoveForeginRows(_tables.value(_relationships.value(objectItem->name())->startItem()->name())->table(),
                                      _tables.value(_relationships.value(objectItem->name())->endItem()->name())->table());
                else
                    RemoveForeginRows(_tables.value(_relationships.value(objectItem->name())->endItem()->name())->table(),
                                      _tables.value(_relationships.value(objectItem->name())->startItem()->name())->table());
            }
            if(_relationships.value(objectItem->name())->relationship()->startType() == CRelationship::ONE &&
               _relationships.value(objectItem->name())->relationship()->endType() == CRelationship::AGGREGATE)
            {
                if(_relationships.value(objectItem->name())->relationship()->startMandatory())
                    RemoveForeginRows(_tables.value(_relationships.value(objectItem->name())->startItem()->name())->table(),
                                      _tables.value(_relationships.value(objectItem->name())->endItem()->name())->table());
            }
            /*tmpTable = _tables.value(_relationships.value(objectItem->name())->endItem()->name())->table();
            TableFRows = tmpTable->foreingRows();
            foreach(CForeignRow *fRowET,TableFRows)
            {
                // поиск и удаление foreginRow по исходной тиблице (откуда берётся внешний ключ,
                // или проверка на соответствие внешних ключей с и сходной таблицей если идёт n-ая агрегация)
                if(fRowET->tableName() == _relationships.value(objectItem->name())->startItem()->name() || _tables.value(_relationships.value(objectItem->name())->startItem()->name())->table()->SearchFROnName(fRowET->name()))
                    tmpTable->removeForeginRow(fRowET);
            }
            TableFRows.clear();*/
            //----------------------------------------
            _relationships.value(objectItem->name())->startItem()->removeRelationship((CRelationshipItem *)objectItem);
            _relationships.value(objectItem->name())->endItem()->removeRelationship((CRelationshipItem *)objectItem);
            _relationships.remove(objectItem->name());
            break;
        case CTableItem::Type:
            foreach (CRelationshipItem *relationshipItem, _tables.value(objectItem->name())->relationships()) {
                _relationships.remove(relationshipItem->name());
                _scene->removeItem(relationshipItem);
            }
            _tables.value(objectItem->name())->removeRelationships();
            _tables.remove(objectItem->name());
            break;
        }
        objects.append(objectItem->object());
        _scene->removeItem(objectItem);
    }
    _dataModel->removeObjects(objects);
    returnToPointer();
}

//----------------------------------------
void CModelView::RemoveForeginRows(CTable *startTable, CTable *endTable)
{
    QList<CForeignRow *> TableFRows;

    TableFRows = endTable->foreingRows();
    foreach(CForeignRow *fRowET,TableFRows)
    {
        // поиск и удаление foreginRow по исходной тиблице (откуда берётся внешний ключ,
        // или проверка на соответствие внешних ключей с и сходной таблицей если идёт n-ая агрегация)
        if(fRowET->tableName() == startTable->name() || startTable->SearchFROnName(fRowET->name()))
            endTable->removeForeginRow(fRowET);
    }
    TableFRows.clear();
}
//----------------------------------------

void CModelView::showResizeDialog()
{
    ResizeDialog *dialog = new ResizeDialog(_width, _height, this);
    connect(dialog, SIGNAL(dialogFinished(int,int)), this, SLOT(changeSize(int,int)));
    dialog->exec();
}

void CModelView::showChangeTableDialog(const QString &relationshipName, bool start) const
{
    ChangeTableDialog *dialog = new ChangeTableDialog(_dataModel->listTables(),
                                                      (CRelationship *)_relationships.value(relationshipName)->object(),
                                                      start);
    connect(dialog, SIGNAL(dialogFinished(QString,QString,bool)), this, SLOT(changeTable(QString,QString,bool)));
    dialog->exec();
}

void CModelView::flipTables(const QString &relationshipName)
{
    _dataModel->flipTables(relationshipName);

    CTableItem *startItem = _relationships.value(relationshipName)->startItem();
    _relationships.value(relationshipName)->setStartItem(_relationships.value(relationshipName)->endItem());
    _relationships.value(relationshipName)->setEndItem(startItem);
}

CTableItem *CModelView::tableItem(const QString &name) const
{
    return _tables.value(name);
}

void CModelView::changeSize(int w, int h)
{
    _width = w;
    _height = h;
    this->setFixedSize(_width + 2 * this->frameWidth(), _height + 2 * this->frameWidth());
    this->setSceneRect(0, 0, _width, _height);
}

void CModelView::changeTable(const QString &relationshipName, const QString &tableName, bool start)
{
    _dataModel->changeRelationshipTable(relationshipName, tableName, start);

    if(start)
    {
        _relationships.value(relationshipName)->setStartItem(_tables.value(tableName));
    }
    else
    {
        _relationships.value(relationshipName)->setEndItem(_tables.value(tableName));
    }
}

void CModelView::changeTableName(const QString &oldName, const QString &newName)
{
    _tables.insert(newName, _tables.value(oldName));
    _tables.remove(oldName);
    foreach (CRelationshipItem *relationshipItem, _tables.value(newName)->relationships()) {
        if(relationshipItem->startItem() == _tables.value(newName))
        {
            _relationships.remove(relationshipItem->name());
            _relationships.insert(QString("%1_%2")
                                  .arg(newName)
                                  .arg(relationshipItem->endItem()->name()),
                                  relationshipItem);
        }
        if(relationshipItem->endItem() == _tables.value(newName))
        {
            _relationships.remove(relationshipItem->name());
            _relationships.insert(QString("%1_%2")
                                  .arg(relationshipItem->startItem()->name())
                                  .arg(newName),
                                  relationshipItem);
        }
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
                    _tablesToRelate.append(item->name());
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

QString CModelView::exportToText() const
{
    QString text;
    /*# CASE-SWDEP CDM File: 'file_name.file_extension'
     * model w h
     */
    text += QString("CASE-SWDEP CDM File: '%1.cdmod'\n"
                    "model %2 %3\n")
            .arg(this->name())
            .arg(this->width())
            .arg(this->height());

    /*  table Table_name
     *  pos x y
     *  size w h
     *  data_from_table
     */
    foreach (const CTableItem *tableItem, _tables) {
        text += QString("\ntable %1\n"
                        "pos %2 %3\n"
                        "size %4 %5\n")
                .arg(tableItem->table()->name())
                .arg(tableItem->pos().x())
                .arg(tableItem->pos().y())
                .arg(tableItem->width())
                .arg(tableItem->height());
        text += tableItem->object()->exportDataToText();
    }
    foreach (const CRelationshipItem *relationshipItem, _relationships) {
        text += relationshipItem->object()->exportDataToText();
    }
    //----------------------------------------------------------
    text += '\n';
    foreach (CTableItem *tableItem, _tables) {
        text += tableItem->table()->exportForeginKeysToText();
    }
    //----------------------------------------------------------
    return text;
}

void CModelView::importFromTextStream(QTextStream &input)
{
    while(!input.atEnd())
    {
        QStringList strList = input.readLine().split(" ");
        /*# CASE-SWDEP CDM File: 'file_name.file_extension'
         * model w h
         *
         *  table Table_name
         *  pos x y
         *  size w h
         *  data_from_table
         */
        if(strList.at(0) == "model")
        {
            if(strList.size() == 3)
                this->changeSize(strList.at(1).toInt(), strList.at(2).toInt());
            continue;
        }
        if(strList.at(0) == "table")
        {
            if(strList.size() == 2)
            {
                CTable *table = new CTable(strList.at(1));
                CTableItem *tableItem = new CTableItem(table);

                strList = input.readLine().split(" ");
                if(strList.at(0) == "pos")
                    if(strList.size() == 3)
                        tableItem->setPos(strList.at(1).toInt(), strList.at(2).toInt());

                strList = input.readLine().split(" ");
                if(strList.at(0) == "size")
                    if(strList.size() == 3)
                    {
                        tableItem->setWidth(strList.at(1).toInt());
                        tableItem->setHeight(strList.at(2).toInt());
                    }
                table->importFromTextStream(input);
                _dataModel->addTable(table);
                _tables.insert(table->name(), tableItem);
                _scene->addItem(tableItem);
            }
            continue;
        }
        if(strList.at(0) == "relationship")
        {
            if(strList.size() == 3)
            {
                CRelationship *relationship = new CRelationship(_tables.value(strList.at(1))->table(),
                                                                _tables.value(strList.at(2))->table());
                CRelationshipItem *relationshipItem = new CRelationshipItem(_tables.value(relationship->startTable()->name()),
                                                                            _tables.value(relationship->endTable()->name()),
                                                                            relationship);
                relationship->importFromTextStream(input);
                _dataModel->addRelationship(relationship);
                _relationships.insert(relationship->name(), relationshipItem);
                _scene->addItem(relationshipItem);
            }
            continue;
        }
        //-----------------------------------------
        if(strList.at(0) == "frow")
            if(strList.size() >= 4)
                this->tables().value(strList.at(3))->table()->importForeginKeysFromTS(strList,this->tables().value(strList.at(1))->table());
        //-----------------------------------------
    }
}

QMap<QString, CRelationshipItem *> CModelView::relationships() const
{
    return _relationships;
}

QMap<QString, CTableItem *> CModelView::tables() const
{
    return _tables;
}

QString CModelView::path() const
{
    return _path;
}

void CModelView::setPath(const QString &path)
{
    _path = path;
}

QString CModelView::name() const
{
    return _name;
}

void CModelView::setName(const QString &name)
{
    _name = name;
}

CDataModel *CModelView::dataModel() const
{
    return _dataModel;
}

void CModelView::deactivateTools()
{
    for (int i = POINTER; i <= AGGREGATE; i++)
    {
        _tools.insert(static_cast<TOOL_TYPE>(i), false);
    }
    if(_tablesToRelate.size())
    {
        foreach (const QString &name, _tablesToRelate) {
            _tables.value(name)->setSelectedForRelation(false);
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
    }
    else
    {
        switch (objectItem->type()) {
        case CTableItem::Type:
            {
                CTableEditor *editor = new CTableEditor((CTable *)objectItem->object(), _dataModel);
                connect(editor, SIGNAL(dataChanged()), this, SLOT(update()));
                objectItem->setEditor((QWidget *)editor);
                editor->show();
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
        addRelationship(startItem->name(), endItem->name());
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
