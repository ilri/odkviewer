#ifndef GENSTRUCT_H
#define GENSTRUCT_H

#include <QDomElement>


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
};
typedef dataDef TdataDef;

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
