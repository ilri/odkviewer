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

#include "datamodel.h"
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QDomDocument>

dataModel::dataModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

void dataModel::loadFile(QString fileName, QString SurveyID, QString fileDesc, QString surveyTag)
{

    QDomDocument doc("ODKDocument");
    QFile xmlfile(fileName);
    if (!xmlfile.open(QIODevice::ReadOnly))
        return ;
    if (!doc.setContent(&xmlfile))
    {
        xmlfile.close();
        return ;
    }
    xmlfile.close();

    QDomNodeList list;
    QDomElement item;

    list = doc.elementsByTagName(surveyTag);
    if (list.count() > 0)
    {
        item = list.item(0).toElement();
        if (item.attribute("id","{NULL}") == SurveyID)
        {
            TdataItem dItem;
            dItem.name = fileDesc.replace(".xml","");
            dItem.path = fileName;

            dataFiles.append(dItem);
        }
    }
}

void dataModel::loadData(QString dataDir, QString surveyID, QString surveyTag)
{
    QDir dir;
    QDir dir2;
    dir.setPath(dataDir);

    if (dir.exists())
    {
        foreach (QString dirName, dir.entryList(QDir::Dirs))
        {
            dir2.setPath(dataDir + "/" + dirName);
            if (dir2.exists())
            {
                foreach (QString fileName, dir2.entryList(QDir::Files))
                {
                    if (QFileInfo(fileName).completeSuffix() == "xml")
                    {
                        loadFile(dataDir + "/" + dirName + "/" + fileName,surveyID,fileName,surveyTag);
                    }
                }
            }
        }
    }
}


QString dataModel::getDataSetPath(int index)
{
    return dataFiles[index].path;
}

QString dataModel::getDataSetName(int index)
{
    return dataFiles[index].name;
}

//-------------------------------------------------Model control functions----------------------------

int dataModel::rowCount(const QModelIndex &) const
{
    return dataFiles.count();
}

int dataModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant dataModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if (index.column() == 0)
            return dataFiles[index.row()].name;
    }
    return QVariant();
}

bool dataModel::setData(const QModelIndex &/*index*/,const QVariant &/*value*/,int /*role*/)
{
    return false;
}

Qt::ItemFlags dataModel::flags(const QModelIndex &) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant dataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 0)
                return "Dataset";
        }
    }
    return QVariant();
}
