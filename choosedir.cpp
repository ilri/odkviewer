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

#include "choosedir.h"
#include "ui_choosedir.h"
#include <QFileSystemModel>
#include <QDebug>
#include <QScroller>

chooseDir::chooseDir(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chooseDir)
{
    ui->setupUi(this);

    QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath("~/");
    model->setFilter(QDir::Dirs|QDir::Drives|QDir::NoDotAndDotDot|QDir::AllDirs);

    ui->treeView->setModel(model);

    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);

    ui->treeView->resizeColumnToContents(0);

    connect(ui->treeView,SIGNAL(expanded(QModelIndex)),this,SLOT(itemExpanded(QModelIndex)));
    connect(ui->treeView,SIGNAL(collapsed(QModelIndex)),this,SLOT(itemExpanded(QModelIndex)));

    seldir = "~/";
#ifdef Q_OS_ANDROID
    QScroller::grabGesture(ui->treeView, QScroller::LeftMouseButtonGesture);
#endif

}

chooseDir::~chooseDir()
{
    delete ui;
}

void chooseDir::on_cmdCancel_clicked()
{
    close();
}

void chooseDir::itemExpanded(QModelIndex)
{
    ui->treeView->resizeColumnToContents(0);
}

void chooseDir::on_cmdok_clicked()
{
    QFileSystemModel *cmodel;
    cmodel = qobject_cast<QFileSystemModel *>(ui->treeView->model());

    if (ui->treeView->currentIndex().isValid())
    {
        if (cmodel->isDir(ui->treeView->currentIndex()))
        {
            seldir = cmodel->filePath(ui->treeView->currentIndex());
        }
    }
    close();
}
