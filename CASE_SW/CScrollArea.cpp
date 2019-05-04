#include "CScrollArea.h"

#include "CViewModel.h"

CScrollArea::CScrollArea(): QScrollArea()
{

}

CScrollArea::~CScrollArea()
{

}

void CScrollArea::resizeEvent(QResizeEvent *re)
{
    if (this->widget())
        this->widget()->resize(re->size());

    QScrollArea::resizeEvent(re);
}

void CScrollArea::keyPressEvent(QKeyEvent *ke)
{
    CViewModel *view = static_cast<CViewModel*> (this->widget());

    if (view && ke->key() == Qt::Key_Control)
        view->setMultiplySelection(true);
}

void CScrollArea::keyReleaseEvent(QKeyEvent *ke)
{
    CViewModel *view = static_cast<CViewModel*> (this->widget());

    if (view && ke->key() == Qt::Key_Control)
        view->setMultiplySelection(false);
}

