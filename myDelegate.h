#ifndef MYDELEGATE_H
#define MYDELEGATE_H

//Reimplementacja QItemDelegate dla itemów z customerWindow

#include <QItemDelegate>
class myDelegate : public QItemDelegate
{
public:
    myDelegate(int margin, QObject* parent);



    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;


private:
    int m_margin;
};
#endif // MYDELEGATE_H
