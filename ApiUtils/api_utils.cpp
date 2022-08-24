/*
 * @Project      :
 * @FilePath     : \IM-Network\ApiUtils\api_utils.cpp
 * @Descripttion :
 * @Author       : GDDG08
 * @Date         : 2022-08-20 11:48:48
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-08-24 21:27:25
 */
#include "api_utils.h"

ApiUtils::ApiUtils(QObject* parent)
    : QObject(parent) {
    dataUtils = new DataUtils(this);

    // Todo: enable network
    socketUtils = new SocketUtils(this, REMOTE_HOST, REMOTE_PORT_SOCKET);
#ifdef HTTP_ENABLE
    httpUtils = new HttpClientHelper(REMOTE_HOST, REMOTE_PORT_HTTP);
#endif
    connect(socketUtils, SIGNAL(dataReceived(QByteArray)), this, SLOT(resultHandle(QByteArray)), Qt::QueuedConnection);
}

int ApiUtils::onLogin(uint32_t _id, QString _pwd) {
    qDebug() << "ApiUtils::"
             << "onLogin";
    uint32_t guid = getGUID("login");

    QString _pwd_md5 = QCryptographicHash::hash(_pwd.toLatin1(), QCryptographicHash::Md5).toHex().right(16);
    Pak_Login* pak = new Pak_Login(_id, _pwd_md5.toLocal8Bit().data());
    pak->GUID = guid;
    QByteArray* data = new QByteArray((char*)pak, sizeof(*pak));

    socketUtils->sendData(*data);
    this->login_ID_trial = _id;
    dataUtils->onLogin(_id, _pwd);
    return 0;
}

int ApiUtils::onRegister(uint32_t _id, QString _pwd, QString _nickname, uint8_t _gender, uint8_t _age, uint8_t _city, uint8_t _job) {
    qDebug() << "ApiUtils::"
             << "onRegister";
    uint32_t guid = getGUID("register");

    QString _pwd_md5 = QCryptographicHash::hash(_pwd.toLatin1(), QCryptographicHash::Md5).toHex().right(16);
    Pak_Register* pak = new Pak_Register(_id, _pwd_md5.toLocal8Bit().data(), _nickname.toLocal8Bit().data(), _gender, _age, _city, _job);
    pak->GUID = guid;
    QByteArray* data = new QByteArray((char*)pak, sizeof(*pak));

    socketUtils->sendData(*data);
    return 0;
}

int ApiUtils::sendMessage(uint32_t _sessionID, uint8_t _sessionType, uint64_t _time, uint8_t _msg_type, QString _content) {
    qDebug() << "ApiUtils::"
             << "sendMessage";
    uint32_t guid = getGUID("msg");
    D_Message msg = {
        this->login_ID,
        _sessionID,
        _sessionType,
        _time,
        0,
        _msg_type,
        _content};
    dataUtils->addMessage(msg, guid);

    uint32_t _msg_len = _content.size() + 1;
    Pak_Message* pak = new Pak_Message(this->login_ID, _sessionID, _sessionType, _time, _msg_type, _msg_len, _content);
    pak->GUID = guid;
    strcpy(pak->token, login_token);
    QByteArray* data = new QByteArray((char*)pak, sizeof(*pak));
    data->chop(sizeof(char*));
    data->append(_content + '\0');

    socketUtils->sendData(*data);
    return 0;
}

int ApiUtils::onRecvMessage(uint32_t _msgID) {
    qDebug() << "ApiUtils::"
             << "onRecvMessage";
    uint32_t guid = getGUID("onRecvMessage");

    Pak_MessageRTN* pak = new Pak_MessageRTN(_msgID);
    pak->GUID = guid;
    strcpy(pak->token, login_token);
    QByteArray* data = new QByteArray((char*)pak, sizeof(*pak));

    socketUtils->sendData(*data);
    return 0;
}

int ApiUtils::getFriendList() {
    qDebug() << "ApiUtils::"
             << "getFriendList";
    uint32_t guid = getGUID("friendlist");

    Pak_Basic* pak = new Pak_Basic(this->login_ID, PACKET_TYPE::FRIEND_LIST);
    pak->GUID = guid;
    strcpy(pak->token, login_token);
    QByteArray* data = new QByteArray((char*)pak, sizeof(*pak));

    socketUtils->sendData(*data);
    return 0;
}

int ApiUtils::getUserInfo(uint32_t _userID) {
    qDebug() << "ApiUtils::"
             << "getUserInfo";
    uint32_t guid = getGUID("getUserInfo");

    Pak_FriendBasic* pak = new Pak_FriendBasic(this->login_ID, _userID, PACKET_TYPE::USER_INFO);
    pak->GUID = guid;
    strcpy(pak->token, login_token);
    QByteArray* data = new QByteArray((char*)pak, sizeof(*pak));

    socketUtils->sendData(*data);
    return 0;
}

