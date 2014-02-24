#include "odkformreader.h"
#include <QtAlgorithms>
#include <QStringList>
#include <QFile>
#include <QDebug>



odkFormReader::odkFormReader()
{

}


int odkFormReader::getTableIndex(QString table)
{
    int res;
    res = 0;
    for (int pos = 0; pos <= tables.count()-1; pos++)
    {
        if (tables[pos].name == table)
            return tables[pos].parentIndex;
    }
    return res;
}

bool odkFormReader::tableExist(QString table)
{
    for (int pos = 0; pos <= tables.count()-1; pos++)
    {
        if (tables[pos].name == table)
            return true;
    }
    return false;
}

bool lkpComp(TlkpValue left, TlkpValue right)
{
  return left.code < right.code;
}

bool tblComp(TtableDef left, TtableDef right)
{
    return left.pos < right.pos;
}

QString odkFormReader::isLkpDuplicated(TtableDef table)
{
    QString res;
    int pos2;
    bool same;

    // We use this two lists to compare values after sorting them by code
    // because of ordering some values might not match.
    QList<TlkpValue> thisValues;
    QList<TlkpValue> currentValues;

    //Move the new list of values to a new list and sort it by code
    thisValues.clear();
    thisValues.append(table.lkpValues);
    qSort(thisValues.begin(),thisValues.end(),lkpComp);

    //qDebug() << table.name;
    for (int pos = 0; pos <= tables.count()-1; pos++)
    {
        if (tables[pos].islookup)
        {
            //Move the current list of values to a new list and sort it by code
            currentValues.clear();
            currentValues.append(tables[pos].lkpValues);
            qSort(currentValues.begin(),currentValues.end(),lkpComp);

            if (currentValues.count() == thisValues.count()) //Same number of values
            {
                res = tables[pos].name;
                same = true;
                for (pos2 = 0; pos2 <= tables[pos].lkpValues.count()-1;pos2++)
                {
                    //Compares if an item in the list dont have same code or same description
                    if ((currentValues[pos2].code.simplified().toLower() != thisValues[pos2].code.simplified().toLower()) ||
                            (currentValues[pos2].desc.simplified().toLower() != thisValues[pos2].desc.simplified().toLower()))
                    {
                        res = "";
                        same = false;
                        break;
                    }
                }
                if (same)
                {
                    return res;
                }
            }
        }
    }
    return res;
}

QString odkFormReader::getKeyField(QString table)
{
    int pos2;
    QString res;
    for (int pos = 0; pos <= tables.count()-1; pos++)
    {
        if (tables[pos].name == table)
        {
            for (pos2 = 0; pos2 <= tables[pos].fields.count()-1;pos2++)
            {
                if (tables[pos].fields[pos2].key == true)
                {
                    res = tables[pos].fields[pos2].name;
                    return res;
                }
            }
        }
    }
    return res;
}


QString odkFormReader::getForeignColumns(TtableDef table, QString relatedTable)
{
    QString res;
    int pos;
    for (pos = 0; pos <= table.fields.count()-1; pos++)
    {
        if (table.fields[pos].rTable == relatedTable)
        {
            res = res + table.fields[pos].name + ",";
        }
    }
    res = res.left(res.length()-1);
    return res;
}

QString odkFormReader::getReferencedColumns(TtableDef table, QString relatedTable)
{
    QString res;
    int pos;
    for (pos = 0; pos <= table.fields.count()-1; pos++)
    {
        if (table.fields[pos].rTable == relatedTable)
        {
            res = res + table.fields[pos].rField + ",";
        }
    }
    res = res.left(res.length()-1);
    return res;
}

bool odkFormReader::isRelatedTableLookUp(QString relatedTable)
{
    int pos;
    for (pos = 0; pos <= tables.count()-1; pos++)
    {
        if (tables[pos].name.toLower() == relatedTable.toLower())
            return tables[pos].islookup;
    }
    return false;
}

