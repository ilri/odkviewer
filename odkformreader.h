#ifndef ODKFORMREADER_H
#define ODKFORMREADER_H

#include <QDomDocument>
#include <QTreeWidgetItem>
#include <genstruct.h>



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