int ApiUtils::getUserDetail(uint32_t _userID) {
    qDebug() << "ApiUtils::"
             << "getUserDetail";
    uint32_t guid = getGUID("getUserDetail");

    Pak_FriendBasic* pak = new Pak_FriendBasic(this->login_ID, _userID, PACKET_TYPE::USER_DETAIL);
    pak->GUID = guid;
    strcpy(pak->token, login_token);
    QByteArray* data = new QByteArray((char*)pak, sizeof(*pak));

    socketUtils->sendData(*data);
    return 0;
}

int ApiUtils::onFriendAdd(uint32_t _userID, QString _verify_msg) {
    qDebug() << "ApiUtils::"
             << "onFriendAdd";
    uint32_t guid = getGUID("onFriendAdd");
    dataUtils->addFriendRequest(this->login_ID, _userID, _verify_msg, FRIEND_REQUEST_STATUS::SEND);

    Pak_FriendAdd* pak = new Pak_FriendAdd(this->login_ID, _userID, _verify_msg);
    pak->GUID = guid;
    strcpy(pak->token, login_token);
    QByteArray* data = new QByteArray((char*)pak, sizeof(*pak));

    socketUtils->sendData(*data);
    return 0;
}

int ApiUtils::onFriendDelete(uint32_t _userID) {
    qDebug() << "ApiUtils::"
             << "onFriendDelete";
    uint32_t guid = getGUID("onFriendDelete");

    Pak_FriendBasic* pak = new Pak_FriendBasic(this->login_ID, _userID, PACKET_TYPE::FRIEND_DELETE);
    pak->GUID = guid;
    strcpy(pak->token, login_token);
    QByteArray* data = new QByteArray((char*)pak, sizeof(*pak));

    socketUtils->sendData(*data);
    return 0;
}

int ApiUtils::onFriendAccept(uint32_t _userID, bool _isAccepted) {
    qDebug() << "ApiUtils::"
             << "onFriendAccept";
    uint32_t guid = getGUID("onFriendAccept");

    dataUtils->changeFriendRequestStatus(_userID, this->login_ID, _isAccepted ? FRIEND_REQUEST_STATUS::ACCEPT : REJECT);

    Pak_FriendAccept* pak = new Pak_FriendAccept(this->login_ID, _userID, _isAccepted);
    pak->GUID = guid;
    strcpy(pak->token, login_token);
    QByteArray* data = new QByteArray((char*)pak, sizeof(*pak));

    socketUtils->sendData(*data);
    return 0;
}

int ApiUtils::onFriendRequest(uint32_t _userID_client) {
    qDebug() << "ApiUtils::"
             << "onFriendRequest";
    uint32_t guid = getGUID("onFriendRequest");

    Pak_FriendBasicRTN* pak = new Pak_FriendBasicRTN(this->login_ID, _userID_client, PACKET_TYPE::FRIEND_REQUEST);
    pak->GUID = guid;
    strcpy(pak->token, login_token);
    QByteArray* data = new QByteArray((char*)pak, sizeof(*pak));

    socketUtils->sendData(*data);
    return 0;
}

int ApiUtils::onFriendResult(uint32_t _userID_client) {
    qDebug() << "ApiUtils::"
             << "onFriendResult";
    uint32_t guid = getGUID("onFriendResult");

    Pak_FriendBasicRTN* pak = new Pak_FriendBasicRTN(this->login_ID, _userID_client, PACKET_TYPE::FRIEND_RESULT);
    pak->GUID = guid;
    strcpy(pak->token, login_token);
    QByteArray* data = new QByteArray((char*)pak, sizeof(*pak));

    socketUtils->sendData(*data);
    return 0;
}

int ApiUtils::onGroupCreate(QString _groupName) {
    qDebug() << "ApiUtils::"
             << "onGroupCreate";
    uint32_t guid = getGUID("onGroupCreate");

    Pak_GroupCreate* pak = new Pak_GroupCreate(this->login_ID, _groupName);
    pak->GUID = guid;
    strcpy(pak->token, login_token);
    QByteArray* data = new QByteArray((char*)pak, sizeof(*pak));

    socketUtils->sendData(*data);
    return 0;
}

