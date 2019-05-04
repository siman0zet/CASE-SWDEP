#ifndef CSCROLLAREA_H
#define CSCROLLAREA_H

#include <QWidget>
#include <QScrollArea>
#include <QResizeEvent>
#include <QKeyEvent>

class CScrollArea : public QScrollArea
{
public:
    CScrollArea();
    ~CScrollArea();

protected:

    void resizeEvent(QResizeEvent *re);
    void keyPressEvent(QKeyEvent *ke);
    void keyReleaseEvent(QKeyEvent *ke);
};

#endif // CSCROLLAREA_H