QString odkFormReader::getFieldType(QString itemID)
{
    QDomNodeList list;
    QDomElement item;
    list = doc.elementsByTagName("bind");
    int pos;
    QString res;
    res = "Unknown";
    for (pos = 0; pos <= list.count()-1;pos++)
    {
        item = list.item(pos).toElement();
        if (item.attribute("nodeset","Unknown") == itemID)
        {
            if (item.attribute("readonly","NOP") == "NOP")
            {
                res = item.attribute("type","Unknown");
                break;
            }
            else
            {
                res = "READONLY";
                break;
            }
        }
    }
    if (res == "Unknown")
        return res;

    if (res != "READONLY")
    {
        if (res == "string")
        {
            return "varchar|255";
        }
        if (res == "decimal")
        {
            return "decimal|7,3";
        }
        if (res == "int")
        {
            return "int|9";
        }
        if (res == "select")
        {
            return "int|9"; //This has to change to a different table to store the values
        }
        if (res == "select1")
        {
            return "int|9";
        }
        if (res == "int")
        {
            return "int|9";
        }
        if (res == "date")
        {
            return "date";
        }
        if (res == "time")
        {
            return "time";
        }
        return "varchar|255";
    }
    else
        return res;
}

QString odkFormReader::getItemLabel(QDomNode node)
{
    while (!node.isNull())
    {
        if (node.toElement().tagName() == "label")
        {
            return node.firstChild().nodeValue();
        }

        node = node.nextSibling();
    }
    return "NotLabelFound";
}

QString odkFormReader::getItemOutput(QDomNode node)
{
    QDomElement item;
    QDomNodeList list;


    while (!node.isNull())
    {
        if (node.toElement().tagName() == "label")
        {
            item = node.toElement();
            list = item.elementsByTagName("output");
            if (list.count() > 0)
            {
                item = list.item(0).toElement();
                return item.attribute("value","NONE");
            }
            else
                return "NONE";
        }

        node = node.nextSibling();
    }
    return "NONE";
}

QList<TlkpValue> odkFormReader::getSelectValues(QDomElement item)
{
    QList<TlkpValue> res;
    QDomNodeList list;
    QDomNode nodeDesc;
    QDomNode nodeCode;
    int pos;
    list = item.elementsByTagName("item");
    if (list.count() > 0)
    {
        for (pos = 0; pos <= list.count()-1;pos++)
        {
            nodeDesc = list.item(pos).firstChild();
            nodeCode = list.item(pos).firstChild().nextSibling();
            TlkpValue value;
            value.code = nodeCode.firstChild().nodeValue();
            value.desc = nodeDesc.firstChild().nodeValue();
            res.append(value);
        }
    }
    return res;
}

bool odkFormReader::areValuesStrings(QList<TlkpValue> values)
{

    bool ok;
    int pos;
    for (pos = 0; pos <= values.count()-1;pos++)
    {
        values[pos].code.toInt(&ok,10);
        if (!ok)
        {
            return true;
        }
    }
    return false;
}

int odkFormReader::getMaxValueLength(QList<TlkpValue> values)
{
    int res;
    res = 0;
    int pos;
    for (pos = 0; pos <= values.count()-1;pos++)
    {
        if (values[pos].code.length() >= res)
            res = values[pos].code.length();
    }
    return res;
}

bool odkFormReader::isLookUpYesNo(QList<TlkpValue> values)
{
    bool hasNo;
    bool hasYes;
    int lkp;
    if (values.count() == 2 )
    {
        hasNo = false;
        hasYes = false;
        for (lkp = 0; lkp <= values.count()-1;lkp++)
        {
            if ((values[lkp].code.toInt() == 0) && (values[lkp].desc.trimmed().toLower() == "no"))
                hasNo = true;
            if ((values[lkp].code.toInt() == 1) && (values[lkp].desc.trimmed().toLower() == "yes"))
                hasYes = true;
        }
        if (hasYes && hasNo)
            return true;
        else
            return false;
    }
    else
        return false;
}

QList<QTreeWidgetItem *> odkFormReader::getTreeItems()
{
    return treeItems;
}

