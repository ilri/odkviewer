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


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "frmsettings.h"
#include <QSettings>
#include <QDir>
#include <QDebug>
#include <QScroller>
#include "about.h"
#include "frmdata.h"

mainWindow::mainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainWindow)
{
    ui->setupUi(this);
    loadSettings();

    m_formModel = new formsModel;

    m_formModel->loadForms(formsDir);
    ui->tableView->setModel(m_formModel);

    ui->tableView->resizeColumnToContents(0);
    ui->tableView->resizeColumnToContents(1);
    ui->tableView->resizeColumnToContents(2);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(formClicked(QModelIndex)));

    movie = new QMovie(":/loading.gif");
    ui->lblmovie->setMovie(movie);

    ui->stackedWidget->setCurrentIndex(0);
    connect(&odkReader,SIGNAL(finished()),this,SLOT(readerFinished()));




#ifdef Q_OS_ANDROID
    QScroller::grabGesture(ui->tableView, QScroller::LeftMouseButtonGesture);
#endif

}


mainWindow::~mainWindow()
{
    delete ui;
}

void mainWindow::on_cmdclose_clicked()
{
    close();
}

void mainWindow::loadSettings()
{
    QSettings settings("ILRI","odktocsv");
    formsDir = settings.value("formdir","Not set").toString();
    dataDir = settings.value("datadir","Not set").toString();

#ifdef Q_OS_ANDROID
    if (formsDir == "Not set")
    {
        formsDir = "/sdcard/odk/forms";
        settings.setValue("formdir",formsDir);
    }
    if (dataDir == "Not set")
    {
        dataDir = "/sdcard/odk/instances";
        settings.setValue("datadir",dataDir);
    }
#endif
}


void mainWindow::on_cmdsettings_clicked()
{
    frmSettings settings;
    settings.setWindowState(Qt::WindowMaximized);
    settings.exec();
    loadSettings();
}

void mainWindow::formClicked(QModelIndex index)
{
    if (index.isValid())
    {

        QString surveyFile;

        surveyFile = m_formModel->getFormFile(index.row());
        odkFormReader reader;

        ui->stackedWidget->setCurrentIndex(1);
        connect(movie,SIGNAL(finished()),this,SLOT(reStart()));

        movie->start();

        ui->cmdabout->setEnabled(false);
        ui->cmdsettings->setEnabled(false);
        ui->cmdclose->setEnabled(false);

        odkReader.setSurveyData(surveyFile,"mainModule");
        odkReader.setReader(reader);
        odkReader.start();

    }
}

void mainWindow::reStart()
{
    movie->start();
}

void mainWindow::readerFinished()
{
    QModelIndex index;

    index = ui->tableView->currentIndex();
    if (!index.isValid())
    {
        movie->stop();
        ui->stackedWidget->setCurrentIndex(0);
        return;
    }

    QString surveyID;
    QString surveyDesc;
    QString surveyVerion;
    QString surveyFile;
    QString surveyTag;
    frmData dataDialog;


    odkFormReader reader;


    surveyDesc = m_formModel->getFormDesc(index.row());
    surveyID = m_formModel->getFormID(index.row());
    surveyVerion = m_formModel->getFormVersion(index.row());
    surveyFile = m_formModel->getFormFile(index.row());

    reader = odkReader.getReader();
    movie->stop();

    ui->cmdabout->setEnabled(true);
    ui->cmdsettings->setEnabled(true);
    ui->cmdclose->setEnabled(true);

    ui->stackedWidget->setCurrentIndex(0);

    surveyTag = m_formModel->getFormTag(index.row());
    dataDialog.setODKReader(reader);
    dataDialog.setSurveyInfo(surveyID,surveyDesc,surveyVerion,surveyFile,surveyTag);
    dataDialog.setDataDir(dataDir);
    dataDialog.loadData();
    dataDialog.setWindowState(Qt::WindowMaximized);
    dataDialog.exec();
}

void mainWindow::on_cmdabout_clicked()
{
    about aboutWindow;
    aboutWindow.setWindowState(Qt::WindowMaximized);
    aboutWindow.exec();
}
