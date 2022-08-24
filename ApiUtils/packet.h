/*
 * @Project      :
 * @FilePath     : \IM-Network\ApiUtils\packet.h
 * @Descripttion :
 * @Author       : GDDG08
 * @Date         : 2022-08-20 10:52:10
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-08-23 02:09:18
 */
#ifndef PACKAGE_H
#define PACKAGE_H

#define PACKET_SIZE(x) sizeof(*(x)) - sizeof(Packet)

#include <cstring>
#include <stdint.h>
#include "socket_utils.h"

#pragma pack(1)

enum PACKET_TYPE {
    LOGIN = 1u,
    REGISTER,
    SEND_MESSAGE,
    RECV_MESSAGE,
    FRIEND_LIST,
    FRIEND_ADD,
    FRIEND_DELETE,
    FRIEND_ACCEPT,
    FRIEND_REQUEST,
    FRIEND_RESULT,
    USER_INFO
};
class Packet {
   public:
    uint8_t PACKET_HEADR_0 = SOCKET_PACKET_HEADR_0;
    uint8_t PACKET_HEADR_1 = SOCKET_PACKET_HEADR_1;
    uint8_t type = 0;
    uint16_t len = 0;
    uint32_t GUID = 0;
    char token[17] = {0};
};

class PacketRTN : public Packet {
   public:
    uint8_t msg;
};

/*======== LOGIN ========*/
class Pak_Login : public Packet {
   private:
    uint32_t ID;
    char PWD[17];

   public:
    Pak_Login(uint32_t _id, char _pwd[]);
};

class Pak_LoginRTN : public PacketRTN {
   public:
    char token[17];
};

/*======== REGISTER ========*/
class Pak_Register : public Packet {
   private:
    uint32_t ID;
    char PWD[17];
    char nickname[128];

    uint8_t gender;
    uint8_t age;

    uint8_t city;
    uint8_t job;

   public:
    Pak_Register(uint32_t _id, char _pwd[], char _nickname[], uint8_t _gender, uint8_t _age, uint8_t _city, uint8_t _job);
};

/*======== MESSAGE ========*/
class Pak_Message : public Packet {
   public:
    uint32_t userID;
    uint32_t sessionID;
    uint8_t sessionType;
    uint64_t time;
    uint8_t msg_type;
    uint32_t msg_len;
    char* content;

    Pak_Message(uint32_t _userID, uint32_t _sessionID, uint8_t _sessionType, uint64_t _time, uint8_t _msg_type, uint32_t _msg_len, QString _content);
    ~Pak_Message();
};

class Pak_MessageRTN : public PacketRTN {
   public:
    uint32_t msgID;
    Pak_MessageRTN(uint32_t _msgID);
};

class Pak_MessageRX : public Packet {
   public:
    uint32_t userID;
    uint32_t sessionID;
    uint8_t sessionType;
    uint64_t time;
    uint8_t msg_type;
    uint32_t msgID;
    uint32_t msg_len;
    char* content;
};

/*======== BASIC ========*/

class Pak_Basic : public Packet {
   public:
    uint32_t userID;

    Pak_Basic(uint32_t _userID, PACKET_TYPE pak_type);
};

class Pak_BasicArrayRTN : public PacketRTN {
   public:
    uint16_t list_len;
    char* start_ptr;
};

/*======== FRIEND ========*/

class Pak_FriendBasicInfo {
   public:
    uint32_t userID;
    char nickName[128];
    uint32_t avatarID;
};
class Pak_FriendBasicRTN : public PacketRTN {
   public:
    uint32_t userID;
    uint32_t userID_client;

    Pak_FriendBasicRTN(uint32_t _userID, uint32_t _userID_client, PACKET_TYPE type);
};

// FRIEND_DELETE
class Pak_FriendBasic : public Pak_Basic {
   public:
    uint32_t userID_client;
    Pak_FriendBasic(uint32_t _userID_my, uint32_t _userID_client, PACKET_TYPE pak_type);
};

// FRIEND_ADD
class Pak_FriendAdd : public Pak_FriendBasic {
   public:
    char verify_msg[256];
    Pak_FriendAdd(uint32_t _userID_my, uint32_t _userID_client, QString _verify_msg);
};

// FRIEND REQUEST ACTION

class Pak_FriendAccept : public Pak_FriendBasic {
   public:
    uint8_t isAccepted;
    Pak_FriendAccept(uint32_t _userID_my, uint32_t _userID_client, uint8_t _isAccepted);
};
#endif  // PACKAGE_H
