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

#ifndef GENSTRUCT_H
#define GENSTRUCT_H

#include <QDomElement>

// Different kinds of structures used by the ODK reader and the data models.

struct fieldDef
{
  QString name;
  QString desc;
  QString type;
  int size;
  int decSize;
  QString rTable;
  QString rField;
  bool key;
  QString section;
  QString xmlCode;
};
typedef fieldDef TfieldDef;

struct dataItemDef
{
    QString value;
    QDomElement itemElement;
    bool ignore;
    bool modified;
};
typedef dataItemDef TdataItemDef;

struct dataDef
{
    bool modified;
    QList <TdataItemDef> fields;
    QDomElement itemElement;
    int index;
};
typedef dataDef TdataDef;

struct dataFileDef
{
    QString dataFile;
    QString dataDesc;
    QList<TdataDef > dataList;
    QDomDocument doc;
    bool opened;
    bool writeFile;
};
typedef dataFileDef TdataFileDef;


//Lookup value structure
struct lkpValue
{
  QString code;
  QString desc;
};
typedef lkpValue TlkpValue;

//Table structure
struct tableDef
{
  QString name;
  QString desc;
  QList<TfieldDef> fields;
  QList<TlkpValue> lkpValues;
  int pos;
  bool islookup;
  QString xmlCode;
  QString parentTable;
  int parentIndex;
};
typedef tableDef TtableDef;

#endif // GENSTRUCT_H
