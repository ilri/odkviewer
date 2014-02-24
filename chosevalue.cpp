#include "chosevalue.h"
#include "ui_chosevalue.h"
#include <QDebug>
#include <QDebug>


choseValue::choseValue(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::choseValue)
{
    ui->setupUi(this);

    m_lkpModel = new lkpTableModel;

#ifdef Q_OS_ANDROID
    //QScroller::grabGesture(ui->tableView, QScroller::LeftMouseButtonGesture);
    valueCharm.activateOn(ui->tableView);
#endif

}

choseValue::~choseValue()
{
    delete ui;
}

void choseValue::keyReleaseEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key_Back)
    {
        event->accept();
        close();
    }
}

void choseValue::on_cmdcancel_clicked()
{
    close();
}

void choseValue::loadValues(QList<TlkpValue > values)
{
    m_lkpModel->loadData(values);
    ui->tableView->setModel(m_lkpModel);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    int currIndex;
    currIndex = m_lkpModel->getItemIndex(return_value);
    QModelIndex itemIndex = m_lkpModel->index(currIndex,0);
    if (itemIndex.isValid())
        ui->tableView->setCurrentIndex(itemIndex);
}

void choseValue::on_cmdok_clicked()
{
    QModelIndex index;
    index = ui->tableView->currentIndex();
    if (index.isValid())
    {
        return_value = m_lkpModel->getValue(index.row());
        close();
    }
}