int ApiUtils::onGroupAdd(uint32_t _groupID, int64_t _userID) {
    qDebug() << "ApiUtils::"
             << "onGroupAdd";
    uint32_t guid = getGUID("onGroupAdd");
    if (_userID == -1)
        _userID = this->login_ID;
    Pak_GroupAdd* pak = new Pak_GroupAdd(this->login_ID, _userID, _groupID);
    pak->GUID = guid;
    strcpy(pak->token, login_token);
    QByteArray* data = new QByteArray((char*)pak, sizeof(*pak));

    socketUtils->sendData(*data);
    return 0;
}
#ifdef HTTP_ENABLE
int ApiUtils::onSendFile(QString filePath) {
    qDebug() << "ApiUtils::"
             << "onSendFile";
    int msgID;
    bool ret = httpUtils->sendFileToServer(msgID, filePath.toStdString());
    qDebug() << "client send test return : " << ret << " with msgid : " << msgID << endl;
    return 0;
}

int ApiUtils::onDownFile(int _msgID, QString _filePath) {
    qDebug() << "ApiUtils::"
             << "onDownFile";
    int msgID;
    std::string filePath = _filePath.toStdString();
    bool ret = httpUtils->askFileFromServer(_msgID, filePath);
    qDebug() << "client send test return : " << ret << " with filePath : " << QString::fromStdString(filePath) << endl;
    return 0;
}
#endif

uint32_t ApiUtils::getGUID(QString tag) {
    return abs(rand());
}

