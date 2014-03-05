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

#ifndef ODKFORMREADER_H
#define ODKFORMREADER_H

#include <QDomDocument>
#include <QTreeWidgetItem>
#include <genstruct.h>

// This class reads an ODK Survey XML file and stores each variable in
// independent tables. Lookup tables are also stored in separate tables.

class odkFormReader
{
public:
    odkFormReader();
    void processXML(QString inputFile, QString mainTable);

    QList<QTreeWidgetItem *> getTreeItems();
    TtableDef getTable(QString tableName);
    QList<TlkpValue > getLookUpValues(QString table);
    void createItemsTree();



private:
    QList<TtableDef> tables; //List of tables
    fieldDef mainID;
    bool ignoreYesNo;
    QString surveyID;
    QDomDocument doc;
    QString mTable;
    int tableIndex;
    QList<QTreeWidgetItem *> treeItems;

//----------------------------

    bool tableExist(QString table);
    int getTableIndex(QString table);
    QString isLkpDuplicated(TtableDef table);
    QString getKeyField(QString table);
    QString getForeignColumns(TtableDef table, QString relatedTable);
    QString getReferencedColumns(TtableDef table, QString relatedTable);
    bool isRelatedTableLookUp(QString relatedTable);
    QString getFieldType(QString itemID);
    QString getItemLabel(QDomNode node);
    QString getItemOutput(QDomNode node);
    QList<TlkpValue> getSelectValues(QDomElement item);
    bool areValuesStrings(QList<TlkpValue> values);
    int getMaxValueLength(QList<TlkpValue> values);
    bool isLookUpYesNo(QList<TlkpValue> values);
    void extractFields(QDomNode child, QString tableName, TtableDef &parentTable, QString groupCode, QString xmlCode);

    int insertNodes(int parent, QString caption, QString tableName,QString xmlTag);


};

#endif // ODKFORMREADER_H
