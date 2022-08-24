/*
 * @Project      :
 * @FilePath     : \IM-Network\ApiUtils\data\data_storage.cpp
 * @Descripttion :
 * @Author       : GDDG08
 * @Date         : 2022-08-22 20:15:38
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-08-24 21:41:15
 */
#include "data_storage.h"

DataStorage::DataStorage(QObject* parent)
    : QObject(parent) {
    qDebug() << QSqlDatabase::drivers();

    // Todo Remove
    connectDb(111, "123456");
    openDb();
    createTable();
    // QSqlQuery query;
    // query.exec("select * from test");

    // while (query.next()) {
    //     QString userID = query.value("username").toString();
    //     qDebug() << userID;
    // }

    // struct UserInfo{
    //     int char
    // }
    // QList<UserInfo>
    // DataResult r;
    // select(r, "select * from user", 10);
    // qDebug() << "select";
}

DataStorage::~DataStorage() {
    closeDb();
}

void DataStorage::connectDb(uint32_t userID, QString pwd) {
    QString user = QString(userID);

    if (QSqlDatabase::contains("user" + user)) {
        qDebug() << "QSqlDatabase::"
                 << "Database connected";
        db = QSqlDatabase::database("user" + user);
    } else {
        qDebug() << "QSqlDatabase::"
                 << "Database added";
        db = QSqlDatabase::addDatabase("QSQLITE", "user" + user);
        // system("IF NOT EXIST \"C:/OwlChat\" MD  \"C:/OwlChat\" ");
        db.setDatabaseName("C:/IM/123.db");
        // mklink /J C:\IM D:\@Projects\Qt\IM-Network
        db.setUserName(user);
        db.setPassword(pwd);
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

void DataStorage::createTable() {
    execute(SQLCREATE_FRIEND);
    execute(SQLCREATE_GP);
    execute(SQLCREATE_MSG);
    execute(SQLCREATE_FR);
    execute(SQLCREATE_TRIGGER_MSG);
    execute(SQLCREATE_TRIGGER_GROUP);
    qDebug() << "DataStorage"
             << "create table Done";
}

// int DataStorage::select(DataResult& res, std::string _sql, int resultNum) {
//     QString sql = QString::fromStdString(_sql);
//     select(res, sql, resultNum);
// }
int DataStorage::select(DataResult& res, QString sql, int resultNum) {
    QSqlQuery query;
    query.setForwardOnly(true);
    execute(query, sql);

    if (!query.isActive()) {
        return 1;
    } else {
        while (query.next()) {
            std::vector<QVariant> resRow;
            for (int i = 0; i < resultNum; i++) {
                resRow.push_back(query.value(i));
            }
            res.push_back(resRow);
        }
        return 0;
    }
}

int DataStorage::execute(QString sql) {
    QSqlQuery query;
    return execute(query, sql);
}

int DataStorage::execute(QSqlQuery& query, QString sql) {
    bool status = query.exec(sql);

    if (status) {
        return 0;
    } else {
        QSqlError err = query.lastError();
        qDebug() << "DataStorage::excute Error:" << err.databaseText() << "; " << err.driverText();
        return 1;
    }
}
