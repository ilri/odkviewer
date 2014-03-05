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
    //void dataSetClicked(QModelIndex index);

    void on_cmdload_clicked();

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
    int maxDatasets;

};

#endif // FRMDATA_H
