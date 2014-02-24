#ifndef LKPTABLEMODEL_H
#define LKPTABLEMODEL_H

#include <QAbstractTableModel>
#include <genstruct.h>

class lkpTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit lkpTableModel(QObject *parent = 0);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void loadData(QList <TlkpValue > values);

    QString getValue(int row);
    int getItemIndex(QString value);


private:

    QList <TlkpValue > lstValues;


};

#endif // LKPTABLEMODEL_H
