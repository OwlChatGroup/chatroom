/*
 * @Project      :
 * @FilePath     : \IM-Network\ApiUtils\tools\logger.h
 * @Descripttion :
 * @Author       : GDDG08
 * @Date         : 2022-08-20 19:40:51
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-08-23 01:02:50
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <QString>

// class Logger {
//    public:
// void static logd(QString);
// bool static isLogging;
void myMessageHandle(QtMsgType, const QMessageLogContext&, const QString& msg);
// };

#endif  // LOGGER_H
