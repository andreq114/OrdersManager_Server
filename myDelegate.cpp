#include "mydelegate.h"

myDelegate::myDelegate(int margin, QObject* parent) : QItemDelegate(parent),
    m_margin(margin)
{}


void myDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem itemOption(option);

    // Make the 'drawing rectangle' smaller.
    itemOption.rect.adjust(0, m_margin, -0, -m_margin);


    QItemDelegate::paint(painter, itemOption, index);
}
