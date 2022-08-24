/*
 * @Project      :
 * @FilePath     : \IM-Network\ApiUtils\data\data_utils.h
 * @Descripttion :
 * @Author       : GDDG08
 * @Date         : 2022-08-23 18:20:00
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-08-24 20:59:41
 */
#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#include <QObject>
#include <QString>
#include <ApiUtils/data/data_storage.h>
#include <ApiUtils/data/data_struct.h>

#include <QDateTime>
class DataUtils : public QObject {
    Q_OBJECT
   public:
    explicit DataUtils(QObject* parent = 0);

    // init database
    int onLogin(uint32_t userID, QString pwd);
    int addMessage(D_Message msg, int GUID = -1);
    int setMessageID(uint32_t GUID, uint32_t msgID);
    int getMessages(uint32_t sessionID, uint8_t sessionType, QList<D_Message>& list);
    int getRecentMessageList(QList<D_RecentMsgListItem>& list);
    int addFriendRequest(uint32_t fromID, uint32_t toID, QString verify_msg, uint8_t status);
    int getFriendRequestList(QList<D_FriendRequest>& list);
    int changeFriendRequestStatus(uint32_t fromID, uint32_t toID, uint8_t status);
    int addFriend(uint32_t userID);
    int deleteFriend(uint32_t userID);
    int updateUserInfo(D_UserBasicInfo info);
    int getUserInfo(uint32_t userID, D_UserBasicInfo& info);
    int updateUserDetail(D_UserDetailInfo info);
    int getUserDetail(uint32_t userID, D_UserDetailInfo& info);
    int updateFriendList(QList<D_UserBasicInfo> list);
    int getFriendList(QList<D_UserBasicInfo>& list);
    int updateGroupList(QList<D_GroupInfo> list);
    int getGroupList(QList<D_GroupInfo>& list);
    int updateGroupInfo(D_GroupInfo info);
    int getGroupInfo(uint32_t groupID, D_GroupInfo& info);
    int addGroup(uint32_t groupID);
    int deleteGroup(uint32_t groupID);

   private:
    DataStorage* dataStorage;

   signals:

   public slots:
};

#endif  // DATA_UTILS_H