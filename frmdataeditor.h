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



class frmDataEditor : public QDialog
{
    Q_OBJECT

public:
    explicit frmDataEditor(QWidget *parent = 0);
    ~frmDataEditor();
    void addData(QString dataFile, QString dataDesc);
    void setSurveyInfo(QString surveyID, QString surveyDesc, QString surveyTag, QString surveyVersion, QString surveyFile);
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


    void on_cmdhidesurvey_clicked();

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

};

#endif // FRMDATAEDITOR_H
