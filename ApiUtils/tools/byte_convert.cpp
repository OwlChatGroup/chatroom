/*
 * @Project      :
 * @FilePath     : \IM-Network\ApiUtils\tools\byte_convert.cpp
 * @Descripttion :
 * @Author       : GDDG08
 * @Date         : 2022-08-20 15:48:31
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-08-20 19:42:04
 */
#include "byte_convert.h"

QString ByteConvert::byte2HexString(QByteArray byte) {
    return byte2HexString(byte.data(), byte.size());
}

QString ByteConvert::byte2HexString(char byte[], int len) {
    QString strTmp;
    for (int n = 0; n < len; n++)
        strTmp += QString().sprintf("%x ", (unsigned char)byte[n]);
    return strTmp;
}
