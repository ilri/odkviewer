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

#include "formsmodel.h"
#include <QDir>
#include <QFile>
#include <QDomDocument>
#include <QDebug>




formsModel::formsModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

formsModel::~formsModel()
{
    //qDebug() << "Destroy: formsModel";
}


void formsModel::loadFile(QString file)
{
    //qDebug() << file;

    QDomDocument doc("ODKDocument");
    QFile xmlfile(file);
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

    QDomElement mainInstance;

    QString surveyTitle;

    list = doc.elementsByTagName("h:title");
    if (list.count() > 0)
    {
        surveyTitle = list.item(0).firstChild().nodeValue();
    }


    list = doc.elementsByTagName("instance");
    if (list.count() > 0)
    {
        int pos;
        for (pos = 0; pos <= list.count()-1;pos++)
        {
            item = list.item(pos).toElement();
            if (item.attribute("id","NONE") == "NONE")
            {
                mainInstance = item;
                break;
            }
        }
    }
    item = mainInstance.firstChild().toElement();


    QString surveyID;
    surveyID = item.attribute("id","Unknown");

    QString surveyVersion;
    surveyVersion = item.attribute("version","Unknown");

    TformItem fitem;
    fitem.code = surveyID;
    fitem.tagName = item.tagName();
    fitem.description = surveyTitle;
    fitem.version = surveyVersion;
    fitem.file = file;

    forms.append(fitem);

    //qDebug() << surveyID;
    //qDebug() << surveyTitle;
    //qDebug() << surveyVersion;

}

void formsModel::loadForms(QString directory)
{
    //qDebug() << directory;

    QDir dir;
    dir.setPath(directory);
    if (dir.exists())
    {
        foreach (QString fileName, dir.entryList(QDir::Files))
        {
            if (QFileInfo(fileName).completeSuffix() == "xml")
            {
                loadFile(directory + "/" + fileName);
            }
        }
    }
}

QString formsModel::getFormID(int index)
{
    return forms[index].code;
}

QString formsModel::getFormDesc(int index)
{
    return forms[index].description;
}

QString formsModel::getFormVersion(int index)
{
    return forms[index].version;
}
QString formsModel::getFormFile(int index)
{
    return forms[index].file;
}

QString formsModel::getFormTag(int index)
{
    return forms[index].tagName;
}


int formsModel::rowCount(const QModelIndex &) const
{
    return forms.count();
}


int formsModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant formsModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if (index.column() == 0)
            return forms[index.row()].code;
        if (index.column() == 1)
            return forms[index.row()].description;
        if (index.column() == 2)
            return forms[index.row()].version;
    }
    return QVariant();
}

bool formsModel::setData(const QModelIndex &/*index*/,const QVariant &/*value*/,int /*role*/)
{
    return false;
}

Qt::ItemFlags formsModel::flags(const QModelIndex &) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant formsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 0)
                return "ID";
            if (section == 1)
                return "Description";
            if (section == 2)
                return "Version";
        }
    }
    return QVariant();
}
