#ifndef CMODELVIEW_H
#define CMODELVIEW_H

#include <QGraphicsView>
#include <QMap>

class MainWindow;
class CDataModel;
class CEntityItem;

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

public slots:
    void changeSize(int w, int h);

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
    QMap<int, CEntityItem *> _entities;
    QMap<int, QGraphicsItem *> _relationships;
    QList<int> _entitiesToRelate;

    void addEntity(const QPoint &pos);
    void addRelationship(int startId, int endId);
    void deactivateTools();
    int countSelectedEntities();
    void showContextMenu(const QPoint &pos, bool isEnabled, bool isTwoEntities = false);
    void showTableContextMenu(const QPoint &pos, QString tableName);
    void returnToPointer();

private slots:
    void addRelationship();

};

#endif // CMODELVIEW_H
