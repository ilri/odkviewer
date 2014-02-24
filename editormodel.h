#ifndef EDITORMODEL_H
#define EDITORMODEL_H

#include <QAbstractTableModel>
#include "genstruct.h"


class editorModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit editorModel(QObject *parent = 0);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void loadData(TtableDef table, QString dataFile);
    void saveData();

    QString getColumnLkp(int column);

private:
    TtableDef activeTable;
    QString m_dataFile;
    QList<TdataDef > dataList;
    QDomDocument doc;

public slots:

};

#endif // EDITORMODEL_H
