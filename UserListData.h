#ifndef USERITEMDATA_H
#define USERITEMDATA_H


#include<sstream>
enum class Status{
    online = 0,
    hide=1,
    offline=2,
    block = 3
};

enum class MsgTpye{
    Group,
    Single,
    AddMsg
};

typedef struct UserItemData
{
    QString sHeadPic;
    QString sUserName;
    QString sUserSignature;
    uint32_t sUserID;
    QString sUserPwd;
    uint8_t sUserGender;
    uint8_t sUserAge;
    uint8_t sUserCity;
    uint8_t sUserJob;
    //    QString
    Status sUserStatus;
    MsgTpye sMsgTpye;
    bool isBlocked;

    UserItemData() {
        sHeadPic = "";
        sUserName = "梦里想你";
        sUserSignature = "你好";
        sUserID = 220823;
        sUserPwd = 123;
        sUserAge = 18;
        sUserGender = 1;
        sUserCity = 1;
        sUserJob = 1;
        sUserStatus = Status::online;
        sMsgTpye = MsgTpye::Single;
        isBlocked = false;
    }
};



Q_DECLARE_METATYPE(UserItemData);

#endif // USERITEMDATA_H
