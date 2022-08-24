#ifndef OWLCHATMAIN_H
#define OWLCHATMAIN_H

#include <QWidget>
#include <QtGui>
#include <QTime>
#include <QStandardItemModel>
#include <QFile>
#include <expressionpanel.h>
#include <addnewfriends.h>
#include <friendshow.h>
#include <personalmain.h>
#include "ItemInfo.h"
#include "ifaddnewfriendbutton.h"
#include "groupshow.h"
#include <ApiUtils/api_utils.h>

#define EDGE_MARGIN 8

namespace Ui {
class OwlChatMain;
}

class OwlChatMain : public QWidget
{
    Q_OBJECT

public:
    explicit OwlChatMain(QWidget *parent = 0);
    ~OwlChatMain();
    void setDialogWidget(QWidget *w);
    QDateTime       m_lastShowMsgTime;
    QStringList     m_sAcPicList;   //拖拽入得图片列表
    QStringList     m_sAcFileList;  //拖拽入得文件列表
    bool eventFilter(QObject *obj, QEvent *event);
    static ApiUtils* api;
    static uint8_t myID ;
    void setMyID(uint8_t myID);
protected:
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void enterEvent(QEvent * event);
    virtual void leaveEvent(QEvent * event);

    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    void keyReleaseEvent(QKeyEvent *event);
private slots:


    void on_btnClose_clicked();

    void on_btnMin_clicked();

    void on_btnSkin_clicked();

    void on_listView_clicked(const QModelIndex &index);

    void on_btnExpression_clicked();

    void on_btnFolder_clicked(QString fp = NULL);

    void handle_expression();

    void handle_add();

    void handle_delete_dialog();

    void handle_delete_friend();

    void handle_add_to_blacklist();


    void on_btnAddFriends_clicked();

    void on_btnChatList_clicked();

    void on_btnFriendsList_clicked();

    void on_pushButton_2_clicked();

    void on_btnMore_clicked();

    void on_btnHeadIcon_clicked();

    void receive_from_remote(ItemInfo *item);

    void handle_chang_my_status();

    void on_btnSend_clicked();

    void handle_add_new_friend();
    void handle_delete_dialog_group();
    void handle_quit_group();

    void handle_add_member();
    void handle_delete_member();
    void handle_delete_all();

    //网络
    void onLoginCallback(uint8_t s);
    void onRegisterCallback(uint8_t s);
    void sendMessageCallback(uint8_t s, uint32_t msgID);
    void recvMessageCallback(uint32_t fromUserID, uint32_t sessionID, uint64_t time, uint32_t msgID, uint8_t msg_type, QString content);
    void getFriendListCallback(QList<D_UserBasicInfo> infolist);  // Caution list might be zero
    void getUserInfoCallback(D_UserBasicInfo info);
    void getUserDetailCallback(D_UserDetailInfo info);
    void onFriendAddCallback(uint8_t s, uint32_t userID_client);
    void onFriendDeleteCallback(uint8_t s, uint32_t userID_client);
    //@回应好友请求回调
    void onFriendAcceptCallback(uint8_t s, uint32_t userID_client);
    //@收到好友请求回调
    void onFriendRequestCallback(uint32_t fromUserID, QString verify_msg);
    //@收到对方回执回调
    void onFriendResultCallback(uint32_t userID_client, bool isAccepted);
    void onGroupCreateCallback(uint8_t s1, uint32_t s2);
    void onGroupAddCallback(uint8_t s);

private:
    QPoint dragPosition;   //鼠标拖动的位置
    enum {nodir,
          top = 0x01,
          bottom = 0x02,
          left = 0x04,
          right = 0x08,
          topLeft = 0x01 | 0x04,
          topRight = 0x01 | 0x08,
          bottomLeft = 0x02 | 0x04,
          bottomRight = 0x02 | 0x08
         } resizeDir; //更改尺寸的方向

    void changeColor(bool isChatList);

private:
    //    void testEdge(QMouseEvent *event);  //检测鼠标是否接近窗口边缘
    Ui::OwlChatMain *ui;
    QStandardItemModel *m_pModel;//消息列表
    QStandardItemModel *m_pModel2;//好友列表
    QString friendHeadIcon;
    QString myHeadIcon;
    QFile file;//文件对象
    QString fileName;//文件名字
    qint64 fileSize;//文件大小
    qint64 sendSize;//已经发送大小
    ExpressionPanel *ep;
    AddNewFriends *add;
    FriendShow *mo;
    GroupShow *mo_g;
    PersonalMain *pm;
    IfAddNewFriendButton *ifAdd;
    qint64 selectIndex;
    qint64 selectIndex_last;
    bool isChatList;

    UserItemData *myInfo;
    UserItemData *friendInfo;

};

#endif // OWLCHATMAIN_H
