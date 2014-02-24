#ifndef CHOSEVALUE_H
#define CHOSEVALUE_H

#include <QDialog>
#include <genstruct.h>
#include "lkptablemodel.h"
#include <QKeyEvent>
#include "flickcharm.h"

namespace Ui {
class choseValue;
}

class choseValue : public QDialog
{
    Q_OBJECT

public:
    explicit choseValue(QWidget *parent = 0);
    ~choseValue();
    void loadValues(QList<TlkpValue > values);
    QString return_value;

private slots:
    void on_cmdcancel_clicked();

    void on_cmdok_clicked();

protected:
    void keyReleaseEvent(QKeyEvent * event);

private:
    Ui::choseValue *ui;

    lkpTableModel * m_lkpModel;

    FlickCharm valueCharm;
};

#endif // CHOSEVALUE_H
