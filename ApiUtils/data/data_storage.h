/*
 * @Project      :
 * @FilePath     : \IM-Network\ApiUtils\data\data_storage.h
 * @Descripttion :
 * @Author       : GDDG08
 * @Date         : 2022-08-22 20:15:38
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-08-24 21:40:00
 */
#ifndef DATA_STORAGE_H
#define DATA_STORAGE_H

#include <QObject>
#include <QSqlDatabase>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

#include "sql.h"
#include <vector>

typedef std::vector<std::vector<QVariant>> DataResult;
typedef std::vector<QVariant> DataRow;

class DataStorage : public QObject {
    Q_OBJECT
   public:
    explicit DataStorage(QObject* parent = 0);
    ~DataStorage();

    // api
    void getFriendList();
    void getGroupList();
    void getMsg(QString sessionid);
    // int select(DataResult& res, std::string _sql, int resultNum);
    int select(DataResult& res, QString sql, int resultNum);
    int execute(QString sql);
    int execute(QSqlQuery& query, QString sql);

   public:
    QSqlDatabase db;
    void connectDb(uint32_t userID, QString pwd);
    bool openDb();
    void closeDb();
    void createTable();

   signals:

   public slots:
};

#endif  // DATA_STORAGE_H