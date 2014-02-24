#include "frmdata.h"
#include "ui_frmdata.h"
#include <QDebug>
#include "frmdataeditor.h"
#include <QScroller>

frmData::frmData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmData)
{
    ui->setupUi(this);

    m_dataModel = new dataModel;

#ifdef Q_OS_ANDROID
    //QScroller::grabGesture(ui->tableView, QScroller::LeftMouseButtonGesture);
    dataCharm.activateOn(ui->tableView);
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

    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(dataSetClicked(QModelIndex)));

}

void frmData::on_cmdback_clicked()
{
    close();
}

void frmData::dataSetClicked(QModelIndex index)
{
    if (index.isValid())
    {
        QString dataDesc;
        QString dataFile;
        dataDesc = m_dataModel->getDataSetName(index.row());
        dataFile = m_dataModel->getDataSetPath(index.row());
        frmDataEditor editorDialog;
        editorDialog.setODKReader(m_ODKReader);
        editorDialog.setData(m_surveyID,m_surveyDesc,m_tagName,m_surveyVersion,m_surveyFile,dataDesc,dataFile);
        editorDialog.setWindowState(Qt::WindowMaximized);
        editorDialog.exec();
    }
}
