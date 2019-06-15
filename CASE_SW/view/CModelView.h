#ifndef CMODELVIEW_H
#define CMODELVIEW_H

#include <QGraphicsView>
#include <QMap>

class MainWindow;
class CDataModel;
class CTableItem;
class CRelationshipItem;
class CObjectItem;
class QMainWindow;

class CModelView : public QGraphicsView
{
    Q_OBJECT

public:
    CModelView(const QString &name, const QString &path, QWidget *parent = nullptr);
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

    bool saveToFile(const QString &path) const;

    void activateTool(const cursorToolType &type);
    void showResizeDialog();
    void showChangeTableDialog(const QString &relationshipName, bool end) const;
    void showObjectEditor(CObjectItem *objectItem);
    void flipTables(const QString &relationshipName);

    CTableItem *tableItem(const QString &name) const;
    CDataModel *dataModel() const;

    QString name() const;
    void setName(const QString &name);
    QString path() const;
    void setPath(const QString &path);
    QMainWindow *pModelWindow() const;
    void setPModelWindow(QMainWindow *pModelWindow);

    QMap<QString, CTableItem *> tables() const;
    QMap<QString, CRelationshipItem *> relationships() const;

public slots:
    void changeSize(int w, int h);
    void changeTable(const QString &relationshipName, const QString &tableName, bool start);

protected:
    // QWidget interface
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QString _name;
    QString _path;
    QMainWindow *_pModelWindow;

    MainWindow *_parent;
    QGraphicsScene *_scene;
    CDataModel *_dataModel;

    int   _width;
    int   _height;    

    QMap<cursorToolType, bool> _tools;
    QMap<QString, CTableItem *> _tables;
    QMap<QString, CRelationshipItem *> _relationships;
    QList<QString> _tablesToRelate;

    QString convertToText() const;
    void addTable(const QPoint &pos);
    void addRelationship(const QString &startName, const QString &endName);
    void removeItem(const QPoint &pos);
    void removeItems(QList<QGraphicsItem *> items);
    void deactivateTools();
    void showContextMenu(const QPoint &pos, bool isEnabled,  bool colorOption = false, bool relationOption = false);
    void showTableContextMenu(const QPoint &pos, QString tableName);
    void showRelationshipContextMenu(const QPoint &pos);
    void returnToPointer();

private slots:
    void addRelationship();
    void removeItems();
    void showObjectEditor();
    void changeTableName(const QString &oldName, const QString &newName);

};

#endif // CMODELVIEW_H
