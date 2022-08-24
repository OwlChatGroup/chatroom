/*
 * @Project      :
 * @FilePath     : \IM-Network\ApiUtils\data\data_storage.cpp
 * @Descripttion :
 * @Author       : GDDG08
 * @Date         : 2022-08-22 20:15:38
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-08-23 03:53:13
 */
#include "data_storage.h"

DataStorage::DataStorage(QObject* parent)
    : QObject(parent) {
    qDebug() << QSqlDatabase::drivers();
    creatDb();
    openDb();

    QSqlQuery query;
    query.exec("select * from user");

    while (query.next()) {
        uint32_t userID = query.value("userid").toInt();
        qDebug() << userID;
    }

    // struct UserInfo{
    //     int char
    // }
    // QList<UserInfo>
}

DataStorage::~DataStorage() {
    closeDb();
}

void DataStorage::creatDb() {
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        qDebug() << "QSqlDatabase::"
                 << "Database connected";
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        qDebug() << "QSqlDatabase::"
                 << "Database added";
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("C:/IM/pro.db");
        // mklink /J C:\IM D:\@Projects\Qt\IM-Network
        db.setUserName("pro");
        db.setPassword("pro");
    }
}

bool DataStorage::openDb() {
    if (!db.open()) {
        qDebug() << "QSqlDatabase::"
                 << "Error: Failed to connect database." << db.lastError();
        return false;
    }
    qDebug() << "QSqlDatabase::"
             << "Database opened";
    return true;
}
void DataStorage::closeDb() {
    qDebug() << "QSqlDatabase::"
             << "Database closed";
    db.close();
}
