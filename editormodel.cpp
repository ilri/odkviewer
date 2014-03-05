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

#include "editormodel.h"
#include <QDomDocument>
#include <QFile>
#include <QBrush>
#include <QTextStream>
#include <QDebug>

editorModel::editorModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

void editorModel::addData(QString dataFile, QString dataDesc)
{
    TdataFileDef dfile;
    dfile.dataFile = dataFile;
    dfile.dataDesc = dataDesc;
    dfile.doc = QDomDocument("ODKDocument");
    dfile.opened = true;
    dfile.writeFile = false;
    QFile xmlfile(dataFile);
    if (!xmlfile.open(QIODevice::ReadOnly))
        dfile.opened = false;
    if (!dfile.doc.setContent(&xmlfile))
    {
        xmlfile.close();
        dfile.opened = false;
    }
    xmlfile.close();
    fileList.append(dfile);

}

void editorModel::loadData(TtableDef table)
{
    activeTable = table;
    this->beginResetModel();


    //m_dataFile = dataFile;

    //doc = QDomDocument("ODKDocument");
    /*QFile xmlfile(dataFile);
    if (!xmlfile.open(QIODevice::ReadOnly))
        return ;
    if (!doc.setContent(&xmlfile))
    {
        xmlfile.close();
        return ;
    }
    xmlfile.close();*/
    int index;
    index = 0;

    //qDebug() << "Begin load data";

    for (int idoc = 0; idoc <= fileList.count()-1;idoc++)
    {
        if (fileList[idoc].opened)
        {
            fileList[idoc].dataList.clear();

            if (table.name == "mainModule")
            {
                QDomNodeList records;
                QDomNodeList fields;
                int clm;
                records = fileList[idoc].doc.elementsByTagName(table.xmlCode);
                for (int pos = 0; pos <= records.count()-1;pos++)
                {
                    TdataDef record;
                    record.modified = false;
                    record.itemElement = records.item(pos).toElement();
                    record.index = index;
                    for (clm = 0; clm <= activeTable.fields.count()-1; clm++)
                    {
                        fields = record.itemElement.elementsByTagName(activeTable.fields[clm].name);
                        if (fields.count() > 0)
                        {
                            TdataItemDef item;
                            item.ignore = false;
                            item.modified = false;
                            item.itemElement = fields.item(0).toElement();
                            item.value = fields.item(0).toElement().firstChild().nodeValue();
                            record.fields.append(item);
                        }
                        else
                        {
                            TdataItemDef item;
                            item.ignore = true;
                            item.value = "NA";
                            item.modified = false;
                            record.fields.append(item);
                        }
                    }
                    fileList[idoc].dataList.append(record);
                    index++;
                }
            }
            else
            {
                QDomNodeList records;
                QDomNodeList fields;
                int clm;
                records = fileList[idoc].doc.elementsByTagName(table.name);
                for (int pos = 0; pos <= records.count()-1;pos++)
                {
                    TdataDef record;
                    record.modified = false;
                    record.itemElement = records.item(pos).toElement();
                    record.index = index;
                    for (clm = 0; clm <= activeTable.fields.count()-1; clm++)
                    {
                        fields = record.itemElement.elementsByTagName(activeTable.fields[clm].name);
                        if (fields.count() > 0)
                        {
                            TdataItemDef item;
                            item.ignore = false;
                            item.modified = false;
                            item.itemElement = fields.item(0).toElement();
                            item.value = fields.item(0).toElement().firstChild().nodeValue();
                            record.fields.append(item);
                        }
                        else
                        {
                            TdataItemDef item;
                            item.ignore = true;
                            item.value = "NA";
                            item.modified = false;
                            record.fields.append(item);
                        }
                    }
                    fileList[idoc].dataList.append(record);
                    index++;
                }
            }
        }
        else
        {
            qDebug() << fileList[idoc].dataFile + " is not opened";
        }
    }

    //qDebug() << "End load data";

    this->endResetModel();

}

void editorModel::saveData()
{
    int clm;
    int idoc;

    for (idoc = 0; idoc <= fileList.count()-1;idoc++)
    {

        for (int pos = 0; pos <= fileList[idoc].dataList.count()-1;pos++)
        {
            if (fileList[idoc].dataList[pos].modified)
            {
                for (clm = 0; clm <= fileList[idoc].dataList[pos].fields.count()-1; clm++)
                {
                    if (!fileList[idoc].dataList[pos].fields[clm].ignore)
                    {
                        if (!fileList[idoc].dataList[pos].fields[clm].itemElement.firstChild().isNull())
                            fileList[idoc].dataList[pos].fields[clm].itemElement.firstChild().setNodeValue(fileList[idoc].dataList[pos].fields[clm].value);
                        else
                        {
                            QDomText data;
                            data = fileList[idoc].doc.createTextNode(fileList[idoc].dataList[pos].fields[clm].value);
                            fileList[idoc].dataList[pos].fields[clm].itemElement.appendChild(data);
                            //dataList[pos].fields[clm].itemElement.firstChild().setNodeValue();
                        }
                        fileList[idoc].writeFile = true;
                    }
                }
            }
        }
    }

    for (idoc = 0; idoc <= fileList.count()-1;idoc++)
    {
        if (fileList[idoc].writeFile)
        {
            QFile file(fileList[idoc].dataFile);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream out(&file);
                out.setCodec("UTF-8");

                //out << doc.toString();
                fileList[idoc].doc.save(out,1,QDomNode::EncodingFromTextStream);

                file.close();
            }


        }
        else
            qDebug() << "Nothing to write for " + fileList[idoc].dataFile;
    }
    loadData(activeTable);
}


