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

#include "frmdataeditor.h"
#include "ui_frmdataeditor.h"
#include <QScroller>
#include <QDebug>
#include "odkformreader.h"
#include "chosevalue.h"
#include <QLineEdit>



frmDataEditor::frmDataEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmDataEditor)
{
    ui->setupUi(this);

    //ODKreader = new odkFormReader(this);
    m_editorModel = new editorModel;




    //ui->tableView->setModel(m_editorModel);
    ui->tableView->setModel(m_editorModel);


    ui->cmdsave->setEnabled(false);
    ui->cmdcancel->setEnabled(false);

    connect(m_editorModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(dataChanged(QModelIndex,QModelIndex)));


}

frmDataEditor::~frmDataEditor()
{
    delete ui;
}


void frmDataEditor::setODKReader(odkFormReader reader)
{
    ODKreader = reader;
    ODKreader.createItemsTree(); //We create the items tree here because is a list of pointer and thet get free when the editor closes.
    ui->treeWidget->insertTopLevelItems(0,ODKreader.getTreeItems());
    ui->treeWidget->expandAll();
    ui->treeWidget->resizeColumnToContents(0);
#ifdef Q_OS_ANDROID
    editcharm.activateOn(ui->tableView);
    treeCharm.activateOn(ui->treeWidget);
#endif
}


void frmDataEditor::addData(QString dataFile, QString dataDesc)
{

    m_dataFile = dataFile;
    m_dataDesc = dataDesc;
    m_editorModel->addData(dataFile,dataDesc);

}

void frmDataEditor::setSurveyInfo(QString surveyID, QString surveyDesc, QString surveyTag, QString surveyVersion, QString surveyFile)
{
    m_surveyID = surveyID;
    m_surveyDesc = surveyDesc;
    m_surveyTag = surveyTag;
    m_surveyVersion = surveyVersion;
    m_surveyFile = surveyFile;
    ui->lblsurvey->setText("Survey: " + m_surveyDesc);
    ui->lblversion->setText("Version: " + m_surveyVersion);
}

void frmDataEditor::dataChanged(QModelIndex , QModelIndex )
{
    ui->treeWidget->setEnabled(false);
    ui->cmdsave->setEnabled(true);
    ui->cmdcancel->setEnabled(true);
}

void frmDataEditor::on_cmdback_clicked()
{
    close();
}

void frmDataEditor::on_cmdhide_clicked()
{
    if (ui->treeWidget->isVisible())
        ui->treeWidget->setVisible(false);
    else
        ui->treeWidget->setVisible(true);
}



void frmDataEditor::on_treeWidget_itemClicked(QTreeWidgetItem *item, int /*column*/)
{
    TtableDef table;
    table = ODKreader.getTable(item->data(0,Qt::UserRole).toString());
    m_editorModel->loadData(table);

    ui->tableView->resizeColumnsToContents();


}

void frmDataEditor::on_cmdsave_clicked()
{
    m_editorModel->saveData();
    ui->treeWidget->setEnabled(true);
    ui->cmdsave->setEnabled(false);
    ui->cmdcancel->setEnabled(false);
}

void frmDataEditor::on_cmdcancel_clicked()
{
    QTreeWidgetItem *item;
    item = ui->treeWidget->currentItem();
    if (item)
    {
        TtableDef table;
        //table = ODKreader.getTable(item->data(0,Qt::UserRole).toString());
        table = ODKreader.getTable(item->data(0,Qt::UserRole).toString());
        m_editorModel->loadData(table);

        ui->tableView->resizeColumnsToContents();
        //tableView->resizeColumnsToContents();

        ui->treeWidget->setEnabled(true);
        ui->cmdsave->setEnabled(false);
        ui->cmdcancel->setEnabled(false);
    }

}

void frmDataEditor::on_tableView_doubleClicked(const QModelIndex &index)
{

        QString lkpTable;
        lkpTable = m_editorModel->getColumnLkp(index.column());
        if (lkpTable != "NULL")
        {
            choseValue valueDialog;
            valueDialog.setWindowState(Qt::WindowMaximized);
            valueDialog.return_value = index.data().toString();
            //valueDialog.loadValues(ODKreader.getLookUpValues(lkpTable));
            valueDialog.loadValues(ODKreader.getLookUpValues(lkpTable));
            valueDialog.exec();
            m_editorModel->setData(index,valueDialog.return_value);
        }

}



void frmDataEditor::on_cmdedit_clicked()
{
    QModelIndex index;
    index = ui->tableView->currentIndex();
    //index = tableView->currentIndex();
    if (index.isValid())
    {

            QString lkpTable;
            lkpTable = m_editorModel->getColumnLkp(index.column());
            if (lkpTable != "NULL")
            {
                choseValue valueDialog;
                valueDialog.setWindowState(Qt::WindowMaximized);
                valueDialog.return_value = index.data().toString();
                //valueDialog.loadValues(ODKreader.getLookUpValues(lkpTable));
                valueDialog.loadValues(ODKreader.getLookUpValues(lkpTable));
                valueDialog.exec();
                m_editorModel->setData(index,valueDialog.return_value);
            }
            else
            {
                ui->tableView->edit(index);


                QLineEdit * edit =  qobject_cast<QLineEdit *>(ui->tableView->indexWidget(index));
                if (edit)
                {
                    edit->setSelection(0,0);
                    edit->setCursorPosition(edit->text().length());
                    QGuiApplication::inputMethod()->show();
                }
            }

    }
}

void frmDataEditor::on_cmdhidesurvey_clicked()
{
   if (ui->tableView->verticalHeader()->isVisible())
       ui->tableView->verticalHeader()->hide();
   else
       ui->tableView->verticalHeader()->show();
}
