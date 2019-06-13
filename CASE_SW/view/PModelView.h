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
public:
    PModelView(CModelView *cModelView, QWidget *parent = nullptr);
    ~PModelView();

public slots:
    void changeSize(int w, int h);

private:
    QGraphicsScene *_scene;

    int _width;
    int _height;

    QMap<QString, CTableItem *> _tables;
    QMap<QString, CRelationshipItem *> _relationships;

    void populateScene(CDataModel *pModel, CModelView *cModelView);
    QPointF calculateConduitPoint(CTableItem *startItem, CTableItem *endItem);
};

#endif // PMODELVIEW_H
