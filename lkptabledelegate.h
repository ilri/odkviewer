#ifndef LKPTABLEDELEGATE_H
#define LKPTABLEDELEGATE_H

#include <QStyledItemDelegate>
#include "genstruct.h"

class lkpTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit lkpTableDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
                       const QModelIndex &index) const;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void loadItems(QList< TlkpValue> items);

private:
    QList< TlkpValue> m_items;

};

#endif // LKPTABLEDELEGATE_H
