#ifndef DATATABLEVIEW_H
#define DATATABLEVIEW_H

#include <QTableView>
#include <QGestureEvent>
#include <QEvent>

class dataTableView : public QTableView
{
    Q_OBJECT
public:
    explicit dataTableView(QWidget *parent = 0);

protected:
    bool event(QEvent * event);
    bool gestureEvent(QGestureEvent *event);

};

#endif // DATATABLEVIEW_H
