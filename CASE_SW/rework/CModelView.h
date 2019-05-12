#ifndef CMODELVIEW_H
#define CMODELVIEW_H

#include <QGraphicsView>
#include <QMap>

class CDataModel;

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
    void addEntity();
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
    QGraphicsScene *_scene;
    CDataModel *_dataModel;
    int   _width;
    int   _height;

    QMap<cursorToolType, bool> _tools;
    QMap<int, QGraphicsItem *> _entities;

    void deactivateTools();
    int countSelectedEntities();
    void showContextMenu(const QPoint &pos, bool isEnabled);
    void showTableContextMenu(const QPoint &pos, QString tableName);
    void showTableRelationshipContextMenu(const QPoint &pos);

};

#endif // CMODELVIEW_H