int odkFormReader::insertNodes(int parent, QString caption, QString tableName,QString xmlTag)
{
    int res;
    QTreeWidgetItem *newItem;
    if (parent == -1)
    {
        newItem = new QTreeWidgetItem(QStringList(caption));
        newItem->setData(0,Qt::UserRole,tableName);
        newItem->setData(0,Qt::UserRole+1,xmlTag);
        treeItems.append(newItem);
    }
    else
    {
        newItem = new QTreeWidgetItem(treeItems[parent],QStringList(caption));
        newItem->setData(0,Qt::UserRole,tableName);
        newItem->setData(0,Qt::UserRole+1,xmlTag);
        treeItems.append(newItem);
    }
    res = treeItems.count()-1; //We get here the index of the last item
    //Change some flags and set the state of the node of unchecked
    treeItems[res]->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    return res; //We return the new node so can be used to set childs
}


TtableDef odkFormReader::getTable(QString tableName)
{
    TtableDef res;
    for (int pos = 0; pos <= tables.count()-1; pos++)
    {
        if (tables[pos].name == tableName)
        {
            return tables[pos];
        }
    }
    return res;
}

QList<TlkpValue> odkFormReader::getLookUpValues(QString table)
{
    QList<TlkpValue > res;
    for (int pos = 0; pos <= tables.count()-1; pos++)
    {
        if (tables[pos].islookup == true)
        {
            if (tables[pos].name == table)
            {
                res.append(tables[pos].lkpValues);
                break;
            }
        }
    }
    return res;
}

//------------------------------ Main recursive process that extract the data from the ODK XML file -------------------------------