void ApiUtils::resultHandle(QByteArray data) {
    qDebug() << "ApiUtils::"
             << "resultHandle";

    PacketRTN* pak = (PacketRTN*)data.data();
    switch (pak->type) {
        case PACKET_TYPE::LOGIN: {
            Pak_LoginRTN* rtn = (Pak_LoginRTN*)data.data();
            qDebug() << "LOGIN-->"
                     << "msg:" << pak->msg << ", token:" << rtn->token;
            if (pak->msg == TASK_STATUS::SUCCESS) {
                this->login_ID = this->login_ID_trial;
                strcpy(login_token, rtn->token);
            }
            emit onLoginCallback(pak->msg);
        } break;

        case PACKET_TYPE::REGISTER: {
            qDebug() << "REGISTER-->"
                     << "msg:" << pak->msg;
            emit onRegisterCallback(pak->msg);
        } break;

        case PACKET_TYPE::SEND_MESSAGE: {
            Pak_MessageRTN* rtn = (Pak_MessageRTN*)data.data();
            qDebug() << "SEND_MESSAGE-->"
                     << "msg:" << pak->msg << ", msgID:" << rtn->msgID;

            dataUtils->setMessageID(rtn->GUID, rtn->msgID);

            emit sendMessageCallback(pak->msg, rtn->msgID);
        } break;

        case PACKET_TYPE::RECV_MESSAGE: {
            Pak_MessageRX* rtn = (Pak_MessageRX*)data.data();
            char* buff = (char*)&rtn->content;
            QByteArray byteArray(buff, rtn->msg_len);
            QString content(buff);
            qDebug() << "RECV_MESSAGE-->"
                     << "fromUser:" << rtn->userID << ", toSession:" << rtn->sessionID << ", msgID:" << rtn->msgID << ", type:" << rtn->msg_type << ", msg:" << content;

            D_Message msg = {rtn->userID, rtn->sessionID, rtn->sessionType, rtn->time, rtn->msgID, rtn->msg_type, content};
            dataUtils->addMessage(msg);

            emit recvMessageCallback(rtn->userID, rtn->sessionID, rtn->time, rtn->msgID, rtn->msg_type, content);
            onRecvMessage(rtn->msgID);
        } break;

        case PACKET_TYPE::FRIEND_LIST: {
            Pak_BasicArrayRTN* rtn = (Pak_BasicArrayRTN*)data.data();
            QList<D_UserBasicInfo> friend_list;
            if (rtn->list_len == 0) {
                qDebug() << "FRIEND_LIST-->"
                         << "no friend";
                emit getFriendListCallback(friend_list);
            } else {
                // int size = sizeof(D_UserBasicInfo) * rtn->list_len;
                // D_UserBasicInfo* friend_list = (D_UserBasicInfo*)malloc(size);
                // memcpy(friend_list, &rtn->start_ptr, size);
                Pak_UserBasicInfo* ptr = (Pak_UserBasicInfo*)&rtn->start_ptr;
                for (int i = 0; i < rtn->list_len; i++) {
                    D_UserBasicInfo info = {ptr->userID, ptr->avatarID, QString(ptr->nickName), ptr->userStatus};
                    friend_list.append(info);
                }
                qDebug() << "FRIEND_LIST-->"
                         << "friendNum:" << rtn->list_len << "frist Friend:" << friend_list.at(0).userID;

                dataUtils->updateFriendList(friend_list);

                emit getFriendListCallback(friend_list);
            }
        } break;
        case PACKET_TYPE::USER_INFO: {
            Pak_FriendBasicInfoRTN* rtn = (Pak_FriendBasicInfoRTN*)data.data();
            if (rtn->msg == TASK_STATUS::SUCCESS) {
                D_UserBasicInfo info = {rtn->userID, rtn->avatarID, QString(rtn->nickName), rtn->userStatus};
                qDebug() << "USER_INFO-->"
                         << "msg:" << TASK_STATUS_MSG[rtn->msg] << ", userID:" << info.userID << ", nickname:" << QString(info.nickName) << ", avatarID:" << info.avatarID;

                dataUtils->updateUserInfo(info);

                emit getUserInfoCallback(info);
            } else {
                qDebug() << "USER_INFO-->"
                         << "error: " << TASK_STATUS_MSG[rtn->msg];
                //  emit getUserInfoCallback(NULL);
            }
        } break;
        case PACKET_TYPE::USER_DETAIL: {
            Pak_FriendDetailInfoRTN* rtn = (Pak_FriendDetailInfoRTN*)data.data();
            if (rtn->msg == TASK_STATUS::SUCCESS) {
                D_UserDetailInfo info = {rtn->userID, QString(rtn->nickName), rtn->gender, rtn->age, rtn->city, rtn->job, rtn->avatarID, 8, QString(rtn->signature), rtn->userStatus};
                qDebug() << "USER_DETAIL-->"
                         << "msg:" << TASK_STATUS_MSG[rtn->msg] << ", userID:" << info.userID;

                dataUtils->updateUserDetail(info);

                emit getUserDetailCallback(info);
            } else {
                qDebug() << "USER_DETAIL-->"
                         << "error: " << TASK_STATUS_MSG[rtn->msg];
                //  emit getUserInfoCallback(NULL);
            }
        } break;
        case PACKET_TYPE::FRIEND_ADD: {
            Pak_FriendBasicRTN* rtn = (Pak_FriendBasicRTN*)data.data();
            qDebug() << "FRIEND_ADD-->"
                     << "msg:" << pak->msg << ", userID_client:" << rtn->userID_client;

            dataUtils->changeFriendRequestStatus(rtn->userID, rtn->userID_client, FRIEND_REQUEST_STATUS::ARRIVE);

            emit onFriendAddCallback(pak->msg, rtn->userID_client);
        } break;
        case PACKET_TYPE::FRIEND_DELETE: {
            Pak_FriendBasicRTN* rtn = (Pak_FriendBasicRTN*)data.data();
            qDebug() << "FRIEND_DELETE-->"
                     << "msg:" << pak->msg << ", userID_client:" << rtn->userID_client;

            dataUtils->deleteFriend(rtn->userID_client);

            emit onFriendDeleteCallback(pak->msg, rtn->userID_client);
        } break;
        case PACKET_TYPE::FRIEND_ACCEPT: {
            Pak_FriendBasicRTN* rtn = (Pak_FriendBasicRTN*)data.data();
            qDebug() << "FRIEND_ACCEPT-->"
                     << "msg:" << pak->msg << ", userID_client:" << rtn->userID_client;
            emit onFriendAcceptCallback(pak->msg, rtn->userID_client);
        } break;
        case PACKET_TYPE::FRIEND_REQUEST: {
            Pak_FriendAdd* rtn = (Pak_FriendAdd*)data.data();

            QString verify_msg = QString(rtn->verify_msg);
            qDebug() << "FRIEND_REQUEST-->"
                     << "fromUserID:" << rtn->userID << ", verify_msg:" << verify_msg;

            dataUtils->addFriendRequest(rtn->userID, rtn->userID_client, verify_msg, FRIEND_REQUEST_STATUS::ARRIVE);

            emit onFriendRequestCallback(rtn->userID, verify_msg);
            onFriendRequest(rtn->userID);
        } break;
        case PACKET_TYPE::FRIEND_RESULT: {
            Pak_FriendAccept* rtn = (Pak_FriendAccept*)data.data();

            qDebug() << "FRIEND_RESULT-->"
                     << "userID_client:" << rtn->userID << ", isAccepted:" << rtn->isAccepted;

            dataUtils->changeFriendRequestStatus(rtn->userID, rtn->userID_client, rtn->isAccepted ? FRIEND_REQUEST_STATUS::ACCEPT : REJECT);

            emit onFriendResultCallback(rtn->userID, rtn->isAccepted);
            onFriendResult(rtn->userID);
        } break;
        case PACKET_TYPE::GROUP_CREATE: {
            Pak_GroupCreateRTN* rtn = (Pak_GroupCreateRTN*)data.data();

            qDebug() << "GROUP_CREATE-->"
                     << "msg:" << pak->msg << "groupID:" << rtn->groupID;
            emit onGroupCreateCallback(pak->msg, rtn->groupID);
        } break;
        case PACKET_TYPE::GROUP_ADD: {
            qDebug() << "GROUP_ADD-->"
                     << "msg:" << pak->msg;
            emit onGroupAddCallback(pak->msg);
        } break;
    }
}
