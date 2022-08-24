#include "useritemdelegate.h"
#include "UserListData.h"
#include <QPainter>

UserItemDelegate::UserItemDelegate()
{

}

void UserItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.isValid())
    {
        painter->save();
        QVariant var = index.data(Qt::UserRole + 1);
        UserItemData ItemData = var.value<UserItemData>();

        //item矩形区域
        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth(option.rect.width()-1);
        rect.setHeight(option.rect.height()-1);

        //QPainterPath画圆角矩形
        QPainterPath path;
        path.moveTo(rect.topRight());
        path.lineTo(rect.topLeft());
        path.quadTo(rect.topLeft(), rect.topLeft());
        path.lineTo(rect.bottomLeft());
        path.quadTo(rect.bottomLeft(), rect.bottomLeft());
        path.lineTo(rect.bottomRight());
        path.quadTo(rect.bottomRight(), rect.bottomRight());
        path.lineTo(rect.topRight());
        path.quadTo(rect.topRight(), rect.topRight());



        // 鼠标悬停或者选中时改变背景色
        if (option.state.testFlag(QStyle::State_MouseOver)) {
            painter->setPen(QPen(QColor(230,230,230)));
            painter->setBrush(QColor(230,230,230));
            painter->drawPath(path);
        }
        if (option.state.testFlag(QStyle::State_Selected)) {
            painter->setPen(QPen(QColor(211,211,211)));
            painter->setBrush(QColor(211,211,211));
            painter->drawPath(path);
        }

        //绘制数据位置
        QRectF iconRect = QRect(rect.left()+5, rect.top()+5, 40, 40);
        QRectF singerRect = QRect(iconRect.right()+5, iconRect.top(), rect.width()-10-iconRect.width(), 20);
        QRectF songNbRect = QRect(singerRect.left(), singerRect.bottom()+5, rect.width()-10-iconRect.width(), 20);

        painter->drawImage(iconRect, QImage(ItemData.sHeadPic));
        painter->setPen(QPen(Qt::black));
        painter->setFont(QFont("Microsoft Yahei", 10));
        painter->drawText(singerRect, ItemData.sUserName);

        painter->setPen(QPen(Qt::gray));
        painter->drawText(songNbRect, QString("%1").arg(ItemData.sUserID));

        //画用户状态
        QRectF statusRect = QRect(iconRect.right()+280,rect.top()+20,8,8);
        painter->setRenderHint(QPainter:: Antialiasing, true);  //设置渲染,启动反锯齿
        if(ItemData.isBlocked){
                painter->setPen( QPen(QColor(64,66,68),8));
        }else{
            if(ItemData.sUserStatus == Status::offline || ItemData.sUserStatus == Status::hide){
               painter->setPen( QPen(QColor(221,0,27),8));
            }else if(ItemData.sUserStatus == Status::online){
               painter->setPen( QPen(QColor(44,221,113),8));
            }
        }

        painter->drawEllipse(statusRect);   //半径为50的圆


        painter->restore();


    }
}

QSize UserItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QSize(option.rect.width(), 50);
}
