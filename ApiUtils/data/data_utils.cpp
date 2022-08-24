/*
 * @Project      :
 * @FilePath     : \IM-Network\ApiUtils\data\data_utils.cpp
 * @Descripttion :
 * @Author       : GDDG08
 * @Date         : 2022-08-23 18:20:00
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-08-24 21:25:19
 */
#include "data_utils.h"

DataUtils::DataUtils(QObject* parent)
    : QObject(parent) {
    dataStorage = new DataStorage(this);
}

// init database
int DataUtils::onLogin(uint32_t userID, QString pwd) {
    qDebug() << "DataUtils"
             << "onLogin";
    dataStorage->connectDb(userID, pwd);
    dataStorage->openDb();
    dataStorage->createTable();
}

int DataUtils::addMessage(D_Message msg, int GUID) {
    qDebug() << "DataUtils"
             << "addMessage";

    QString sql = QString(
                      "INSERT INTO msg(msgid, fromuserid, sessionid, sessiontype, msgtype, content, guid)"
                      "VALUES(%1, '%2', '%3', %4, %5, %6, %7)")
                      .arg(QString(msg.msgID), QString(msg.fromID), QString(msg.sessionType == 0 ? msg.fromID : msg.sessionID), QString(msg.sessionType), QString(msg.msg_type), QString(msg.content), QString(GUID));
    return dataStorage->execute(sql);
}

int DataUtils::setMessageID(uint32_t GUID, uint32_t msgID) {
    qDebug() << "DataUtils"
             << "setMessageID";

    QString sql = QString("UPDATE msg SET msgid = %1,guid = -1 WHERE guid = %2")
                      .arg(QString(msgID), QString(GUID));
    return dataStorage->execute(sql);
}

int DataUtils::getMessages(uint32_t sessionID, uint8_t sessionType, QList<D_Message>& list) {
    qDebug() << "DataUtils"
             << "getMessages";

    DataResult res;
    QString sql = QString("SELECT * FROM msg WHERE sessionid = '%1' and sessionType = %2").arg(QString(sessionID), QString(sessionType));

    if (dataStorage->select(res, sql, 7) == 0) {
        for (int i = 0; i < res.size(); i++) {
            DataRow row = res[i];
            D_Message info;
            for (int j = 0; j < row.size(); j++) {
                info.msgID = row[0].toUInt();
                info.fromID = row[1].toUInt();
                info.sessionID = row[2].toUInt();
                info.sessionType = row[3].toUInt();
                info.msg_type = row[4].toUInt();
                info.time = row[5].toULongLong();
                info.content = row[6].toString();
            }
            list.push_back(info);
        }
        return 0;
    } else
        return 1;
}
// need some chage!!
int DataUtils::getRecentMessageList(QList<D_RecentMsgListItem>& list) {
    qDebug() << "DataUtils"
             << "getRecentMessageList";

    int error_count = 0;
    QSqlQuery query;
    QString sql = QString("SELECT sessionid, sessiontype from msg GROUP BY sessionid");
    error_count += dataStorage->execute(query, sql);

    while (query.next()) {
        QString sessionid = query.value(0).toString();
        int sessiontype = query.value(1).toInt();
        QSqlQuery query2;
        QString sql2 = sessiontype == 0 ? QString("select avatar, nickname, lastreadtime, lastreadmsg from user where userid = '%1'").arg(QString(sessionid)) : QString("select avtar, grouponame, lastreadtime, lastreadmsg from gp where groupid = %1").arg(QString(sessionid));
        error_count += dataStorage->execute(query2, sql2);

        QString avatar, nickname, lastreadtime, lastreadmsg;
        if (query2.next()) {
            avatar = query2.value(0).toString();
            nickname = query2.value(1).toString();
            lastreadtime = query2.value(2).toString();
            lastreadmsg = query2.value(3).toString();
        }

        QSqlQuery query3;
        QString sql3 = QString("select count(*) from msg where lastreadtime > '%1'").arg(lastreadtime);
        error_count += dataStorage->execute(query3, sql3);
        int noreadnum = 0;
        if (query3.next()) {
            noreadnum = query3.value(0).toInt();
        }

        D_RecentMsgListItem drml;
        drml.sessionID = sessionid.toInt();
        drml.sessionType = sessiontype;
        drml.unread_num = noreadnum;
        drml.sessionName = nickname;
        drml.last_msg = lastreadmsg;
        list.append(drml);
    }
    return error_count;
}

// FRIEND
int DataUtils::addFriendRequest(uint32_t fromID, uint32_t toID, QString verify_msg, uint8_t status) {
    qDebug() << "DataUtils"
             << "addFriendRequest";

    QString sql = QString("INSERT INTO fr (fromuserid,touserid,content,status) VALUES('%1','%2','%3',%4)")
                      .arg(QString(fromID), QString(toID), verify_msg, QString(status));
    return dataStorage->execute(sql);
}

int DataUtils::changeFriendRequestStatus(uint32_t fromID, uint32_t toID, uint8_t status) {
    qDebug() << "DataUtils"
             << "changeFriendRequestStatus";

    QString sql = QString("UPDATE fr SET status = %1 WHERE fromuserid = %2 AND touserid = %3")
                      .arg(QString(status), QString(fromID), QString(toID));
    return dataStorage->execute(sql);
}

