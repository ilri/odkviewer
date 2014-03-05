/*
    ODK Viewer - A tool to visualise and edit survey data collected in ODK
    Copyright (C) 2014  International Livestock Research Institute
    Author: Carlos Quiros (Research Methods Group)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

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
