#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QAbstractTableModel>
#include <QList>


struct dataItem
{
  QString name;
  QString path;
};
typedef dataItem TdataItem;

class dataModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit dataModel(QObject *parent = 0);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void loadData(QString dataDir, QString surveyID, QString surveyTag);

    QString getDataSetPath(int index);
    QString getDataSetName(int index);

private:
    QList <TdataItem > dataFiles;

    void loadFile(QString fileName, QString SurveyID, QString fileDesc, QString surveyTag);


};

#endif // DATAMODEL_H
