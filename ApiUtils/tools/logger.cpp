/*
 * @Project      :
 * @FilePath     : \IM-Network\ApiUtils\tools\logger.cpp
 * @Descripttion :
 * @Author       : GDDG08
 * @Date         : 2022-08-20 19:40:51
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-08-23 01:04:35
 */
#include "logger.h"
#include <qlogging.h>
#include <QApplication>
#include <QMutex>
#include <QMutexLocker>
#include <QFile>
#include <QTextStream>
#include <QDebug>

// void Logger::logd(QString log) {
//     if (Logger::isLogging)
//     // QDebug() << log;
// }
// Logger::Logger() {
//     qInstallMessageHandler(myMessageHandle);
// }

//重定向qdebug输出到文件
void myMessageHandle(QtMsgType, const QMessageLogContext&, const QString& msg) {
    static QMutex mut;  //多线程打印时需要加锁
    QMutexLocker locker(&mut);
    QFile file("./log.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream stream(&file);
        stream << msg << endl;
        file.close();
    }
}