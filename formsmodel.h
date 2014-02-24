#ifndef FORMSMODEL_H
#define FORMSMODEL_H

#include <QAbstractTableModel>
#include <QList>


struct formItem
{
  QString code;
  QString tagName;
  QString description;
  QString version;
  QString file;
};
typedef formItem TformItem;

class formsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit formsModel(QObject *parent = 0);
    ~formsModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void loadForms(QString directory);

    QString getFormID(int index);
    QString getFormDesc(int index);
    QString getFormVersion(int index);
    QString getFormFile(int index);
    QString getFormTag(int index);


private:
    QList <TformItem > forms;
    void loadFile(QString file);


};

#endif // FORMSMODEL_H
