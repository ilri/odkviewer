#ifndef FRMDATA_H
#define FRMDATA_H

#include <QDialog>
#include "datamodel.h"
#include <QModelIndex>
#include "flickcharm.h"
#include "odkformreader.h"

namespace Ui {
class frmData;
}

class frmData : public QDialog
{
    Q_OBJECT

public:
    explicit frmData(QWidget *parent = 0);
    ~frmData();
    void setSurveyInfo(QString id, QString desc, QString version, QString file, QString tagName);
    void setDataDir(QString dir);
    void loadData();
    void setODKReader(odkFormReader reader);


private slots:


    void on_cmdback_clicked();
    void dataSetClicked(QModelIndex index);

private:
    Ui::frmData *ui;
    QString m_surveyID;
    QString m_surveyDesc;
    QString m_surveyVersion;
    QString m_tagName;
    QString m_surveyFile;
    QString m_dataDir;

    dataModel *m_dataModel;

    odkFormReader m_ODKReader;

    FlickCharm dataCharm;

};

#endif // FRMDATA_H
