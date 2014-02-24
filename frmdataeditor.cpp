#include "frmdataeditor.h"
#include "ui_frmdataeditor.h"
#include <QScroller>
#include <QDebug>
#include "odkformreader.h"
#include "chosevalue.h"
#include <QLineEdit>
#include <QMouseEvent>





frmDataEditor::frmDataEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::frmDataEditor)
{
    ui->setupUi(this);

    //ODKreader = new odkFormReader(this);

    m_editorModel = new editorModel;

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
}


void frmDataEditor::setData(QString surveyID, QString surveyDesc, QString surveyTag, QString surveyVersion, QString surveyFile, QString dataDesc, QString dataFile)
{
    m_surveyID = surveyID;
    m_surveyDesc = surveyDesc;
    m_surveyTag = surveyTag;
    m_surveyVersion = surveyVersion;
    m_surveyFile = surveyFile;
    m_dataDesc = dataDesc;
    m_dataFile = dataFile;
    ui->lblsurvey->setText("Survey: " + m_surveyDesc);
    ui->lblversion->setText("Version: " + m_surveyVersion);
    ui->lbldata->setText("Data file: " + m_dataDesc);

    //ODKreader.processXML(m_surveyFile,"mainForm");


    //ui->treeWidget->insertTopLevelItems(0,ODKreader.getTreeItems());
    ui->treeWidget->insertTopLevelItems(0,ODKreader.getTreeItems());
    ui->treeWidget->expandAll();
    ui->treeWidget->resizeColumnToContents(0);

#ifdef Q_OS_ANDROID
    editcharm.activateOn(ui->tableView);
    treeCharm.activateOn(ui->treeWidget);
#endif

    //ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);

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
    m_editorModel->loadData(table,m_dataFile);

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
        m_editorModel->loadData(table,m_dataFile);

        ui->tableView->resizeColumnsToContents();

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
    if (ui->tableView->currentIndex().isValid())
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
            /*QLineEdit * edit =  qobject_cast<QLineEdit *>(ui->tableView->indexWidget(index));
            if (edit)
            {
                edit->setSelection(0,0);
                edit->setCursorPosition(edit->text().length());
                QMouseEvent* event = new QMouseEvent( QEvent::MouseButtonPress, QPoint( 1, 1 ),Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
                QApplication::postEvent( edit, event );
            }*/
        }
    }
}
