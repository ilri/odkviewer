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

#ifndef EDITORMODEL_H
#define EDITORMODEL_H

#include <QAbstractTableModel>
#include "genstruct.h"

// This class is a model that controls the editing and presentation of the ODK data.

class editorModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit editorModel(QObject *parent = 0);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void loadData(TtableDef table);
    void addData(QString dataFile, QString dataDesc);
    void saveData();

    QString getColumnLkp(int column);

private:
    TtableDef activeTable;
    //QString m_dataFile;
    //QList<TdataDef > dataList;
    //QDomDocument doc;
    QList<TdataFileDef> fileList;
    QString getDataDesc(int index) const;
    QString getDataValue(int index, int column) const;
    bool isDataModified(int index, int column) const;
    bool isDataIgnored(int index, int column) const;
    void setData2(int index, int column, QString value);

};

#endif // EDITORMODEL_H
