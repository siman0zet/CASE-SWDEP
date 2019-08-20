#ifndef PMODELVIEW_H
#define PMODELVIEW_H

#include <QGraphicsView>
#include <QMap>

class CModelView;
class CDataModel;
class CTableItem;
class CRelationshipItem;

class PModelView : public QGraphicsView
{
    Q_OBJECT

public:
    PModelView(CModelView *cModelView, QWidget *parent = nullptr);
    ~PModelView();

    CDataModel *dataModel() const;
    QString name() const;
    void showResizeDialog();

public slots:
    void changeSize(int w, int h);

private:
    QGraphicsScene *_scene;

    int _width;
    int _height;

    QMap<QString, CTableItem *> _tables;
    QMap<QString, CRelationshipItem *> _relationships;

    CDataModel *_dataModel;

    QString _name;

    void populateScene(CDataModel *pModel, CModelView *cModelView);
    QPointF calculateConduitPoint(CTableItem *startItem, CTableItem *endItem);
};

#endif // PMODELVIEW_H