int DataUtils::getFriendRequestList(QList<D_FriendRequest>& list) {
    qDebug() << "DataUtils"
             << "getFriendRequestList";

    DataResult res;
    if (dataStorage->select(res, "SELECT * FROM fr", 5) == 0) {
        for (int i = 0; i < res.size(); i++) {
            DataRow row = res[i];
            D_FriendRequest info;
            for (int j = 0; j < row.size(); j++) {
                info.fromID = row[0].toUInt();
                info.toID = row[1].toUInt();
                info.verify_msg = row[2].toString();
                info.status = row[3].toUInt();
                info.timeStamp = row[4].toULongLong();
            }
            list.push_back(info);
        }
        return 0;
    } else
        return 1;
}

int DataUtils::addFriend(uint32_t userID) {
    qDebug() << "DataUtils"
             << "addFriend";
    // 可能已经有了但还不是friend
    QString sql = QString("INSERT INTO user (userid, isfriend) VALUES('%1',TRUE)")
                      .arg(QString(userID));
    return dataStorage->execute(sql);
}

int DataUtils::deleteFriend(uint32_t userID) {
    qDebug() << "DataUtils"
             << "deleteFriend";
    // ����ȡ����ʶ
    QString sql = QString("UPDATE user SET isfriend = 0 WHERE userid = '%1'")
                      .arg(QString(userID));
    return dataStorage->execute(sql);
}

// update 的时候可能已经有�?
int DataUtils::updateUserInfo(D_UserBasicInfo info) {
    qDebug() << "DataUtils"
             << "updateUserInfo";
}

int DataUtils::getUserInfo(uint32_t userID, D_UserBasicInfo& info) {
    qDebug() << "DataUtils"
             << "getUserInfo";
}

int DataUtils::updateUserDetail(D_UserDetailInfo info) {
    qDebug() << "DataUtils"
             << "updateUserDetail";
}

int DataUtils::getUserDetail(uint32_t userID, D_UserDetailInfo& info) {
    qDebug() << "DataUtils"
             << "getUserDetail";

    DataResult res;
    if (dataStorage->select(res, "SELECT * FROM user", 10) == 0) {
        DataRow row = res[0];
        info.userID = row[0].toUInt();
        info.nickName = row[1].toString();
        info.gender = row[2].toUInt();
        info.age = row[3].toUInt();
        info.city = row[4].toUInt();
        info.job = row[5].toUInt();
        info.avatarID = row[6].toUInt();
        info.isFriend = row[7].toUInt();
        info.signature = row[8].toString();
        info.userStatus = row[9].toUInt();
        return 0;
    } else
        return 1;
}

int DataUtils::updateFriendList(QList<D_UserBasicInfo> list) {
    qDebug() << "DataUtils"
             << "updateFriendList";
}

int DataUtils::getFriendList(QList<D_UserBasicInfo>& list) {
    qDebug() << "DataUtils"
             << "getFriendList";

    DataResult res;
    if (dataStorage->select(res, "SELECT (userid,nickname,avatar,status) FROM user WHERE isfriend = TRUE", 4) == 0) {
        for (int i = 0; i < res.size(); i++) {
            DataRow row = res[i];
            D_UserBasicInfo info;
            for (int j = 0; j < row.size(); j++) {
                info.userID = row[0].toUInt();
                info.nickName = row[1].toString();
                info.avatarID = row[2].toUInt();
                info.userStatus = row[3].toUInt();
            }
            list.push_back(info);
        }
        return 0;
    } else
        return 1;
}

// GROUP
int DataUtils::updateGroupList(QList<D_GroupInfo> list) {
    qDebug() << "DataUtils"
             << "updateGroupList";
}

int DataUtils::getGroupList(QList<D_GroupInfo>& list) {
    qDebug() << "DataUtils"
             << "getGroupList";
    DataResult res;
    if (dataStorage->select(res, "SELECT * FROM gp", 5) == 0) {
        for (int i = 0; i < res.size(); i++) {
            DataRow row = res[i];
            D_GroupInfo info;
            for (int j = 0; j < row.size(); j++) {
                info.groupID = row[0].toUInt();
                info.groupName = row[1].toString();
                info.adminUser = row[2].toUInt();
                info.avatarID = row[3].toUInt();
                info.board = row[4].toString();
            }
            list.push_back(info);
        }
        return 0;
    } else
        return 1;
}

int DataUtils::updateGroupInfo(D_GroupInfo info) {
    qDebug() << "DataUtils"
             << "updateGroupInfo";
}

int DataUtils::getGroupInfo(uint32_t groupID, D_GroupInfo& info) {
    qDebug() << "DataUtils"
             << "getGroupInfo";

    QString sql = QString("SELECT * FROM gp WHERE groupid = '%1'").arg(QString(groupID));

    DataResult res;
    if (dataStorage->select(res, sql, 5) == 0) {
        DataRow row = res[0];
        info.groupID = row[0].toUInt();
        info.groupName = row[1].toString();
        info.adminUser = row[2].toUInt();
        info.avatarID = row[3].toUInt();
        info.board = row[4].toString();
        return 0;
    } else
        return 1;
}

int DataUtils::addGroup(uint32_t groupID) {
    qDebug() << "DataUtils"
             << "addGroup";

    QString sql = QString("INSERT INTO gp (groupid) VALUES(%1)")
                      .arg(QString(groupID));
    return dataStorage->execute(sql);
}

int DataUtils::deleteGroup(uint32_t groupID) {
    qDebug() << "DataUtils"
             << "deleteGroup";

    QString sql = QString("DELETE FROM gp WHERE groupid = %1")
                      .arg(QString(groupID));
    return dataStorage->execute(sql);
}
