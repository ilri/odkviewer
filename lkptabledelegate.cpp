#include "lkptabledelegate.h"
#include <QFontMetrics>
#include <QPainter>
#include <QLabel>
#include <QFileDialog>
#include <QLineEdit>
#include <QDebug>

lkpTableDelegate::lkpTableDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void lkpTableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    QString description;
    description = "Set a value";
    for (int pos = 0; pos <= m_items.count()-1; pos++)
    {
        if (m_items[pos].code.simplified().toUpper() == index.data().toString().simplified().toUpper())
        {
            description = m_items[pos].desc;
            break;
        }
    }


    //QStyleOptionViewItem myOption = option;
    //myOption.font.setBold(true);

    //painter->drawText(0,0,description);
    painter->setBrush(option.backgroundBrush);
    painter->setFont(option.font);
    painter->drawText(option.rect,Qt::AlignCenter,"Carlos");
    //QStyledItemDelegate::paint(painter,myOption,index);

}

QSize lkpTableDelegate::sizeHint(const QStyleOptionViewItem &,
               const QModelIndex &index) const
{
    QString description;
    description = "Something";
    for (int pos = 0; pos <= m_items.count()-1; pos++)
    {
        if (m_items[pos].code.simplified().toUpper() == index.data().toString().simplified().toUpper())
        {
            description = m_items[pos].desc;
            break;
        }
    }
    QLabel tlabel;
    QFontMetrics fm(tlabel.font());
    return fm.size(Qt::TextSingleLine,description);
}

QWidget *lkpTableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &,
                           const QModelIndex &/*index*/) const
{
    QLineEdit *edit;
    edit = new QLineEdit(parent);

    return edit;
}

void lkpTableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLineEdit *edit = static_cast<QLineEdit*>(editor);


    edit->setText(index.data().toString());

}

void lkpTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                       const QModelIndex &index) const
{
    QLineEdit *edit = static_cast<QLineEdit*>(editor);
    model->setData(index, edit->text(), Qt::EditRole);
}

void lkpTableDelegate::updateEditorGeometry(QWidget *editor,
         const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}

//-------------------------------------------

void lkpTableDelegate::loadItems(QList<TlkpValue> items)
{
    m_items.append(items);
}
