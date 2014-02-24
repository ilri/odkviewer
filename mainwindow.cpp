#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "frmsettings.h"
#include <QSettings>
#include <QDir>
#include <QDebug>
#include <QScroller>
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

        reader.processXML(surveyFile,"mainModule");



        surveyTag = m_formModel->getFormTag(index.row());
        dataDialog.setODKReader(reader);
        dataDialog.setSurveyInfo(surveyID,surveyDesc,surveyVerion,surveyFile,surveyTag);
        dataDialog.setDataDir(dataDir);
        dataDialog.loadData();
        dataDialog.setWindowState(Qt::WindowMaximized);
        dataDialog.exec();



    }
}
