#include "datatableview.h"
#include <QGestureEvent>

dataTableView::dataTableView(QWidget *parent) :
    QTableView(parent)
{
}

bool dataTableView::event(QEvent * event)
{
    if (event->type() == QEvent::Gesture)
            return gestureEvent(static_cast<QGestureEvent*>(event));
        return QWidget::event(event);
}

bool dataTableView::gestureEvent(QGestureEvent *event)
{
    if (event->gesture(Qt::TapAndHoldGesture))
    {

    }
    return true;
}
