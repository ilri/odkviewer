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

#include "frmdata.h"
#include "ui_frmdata.h"
#include <QDebug>
#include "frmdataeditor.h"
#include <QModelIndexList>


frmData::frmData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmData)
{
    ui->setupUi(this);

    m_dataModel = new dataModel;

#ifdef Q_OS_ANDROID
    //QScroller::grabGesture(ui->tableView, QScroller::LeftMouseButtonGesture);
    dataCharm.activateOn(ui->tableView);
    ui->lblmax->setText("Select a maximum of 3 datasets");
    maxDatasets = 3;
#else
    maxDatasets = 6;
    ui->lblmax->setText("Select a maximum of 6 datasets");
#endif

}

frmData::~frmData()
{
    delete ui;
}

void frmData::setODKReader(odkFormReader reader)
{
    m_ODKReader = reader;
}


void frmData::setSurveyInfo(QString id, QString desc, QString version, QString file, QString tagName)
{
    m_surveyID = id;
    m_surveyDesc = desc;
    m_surveyVersion = version;
    ui->lblsurvey->setText("Survey: " + desc);
    ui->lblversion->setText("Version: " + version);
    m_surveyFile = file;
    m_tagName = tagName;
}

void frmData::setDataDir(QString dir)
{
    m_dataDir = dir;
}

void frmData::loadData()
{
    m_dataModel->loadData(m_dataDir,m_surveyID,m_tagName);
    ui->tableView->setModel(m_dataModel);

    ui->tableView->resizeColumnToContents(0);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::MultiSelection);

    //connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(dataSetClicked(QModelIndex)));

}

void frmData::on_cmdback_clicked()
{
    close();
}

/*void frmData::dataSetClicked(QModelIndex index)
{
    if (index.isValid())
    {
        QString dataDesc;
        QString dataFile;
        dataDesc = m_dataModel->getDataSetName(index.row());
        dataFile = m_dataModel->getDataSetPath(index.row());




    }
}*/

void frmData::on_cmdload_clicked()
{
    QModelIndexList selected;

    if ( ui->tableView->selectionModel()->selection().count() > maxDatasets)
        return;

    selected = ui->tableView->selectionModel()->selection().indexes();

    if (selected.count() > 0)
    {
        QString dataDesc;
        QString dataFile;
         QModelIndex index;

        frmDataEditor editorDialog;
        editorDialog.setODKReader(m_ODKReader);
        editorDialog.setSurveyInfo(m_surveyID,m_surveyDesc,m_tagName,m_surveyVersion,m_surveyFile);
        editorDialog.setWindowState(Qt::WindowMaximized);
        for (int pos = 0; pos <= selected.count()-1;pos++)
        {
            index = selected[pos];
            dataDesc = m_dataModel->getDataSetName(index.row());
            dataFile = m_dataModel->getDataSetPath(index.row());
            editorDialog.addData(dataFile,dataDesc);
        }
        editorDialog.exec();
    }
}
