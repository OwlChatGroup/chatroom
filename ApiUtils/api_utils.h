/*
 * @Project      :
 * @FilePath     : \IM-Network\ApiUtils\api_utils.h
 * @Descripttion :
 * @Author       : GDDG08
 * @Date         : 2022-08-20 11:48:48
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-08-23 02:59:24
 */
#ifndef API_UTILS_H
#define API_UTILS_H

#include <QObject>
#include <QCryptographicHash>
#include <ApiUtils/packet.h>
#include <ApiUtils/socket_utils.h>
#include <ApiUtils/data/data_storage.h>
#include <ApiUtils/tools/logger.h>
#include <QList>

enum TASK_STATUS {
    SUCCESS ,
    NO_USER_ERROR,
    WRONG_PWD_ERROR,
    USER_EXISTED_ERROR,

};

const QString TASK_STATUS_MSG[]{
    "SUCCESS",
    "NO_USER_ERROR",
    "WRONG_PWD_ERROR",
    "USER_EXISTED_ERROR",
    };

class ApiUtils : public QObject {
    Q_OBJECT

   public:
    explicit ApiUtils(QObject* parent = 0);
    // IMPORTANT: API action, call it when you need
    int onLogin(uint32_t, QString);
    int onRegister(uint32_t _id, QString _pwd, QString _nickname, uint8_t _gender, uint8_t _age, uint8_t _city, uint8_t _job);
    int sendMessage(uint32_t _sessionID, uint8_t _sessionType, uint64_t _time, uint8_t _msg_type, QString _content);

    int getFriendList();
    int getUserInfo(uint32_t _userID);
    int onFriendAdd(uint32_t _userID, QString _verify_msg);
    int onFriendDelete(uint32_t _userID);
    int onFriendAccept(uint32_t _userID, bool _isAccepted);

   private:
    uint32_t getGUID(QString tag);
    SocketUtils* socketUtils;
    DataStorage* dataStorage;
    uint32_t login_ID = 0, login_ID_trial = 0;
    char login_token[17] = {0};
    int onRecvMessage(uint32_t _msgID);
    int onFriendRequest(uint32_t _userID_client);
    int onFriendResult(uint32_t _userID_client);

   private slots:
    void resultHandle(QByteArray);

   signals:
    // IMPORTANT: API CallBack, plz connect during ApiUtil init
    void onLoginCallback(uint8_t);
    void onRegisterCallback(uint8_t);
    void sendMessageCallback(uint8_t, uint32_t msgID);
    void recvMessageCallback(uint32_t fromUserID, uint32_t sessionID, uint64_t time, uint32_t msgID, uint8_t msg_type, QString content);
    void getFriendListCallback(QList<Pak_FriendBasicInfo>);  // Caution list might be zero
    void getUserInfoCallback(Pak_FriendBasicInfo);           // Caution list might be zero
    void onFriendAddCallback(uint8_t, uint32_t userID_client);
    void onFriendDeleteCallback(uint8_t, uint32_t userID_client);
    //@回应好友请求回调
    void onFriendAcceptCallback(uint8_t, uint32_t userID_client);
    //@收到好友请求回调
    void onFriendRequestCallback(uint32_t fromUserID, QString verify_msg);
    //@收到对方回执回调
    void onFriendResultCallback(uint32_t userID_client, bool isAccepted);
};

#endif  // API_UTILS_H
