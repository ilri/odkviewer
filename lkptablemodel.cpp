#include "lkptablemodel.h"

lkpTableModel::lkpTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

void lkpTableModel::loadData(QList <TlkpValue > values)
{
    lstValues.append(values);
}

QString lkpTableModel::getValue(int row)
{
    return lstValues[row].code;
}

int lkpTableModel::getItemIndex(QString value)
{
    for (int pos = 0; pos <= lstValues.count()-1;pos++)
    {
        if (lstValues[pos].code == value)
            return pos;
    }
    return -1;
}

//-------------------------------------------------Model control functions----------------------------

int lkpTableModel::rowCount(const QModelIndex &) const
{
    return lstValues.count();
}

int lkpTableModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant lkpTableModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if (index.column() == 0)
            return lstValues[index.row()].code;
        if (index.column() == 1)
            return lstValues[index.row()].desc;
    }
    return QVariant();
}

bool lkpTableModel::setData(const QModelIndex &/*index*/,const QVariant &/*value*/,int /*role*/)
{
    return false;
}

Qt::ItemFlags lkpTableModel::flags(const QModelIndex &) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant lkpTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 0)
                return "Code";
            if (section == 1)
                return "Description";
        }
    }
    return QVariant();
}
