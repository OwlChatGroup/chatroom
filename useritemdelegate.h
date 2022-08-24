#ifndef USERITEMDELEGATE_H
#define USERITEMDELEGATE_H
#include <QStyledItemDelegate>

class UserItemDelegate : public QStyledItemDelegate
{
public:
    UserItemDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

};

#endif // USERITEMDELEGATE_H
