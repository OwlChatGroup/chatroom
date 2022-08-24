/*
 * @Project      :
 * @FilePath     : \IM-Network\ApiUtils\packet.cpp
 * @Descripttion :
 * @Author       : GDDG08
 * @Date         : 2022-08-20 10:52:10
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-08-24 16:01:37
 */
#include "packet.h"

Pak_Login::Pak_Login(uint32_t _id, char _pwd[])
    : ID(_id) {
    strcpy(this->PWD, _pwd);
    this->len = PACKET_SIZE(this);
    this->type = PACKET_TYPE::LOGIN;
};

Pak_Register::Pak_Register(uint32_t _id, char _pwd[], char _nickname[], uint8_t _gender, uint8_t _age, uint8_t _city, uint8_t _job)
    : ID(_id), gender(_gender), age(_age), city(_city), job(_job) {
    strcpy(this->PWD, _pwd);
    strcpy(this->nickname, _nickname);
    this->len = PACKET_SIZE(this);
    this->type = PACKET_TYPE::REGISTER;
};

Pak_Message::Pak_Message(uint32_t _userID, uint32_t _sessionID, uint8_t _sessionType, uint64_t _time, uint8_t _msg_type, uint32_t _msg_len, QString _content)
    : userID(_userID), sessionID(_sessionID), sessionType(_sessionType), time(_time), msg_type(_msg_type), msg_len(_msg_len) {
    this->content = (char*)malloc(sizeof(char) * _msg_len);
    strcpy(this->content, _content.toLocal8Bit().data());
    this->len = PACKET_SIZE(this) - sizeof(char*) + sizeof(char) * _msg_len;
    this->type = PACKET_TYPE::SEND_MESSAGE;
};

Pak_Message::~Pak_Message() {
    free(this->content);
};

Pak_MessageRTN::Pak_MessageRTN(uint32_t _msgID)
    : msgID(_msgID) {
    this->len = PACKET_SIZE(this);
    this->type = PACKET_TYPE::RECV_MESSAGE;
}

Pak_Basic::Pak_Basic(uint32_t _userID, PACKET_TYPE pak_type)
    : userID(_userID) {
    this->len = PACKET_SIZE(this);
    this->type = pak_type;
}

Pak_FriendBasic::Pak_FriendBasic(uint32_t _userID_my, uint32_t _userID_client, PACKET_TYPE pak_type)
    : Pak_Basic(_userID_my, pak_type), userID_client(_userID_client) {
    this->len = PACKET_SIZE(this);
}

Pak_FriendAdd::Pak_FriendAdd(uint32_t _userID_my, uint32_t _userID_client, QString _verify_msg)
    : Pak_FriendBasic(_userID_my, _userID_client, PACKET_TYPE::FRIEND_ADD) {
    memcpy(this->verify_msg, _verify_msg.toLocal8Bit().data(), sizeof(this->verify_msg));
    this->len = PACKET_SIZE(this);
}

Pak_FriendBasicRTN::Pak_FriendBasicRTN(uint32_t _userID, uint32_t _userID_client, PACKET_TYPE pak_type)
    : userID(_userID), userID_client(_userID_client) {
    this->len = PACKET_SIZE(this);
    this->type = pak_type;
}
Pak_FriendAccept::Pak_FriendAccept(uint32_t _userID_my, uint32_t _userID_client, uint8_t _isAccepted)
    : Pak_FriendBasic(_userID_my, _userID_client, PACKET_TYPE::FRIEND_ACCEPT), isAccepted(_isAccepted) {
    this->len = PACKET_SIZE(this);
}

Pak_GroupCreate::Pak_GroupCreate(uint32_t _userID, QString _groupName)
    : Pak_Basic(_userID, PACKET_TYPE::GROUP_CREATE) {
    memcpy(this->groupName, _groupName.toLocal8Bit().data(), sizeof(this->groupName));
    this->len = PACKET_SIZE(this);
}

Pak_GroupAdd::Pak_GroupAdd(uint32_t _userID,uint32_t _clientID, uint32_t _groupID)
    : Pak_Basic(_userID, PACKET_TYPE::GROUP_ADD) ,clientID(_clientID), groupID(_groupID) {
    this->len = PACKET_SIZE(this);
}
