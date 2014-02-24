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



void editorModel::loadData(TtableDef table, QString dataFile)
{
    this->beginResetModel();

    activeTable = table;
    m_dataFile = dataFile;

    doc = QDomDocument("ODKDocument");
    QFile xmlfile(dataFile);
    if (!xmlfile.open(QIODevice::ReadOnly))
        return ;
    if (!doc.setContent(&xmlfile))
    {
        xmlfile.close();
        return ;
    }
    xmlfile.close();

    dataList.clear();


    if (table.name == "mainForm")
    {
        QDomNodeList records;
        QDomNodeList fields;
        int clm;
        records = doc.elementsByTagName(table.xmlCode);
        for (int pos = 0; pos <= records.count()-1;pos++)
        {
            TdataDef record;
            record.modified = false;
            record.itemElement = records.item(pos).toElement();
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
            dataList.append(record);
        }
    }
    else
    {
        QDomNodeList records;
        QDomNodeList fields;
        int clm;
        records = doc.elementsByTagName(table.name);
        for (int pos = 0; pos <= records.count()-1;pos++)
        {
            TdataDef record;
            record.modified = false;
            record.itemElement = records.item(pos).toElement();
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
            dataList.append(record);
        }
    }


    this->endResetModel();

}

void editorModel::saveData()
{
    int clm;
    bool writeFile;
    writeFile = false;
    for (int pos = 0; pos <= dataList.count()-1;pos++)
    {
        if (dataList[pos].modified)
        {
            for (clm = 0; clm <= dataList[pos].fields.count()-1; clm++)
            {
                if (!dataList[pos].fields[clm].ignore)
                {
                    if (!dataList[pos].fields[clm].itemElement.firstChild().isNull())
                        dataList[pos].fields[clm].itemElement.firstChild().setNodeValue(dataList[pos].fields[clm].value);
                    else
                    {
                        QDomText data;
                        data = doc.createTextNode(dataList[pos].fields[clm].value);
                        dataList[pos].fields[clm].itemElement.appendChild(data);
                        //dataList[pos].fields[clm].itemElement.firstChild().setNodeValue();
                    }
                    writeFile = true;
                }
            }
        }
    }

    if (writeFile)
    {
        QFile file(m_dataFile);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {            
            QTextStream out(&file);
            out.setCodec("UTF-8");

            //out << doc.toString();
            doc.save(out,1,QDomNode::EncodingFromTextStream);

            file.close();
        }

        loadData(activeTable,m_dataFile);
    }
    else
        qDebug() << "Nothing to write";
}


QString editorModel::getColumnLkp(int column)
{
    if (activeTable.fields[column].rTable.left(3) == "lkp")
        return activeTable.fields[column].rTable;
    else
        return "NULL";

}

//----------------Model controlling functions-----------------------------

int editorModel::rowCount(const QModelIndex &) const
{
    return dataList.count();
}

int editorModel::columnCount(const QModelIndex &) const
{
    return activeTable.fields.count();
}

QVariant editorModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        return dataList[index.row()].fields[index.column()].value;
    }
    if (role == Qt::BackgroundRole)
    {
        if (!dataList[index.row()].fields[index.column()].modified)
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
        if (dataList[index.row()].fields[index.column()].ignore)
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
        if (!dataList[index.row()].fields[index.column()].ignore)
        {
            if (dataList[index.row()].fields[index.column()].value != value.toString())
            {
                dataList[index.row()].fields[index.column()].value = value.toString();
                dataList[index.row()].fields[index.column()].modified = true;
                dataList[index.row()].modified = true;
                emit dataChanged(index,index);
            }
        }
    }

    return false;
}

Qt::ItemFlags editorModel::flags(const QModelIndex &index) const
{
    if (dataList[index.row()].fields[index.column()].ignore)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    else
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

QVariant editorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    int sect;
    sect = section;
    if (orientation == Qt::Horizontal)
    {
        if (role == Qt::DisplayRole)
        {
            return activeTable.fields[section].desc;

        }
        if (role == Qt::ForegroundRole)
        {
            if (const_cast<editorModel *>(this)->getColumnLkp(sect) != "NULL") //We cast this fuction because we are in a const function an we need to call a no const function
            {
                QBrush fontColor(QColor(0,0,255));
                return fontColor;
            }
        }
    }
    return QVariant();
}