void odkFormReader::extractFields(QDomNode child, QString tableName, TtableDef &parentTable, QString groupCode, QString xmlCode)
{
    QDomElement item;
    QDomNode node;
    QDomNode childNode;

    QString fieldCode;
    QString fieldName;
    QString fieldOutput;
    QString fieldType;
    QStringList ftypes;
    QStringList ftsizes;

    QString sameTable;
    QString grpCode;

    QString childName;
    QString mselptable;
    QString repeatXMLCode;
    QString fieldXMLCode;


    TtableDef table;
    TtableDef tempTable;
    int pos;

    tableIndex++;
    int tindex;
    tindex = tableIndex;

    bool fieldinmselt;

    TfieldDef fsurveyID;
    TfieldDef foriginID;



    if ((tableName.toLower() != parentTable.name.toLower()) || (parentTable.fields.count() == 0)) //We are not processing a group
    {
        //qDebug() << "Table: " + tableName.toLower() + " Parent: " + parentTable.name.toLower() + ". Fields: " + QString::number(parentTable.fields.count());

        if (tableName.toLower() == mTable.toLower())
        {
            table.name = tableName;
            table.desc = "Main survey Table - Change this description";
            table.xmlCode = xmlCode;
            table.parentTable = "NULL";

            /*fsurveyID.name = "surveyid";
            fsurveyID.desc = "Survey ID (UUID)";
            fsurveyID.type = "varchar";
            fsurveyID.size = 80;
            fsurveyID.decSize = 0;
            fsurveyID.section = "main";
            fsurveyID.rField = "";
            fsurveyID.rTable = "";
            fsurveyID.key = false;
            fsurveyID.xmlCode = "NONE";
            table.fields.append(fsurveyID);

            foriginID.name = "originid";
            foriginID.desc = "Origin ID: formhub or aggregate";
            foriginID.type = "varchar";
            foriginID.size = 15;
            foriginID.decSize = 0;
            foriginID.section = "main";
            foriginID.rField = "";
            foriginID.rTable = "";
            foriginID.key = false;
            foriginID.xmlCode = "NONE";
            table.fields.append(foriginID);*/

        }
        else
        {
            table.name = tableName;
            table.xmlCode = xmlCode;
            table.parentTable = parentTable.name;
            table.desc = "Repeat " + tableName + " - Change this description";
            /*
            //Add the key fields of the parent
            for (pos = 0; pos <= parentTable.fields.count()-1;pos++)
            {
                if (parentTable.fields[pos].key == true)
                {
                    TfieldDef parentKeyField;
                    parentKeyField.name = parentTable.fields[pos].name;
                    parentKeyField.desc = parentTable.fields[pos].desc;
                    parentKeyField.key = true;
                    parentKeyField.type = parentTable.fields[pos].type;
                    parentKeyField.size = parentTable.fields[pos].size;
                    parentKeyField.decSize = parentTable.fields[pos].decSize;
                    parentKeyField.rTable = parentTable.name;
                    parentKeyField.rField = parentTable.fields[pos].name;
                    parentKeyField.xmlCode = "NONE";
                    table.fields.append(parentKeyField);
                }
            }*/

            /*TfieldDef KeyField;
            KeyField.name = tableName + "_rowid";
            KeyField.section = "main";
            KeyField.desc = "Unique row ID";
            KeyField.key = true;
            KeyField.type = "int";
            KeyField.size = 3;
            KeyField.decSize = 0;
            KeyField.rTable = "";
            KeyField.rField = "";
            KeyField.xmlCode = "NONE";
            table.fields.append(KeyField);*/

        }
    }



    node = child;
    while (!node.isNull())
    {
        item = node.toElement();
        if ((item.tagName() == "input") || (item.tagName() == "select1") || (item.tagName() == "select"))
        {
            childNode = item.firstChild();
            fieldCode = item.attribute("ref","CodeNotFound!");
            fieldName = getItemLabel(childNode);
            fieldOutput = getItemOutput(childNode);
            if (fieldOutput != "NONE")
            {
                if (getFieldType(fieldOutput) != "Unknown")
                    fieldCode = fieldOutput;
            }

            fieldName = fieldName.replace("\r","");
            fieldName = fieldName.replace("\n","");
            fieldName = fieldName.replace("\t","");
            fieldName = fieldName.replace("'","");
            fieldName = fieldName.replace(";","");

            fieldType = getFieldType(fieldCode);

            fieldXMLCode = fieldCode.replace("/" + surveyID + "/","");

            pos = fieldCode.lastIndexOf("/");
            fieldCode = fieldCode.right(fieldCode.length()-pos-1);

            fieldCode = fieldCode.replace("-","_");
            fieldCode = fieldCode.replace("'","");
            fieldCode = fieldCode.replace(";","");

            //qDebug() << fieldCode;
            //qDebug() << MainField.toLower();


            if (fieldType != "READONLY")
            {
                TfieldDef field;

                field.name = fieldCode;
                field.xmlCode = fieldXMLCode;
                field.section = groupCode;
                field.desc = fieldName;
                //if (fieldCode.toLower() == MainField.toLower())
                //    field.key = true;
                //else
                    field.key = false;

                ftypes = fieldType.split("|",QString::SkipEmptyParts);
                if (ftypes.count() > 1)
                {
                    field.type = ftypes[0];
                    ftsizes =  ftypes[1].split(",",QString::SkipEmptyParts);
                    if (ftsizes.count() > 1)
                    {
                        field.size = ftsizes[0].toInt();
                        field.decSize = ftsizes[1].toInt();
                    }
                    else
                    {
                        field.size = ftsizes[0].toInt();
                        field.decSize = 0;
                    }
                }
                else
                    field.type = fieldType;

                field.rTable = "";
                field.rField = "";
                fieldinmselt = false;

                if (item.tagName() == "select1")
                {
                    if ((isLookUpYesNo(getSelectValues(item)) == false) || ((isLookUpYesNo(getSelectValues(item)) == true) && (ignoreYesNo == false)))
                    {

                        TtableDef lktable;


                        lktable.lkpValues.append(getSelectValues(item));



                        if (areValuesStrings(lktable.lkpValues))
                        {
                            //qDebug() << lktable.name;
                            field.type = "varchar";
                            field.size = getMaxValueLength(lktable.lkpValues);
                            field.decSize = 0;
                        }

                        lktable.name = "lkp" + fieldCode;
                        lktable.desc = "Lookup table [" + fieldName + "]";
                        lktable.pos = -1;
                        lktable.islookup = true;

                        TfieldDef lkpCode;
                        lkpCode.name = fieldCode + "_cod";
                        lkpCode.desc = "Code";
                        lkpCode.key = true;
                        lkpCode.type = field.type;
                        lkpCode.size = field.size;
                        lkpCode.decSize = field.decSize;
                        lktable.fields.append(lkpCode);

                        TfieldDef lkpDesc;
                        lkpDesc.name = fieldCode + "_des";
                        lkpDesc.desc = "Description";
                        lkpDesc.key = false;
                        lkpDesc.type = "varchar";
                        lkpDesc.size = 255;
                        lkpDesc.decSize = 0;
                        lktable.fields.append(lkpDesc);



                        sameTable = isLkpDuplicated(lktable);

                        if (sameTable.isEmpty())
                        {
                            tables.append(lktable);
                            field.rTable = lktable.name;
                            field.rField = fieldCode + "_cod";
                        }
                        else
                        {
                            field.rTable = sameTable;
                            field.rField = getKeyField(sameTable);
                            //qDebug() << lktable.name + " = " + sameTable + ". " + sameTable + " used instead";
                        }
                    }
                }
                if (item.tagName() == "select")
                {

                    //We change the field type to be varchar(255) and add it to the main table so stores values separated by space. The by a separate process we split the values in the child tables.
                    field.type = "varchar";
                    field.size = 255;
                    field.decSize = 0;

                    if (tableName.toLower() != parentTable.name.toLower()) //If we are not processing a group
                        table.fields.append(field);
                    else
                        parentTable.fields.append(field); //If we are processing a group then the field goes to the parent

                    //Then we change it back to a select type
                    field.type = "int";
                    field.size = 9;


                    //*********************Creates a new table to store the multiselect in different rows**************************

                    TtableDef mselTable;
                    if (tableName.toLower() != parentTable.name.toLower()) //If we are not processing a group
                    {
                        mselptable = table.name;
                        tempTable = table;
                    }
                    else
                    {
                        mselptable = parentTable.name;
                        tempTable = parentTable;
                    }

                    mselTable.name =  mselptable + "_msel_" + fieldCode;
                    mselTable.desc = "Table for multiple select of field " + fieldCode;
                    mselTable.islookup = false;
                    tableIndex++;
                    mselTable.pos = tableIndex;
                    mselTable.xmlCode = "NONE";

                    //Move the keys to the multi select table
                    for (pos = 0; pos <= tempTable.fields.count()-1;pos++)
                    {
                        if (tempTable.fields[pos].key == true)
                        {
                            TfieldDef mselKeyField;
                            mselKeyField.name = tempTable.fields[pos].name;
                            mselKeyField.desc = tempTable.fields[pos].desc;
                            mselKeyField.key = true;
                            mselKeyField.type = tempTable.fields[pos].type;
                            mselKeyField.size = tempTable.fields[pos].size;
                            mselKeyField.decSize = tempTable.fields[pos].decSize;
                            mselKeyField.rTable = tempTable.name;
                            mselKeyField.rField = tempTable.fields[pos].name;
                            mselTable.fields.append(mselKeyField);
                        }
                    }

                    //************************* Creates the lookup table *********************************

                    if ((isLookUpYesNo(getSelectValues(item)) == false) || ((isLookUpYesNo(getSelectValues(item)) == true) && (ignoreYesNo == false)))
                    {

                        TtableDef lktable;


                        lktable.lkpValues.append(getSelectValues(item));



                        if (areValuesStrings(lktable.lkpValues))
                        {
                            //qDebug() << lktable.name;
                            field.type = "varchar";
                            field.size = getMaxValueLength(lktable.lkpValues);
                            field.decSize = 0;
                        }

                        lktable.name = "lkp" + fieldCode;
                        lktable.desc = "Lookup table [" + fieldName + "]";
                        lktable.pos = -1;
                        lktable.islookup = true;

                        TfieldDef lkpCode;
                        lkpCode.name = fieldCode + "_cod";
                        lkpCode.desc = "Code";
                        lkpCode.key = true;
                        lkpCode.type = field.type;
                        lkpCode.size = field.size;
                        lkpCode.decSize = field.decSize;
                        lktable.fields.append(lkpCode);

                        TfieldDef lkpDesc;
                        lkpDesc.name = fieldCode + "_des";
                        lkpDesc.desc = "Description";
                        lkpDesc.key = false;
                        lkpDesc.type = "varchar";
                        lkpDesc.size = 255;
                        lkpDesc.decSize = 0;
                        lktable.fields.append(lkpDesc);



                        sameTable = isLkpDuplicated(lktable);

                        if (sameTable.isEmpty())
                        {
                            tables.append(lktable);
                            field.rTable = lktable.name;
                            field.rField = fieldCode + "_cod";
                        }
                        else
                        {
                            field.rTable = sameTable;
                            field.rField = getKeyField(sameTable);
                            //qDebug() << lktable.name + " = " + sameTable + ". " + sameTable + " used instead";
                        }
                    }

                    fieldinmselt = true;
                    field.key = true;
                    mselTable.fields.append(field);
                    tables.append(mselTable);

                }
                if (fieldinmselt == false)
                {
                    if (tableName.toLower() != parentTable.name.toLower()) //If we are not processing a group
                        table.fields.append(field);
                    else
                        parentTable.fields.append(field); //If we are processing a group then the field goes to the parent
                }

            }
        }
        else
        {
            if (item.tagName() == "group")
            {
                grpCode = item.attribute("ref","/GRPNotKnown");
                pos = grpCode.lastIndexOf("/");
                grpCode = grpCode.right(grpCode.length()-pos-1);
                grpCode = grpCode;
                //qDebug() << "Group:" + item.attribute("ref","/GRPNotKnown");
                //qDebug() << "Group table:" + table.name;

                if (!table.name.isEmpty())
                    extractFields(item.firstChild(),table.name,table,grpCode,xmlCode); //Recursive call for group
                else
                    extractFields(item.firstChild(),parentTable.name,parentTable,grpCode,xmlCode); //Recursive call for group
            }
            if (item.tagName() == "repeat")
            {
                childName = item.attribute("nodeset","unknowrepeat");

                repeatXMLCode = childName.replace("/" + surveyID + "/","");

                pos = childName.lastIndexOf("/");
                childName = childName.right(childName.length()-pos-1);
                if (!table.name.isEmpty())
                    extractFields(item.firstChild(),childName,table,"main",repeatXMLCode); //Recursive call for repeat
                else
                    extractFields(item.firstChild(),childName,parentTable,"main",repeatXMLCode); //Recursive call for repeat
            }
        }


        node = node.nextSibling();
    }

    if (tableName.toLower() != parentTable.name.toLower())
    {
        table.pos = tindex;
        table.islookup = false;
        tables.append(table);
    }

}

