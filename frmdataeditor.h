#ifndef FRMDATAEDITOR_H
#define FRMDATAEDITOR_H

#include <QDialog>
#include "odkformreader.h"
#include "editormodel.h"
#include <QList>
#include <QPointer>
#include "odkformreader.h"

#include "flickcharm.h"


namespace Ui {
class frmDataEditor;
}

typedef QPointer<QAbstractItemDelegate> Tdelegate;

class frmDataEditor : public QDialog
{
    Q_OBJECT

public:
    explicit frmDataEditor(QWidget *parent = 0);
    ~frmDataEditor();
    void setData(QString surveyID, QString surveyDesc, QString surveyTag, QString surveyVersion, QString surveyFile, QString dataDesc, QString dataFile);
    void setODKReader(odkFormReader reader);


private slots:
    void on_cmdback_clicked();

    void on_cmdhide_clicked();



    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_cmdsave_clicked();

    void on_cmdcancel_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_cmdedit_clicked();

    void dataChanged(QModelIndex from, QModelIndex to);


private:
    Ui::frmDataEditor *ui;
    QString m_surveyID;
    QString m_surveyTag;
    QString m_surveyDesc;
    QString m_surveyVersion;
    QString m_surveyFile;
    QString m_dataFile;
    QString m_dataDesc;


    odkFormReader ODKreader;


    FlickCharm editcharm;
    FlickCharm treeCharm;

    editorModel *m_editorModel;
    QList< Tdelegate> delegates;
};

#endif // FRMDATAEDITOR_H
