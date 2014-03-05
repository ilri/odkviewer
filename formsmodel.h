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

#ifndef FORMSMODEL_H
#define FORMSMODEL_H

#include <QAbstractTableModel>
#include <QList>



struct formItem
{
  QString code;
  QString tagName;
  QString description;
  QString version;
  QString file;
};
typedef formItem TformItem;

// This class is a model that shows the different ODK forms avaiable in a directory.

class formsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit formsModel(QObject *parent = 0);
    ~formsModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void loadForms(QString directory);

    QString getFormID(int index);
    QString getFormDesc(int index);
    QString getFormVersion(int index);
    QString getFormFile(int index);
    QString getFormTag(int index);


private:
    QList <TformItem > forms;
    void loadFile(QString file);


};

#endif // FORMSMODEL_H