QString editorModel::getColumnLkp(int column)
{
    if (activeTable.fields[column].rTable.left(3) == "lkp")
        return activeTable.fields[column].rTable;
    else
        return "NULL";

}

QString editorModel::getDataDesc(int index) const
{
    int pos;
    for (int idoc = 0; idoc <= fileList.count()-1;idoc++)
    {
        for (pos = 0; pos <= fileList[idoc].dataList.count()-1;pos++)
        {
            if (fileList[idoc].dataList[pos].index == index)
            {
                return fileList[idoc].dataDesc;
            }
        }
    }
    return "Unknonw";
}

QString editorModel::getDataValue(int index, int column) const
{
    int pos;
    for (int idoc = 0; idoc <= fileList.count()-1;idoc++)
    {
        for (pos = 0; pos <= fileList[idoc].dataList.count()-1;pos++)
        {
            if (fileList[idoc].dataList[pos].index == index)
            {
                return fileList[idoc].dataList[pos].fields[column].value;
            }
        }
    }
    return "Unknown";
}

bool editorModel::isDataModified(int index, int column) const
{
    int pos;
    for (int idoc = 0; idoc <= fileList.count()-1;idoc++)
    {
        for (pos = 0; pos <= fileList[idoc].dataList.count()-1;pos++)
        {
            if (fileList[idoc].dataList[pos].index == index)
            {
                return fileList[idoc].dataList[pos].fields[column].modified;
            }
        }
    }
    return false;
}

bool editorModel::isDataIgnored(int index, int column) const
{
    int pos;
    for (int idoc = 0; idoc <= fileList.count()-1;idoc++)
    {
        for (pos = 0; pos <= fileList[idoc].dataList.count()-1;pos++)
        {
            if (fileList[idoc].dataList[pos].index == index)
            {
                return fileList[idoc].dataList[pos].fields[column].ignore;
            }
        }
    }
    return false;
}

void editorModel::setData2(int index, int column, QString value)
{
    int pos;
    for (int idoc = 0; idoc <= fileList.count()-1;idoc++)
    {
        for (pos = 0; pos <= fileList[idoc].dataList.count()-1;pos++)
        {
            if (fileList[idoc].dataList[pos].index == index)
            {
                fileList[idoc].dataList[pos].fields[column].value = value;
                fileList[idoc].dataList[pos].fields[column].modified = true;
                fileList[idoc].dataList[pos].modified = true;
                return;
            }
        }
    }
}

//----------------Model controlling functions-----------------------------

int editorModel::rowCount(const QModelIndex &) const
{
    int rows;
    rows = 0;
    for (int idoc = 0; idoc <= fileList.count()-1;idoc++)
    {
        rows = rows + fileList[idoc].dataList.count();
    }
    return rows;
}

int editorModel::columnCount(const QModelIndex &) const
{
    return activeTable.fields.count();
}

QVariant editorModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        return getDataValue(index.row(),index.column()); //dataList[index.row()].fields[index.column()].value;
    }
    if (role == Qt::BackgroundRole)
    {

        //if (!dataList[index.row()].fields[index.column()].modified)
        if (!isDataModified(index.row(),index.column()))
        {
            QBrush Background(QColor(255,255,255));
            Background.setStyle(Qt::SolidPattern);
            return Background;
        }
        else
        {
            QBrush Background(QColor(255,0,0));
            Background.setStyle(Qt::SolidPattern);
            return Background;
        }

    }
    if (role == Qt::ForegroundRole)
    {

        //if (dataList[index.row()].fields[index.column()].ignore)
        if (isDataIgnored(index.row(),index.column()))
        {
            QBrush FontColor(QColor(191,184,181));
            return FontColor;
        }

    }
    return QVariant();
}

bool editorModel::setData(const QModelIndex &index,const QVariant &value,int role)
{
    if (role == Qt::EditRole)
    {
        //if (!dataList[index.row()].fields[index.column()].ignore)
        if (!isDataIgnored(index.row(),index.column()))
        {
            //if (dataList[index.row()].fields[index.column()].value != value.toString())
            if (getDataValue(index.row(),index.column()) != value.toString())
            {
                //dataList[index.row()].fields[index.column()].value = value.toString();
                //dataList[index.row()].fields[index.column()].modified = true;
                //dataList[index.row()].modified = true;
                setData2(index.row(),index.column(),value.toString());
                emit dataChanged(index,index);
            }
        }
    }

    return false;
}

Qt::ItemFlags editorModel::flags(const QModelIndex &index) const
{
    if (isDataIgnored(index.row(),index.column()))
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    else
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

QVariant editorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal)
    {
        if (role == Qt::DisplayRole)
        {
            return activeTable.fields[section].desc;

        }
        if (role == Qt::ForegroundRole)
        {

            if (const_cast<editorModel *>(this)->getColumnLkp(section) != "NULL") //We cast this fuction because we are in a const function an we need to call a no const function
            {
                QBrush fontColor(QColor(0,0,255));
                return fontColor;
            }

        }
    }
    if (orientation == Qt::Vertical)
    {
        if (role == Qt::DisplayRole)
        {
            return getDataDesc(section);
        }
    }
    return QVariant();
}