//-------------------------- Public fuction that starts the ODK reader -----------------------------------------

void odkFormReader::processXML(QString inputFile, QString mainTable)
{
    tables.clear();
    treeItems.clear();

    doc = QDomDocument("ODKDocument");
    QFile xmlfile(inputFile);
    if (!xmlfile.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&xmlfile))
    {
        xmlfile.close();
        return;
    }
    xmlfile.close();

    QDomNodeList list;
    QDomElement item;
    QDomNode node;

    QDomElement mainInstance;

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


    surveyID =item.tagName();


    list = doc.elementsByTagName("h:body");
    if (list.count() > 0)
    {
        node = list.item(0).firstChild();
        TtableDef nulltable;
        nulltable.name = "NULL";
        extractFields(node,mainTable,nulltable,"main",surveyID);
    }

    //Now that we have the list of tables we move the list into a tree


}

void odkFormReader::createItemsTree()
{
    treeItems.clear();

    int tblindex;
    int parentindex;

    qSort(tables.begin(),tables.end(),tblComp);

    for (int pos = 0; pos <= tables.count()-1;pos++)
    {
        if (tables[pos].islookup == false)
        {
            if (tables[pos].xmlCode != "NONE")
            {
                //qDebug() << "Table:" + tables[pos].name;
                if (tables[pos].parentTable == "NULL")
                {
                    tblindex = insertNodes(-1,tables[pos].name,tables[pos].name,tables[pos].xmlCode);
                    //qDebug() << tables[pos].xmlCode;
                    tables[pos].parentIndex = tblindex;
                }
                else
                {
                    //qDebug() << "Parent:" + tables[pos].parentTable;
                    parentindex = getTableIndex(tables[pos].parentTable);
                    //qDebug() << "Parent index:" + QString::number(parentindex);

                    tblindex = insertNodes(parentindex,tables[pos].name,tables[pos].name,tables[pos].xmlCode);
                    tables[pos].parentIndex = tblindex;
                }
            }
        }
    }

}
