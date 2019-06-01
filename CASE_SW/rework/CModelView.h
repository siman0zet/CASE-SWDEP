#ifndef CMODELVIEW_H
#define CMODELVIEW_H

#include <QGraphicsView>
#include <QMap>

class MainWindow;
class CDataModel;
class CTableItem;
class CRelationshipItem;
class CObjectItem;

class CModelView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit CModelView(QWidget *parent = nullptr);
    ~CModelView();

    enum cursorToolType {
        POINTER = 0,
        CREATE,
        DELETE,
        ONE_ONE,
        ONE_MANY,
        MANY_MANY,
        AGGREGATE
    };

    void activateTool(cursorToolType type);
    void showResizeDialog();
    void flipTables(int id);

public slots:
    void changeSize(int w, int h);
    void removeRelationship(int id);

protected:
    // QWidget interface
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    MainWindow *_parent;
    QGraphicsScene *_scene;
    CDataModel *_dataModel;

    int   _width;
    int   _height;    

    QMap<cursorToolType, bool> _tools;
    QMap<int, CTableItem *> _tables;
    QMap<int, CRelationshipItem *> _relationships;
    QList<int> _tablesToRelate;

    void addTable(const QPoint &pos);
    void addRelationship(int startId, int endId);
    void removeItem(const QPoint &pos);
    void removeItems(QList<QGraphicsItem *> items);
    void deactivateTools();
    void showContextMenu(const QPoint &pos, bool isEnabled,  bool colorOption = false, bool relationOption = false);
    void showTableContextMenu(const QPoint &pos, QString tableName);
    void showRelationshipContextMenu(const QPoint &pos);
    void returnToPointer();
    void showObjectEditor(CObjectItem *objectItem);

private slots:
    void addRelationship();
    void removeItems();
    void showObjectEditor();

};

#endif // CMODELVIEW_H
