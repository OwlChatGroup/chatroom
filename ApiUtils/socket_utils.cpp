/*
 * @Project      :
 * @FilePath     : \IM-Network\ApiUtils\socket_utils.cpp
 * @Descripttion :
 * @Author       : GDDG08
 * @Date         : 2022-08-20 11:25:29
 * @LastEditors  : GDDG08
 * @LastEditTime : 2022-08-24 10:54:21
 */
#include "socket_utils.h"

// SocketUtils::SocketUtils(ApiUtils* api) {
//     this->dataHandler = api->resultHandle;
//     SocketUtils();
// }

SocketUtils::SocketUtils(QObject* parent, QString _host, int _host_port)
    : QObject(parent), host(_host), host_port(_host_port) {
    this->client = new QTcpSocket();

    connect(client, SIGNAL(connected()), this, SLOT(connectCallback()));
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnectCallback()));
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorCallback(QAbstractSocket::SocketError)));

    connectServer();
}

SocketUtils::~SocketUtils() {
    this->client->disconnectFromHost();
}

void SocketUtils::connectServer() {
    this->connectServer(this->host, this->host_port);
    // this->connectServer("localhost", 10000);
    // this->connectServer("10.172.175.211", 10000);
    // this->connectServer("192.168.192.92", 10000);
}
void SocketUtils::connectServer(QString hostname, quint16 port) {
    qDebug() << "SocketUtils-->"
             << "connecting...";
    this->isConnected = false;
    this->client->connectToHost(hostname, port);
}

void SocketUtils::connectCallback(void) {
    qDebug() << "SocketUtils-->"
             << "connected";
    connect(client, SIGNAL(readyRead()), this, SLOT(recvCallback()), Qt::QueuedConnection);
    this->isConnected = true;
}

void SocketUtils::disconnectCallback(void) {
    qDebug() << "SocketUtils-->"
             << "disconnected";
    this->isConnected = false;
    this->connectServer();
}

void SocketUtils::errorCallback(QAbstractSocket::SocketError e) {
    qDebug() << "SocketUtils-->"
             << "error, code: " << e;
}

void SocketUtils::recvCallback(void) {
    QByteArray recvArray = client->readAll();
    qDebug() << "SocketUtils-->"
             << "RX_orgin--->" << recvArray.toHex();
    for (int end = 0; end < recvArray.size();) {
        int begin = recvArray.indexOf(pak_header, end);
        end = recvArray.indexOf(pak_header, end + 2);
        if (end == -1)
            end = recvArray.size();
        QByteArray kidArray = recvArray.mid(begin, end - begin);
        qDebug() << "SocketUtils-->"
                 << "RX--->" << kidArray.toHex();
        emit dataReceived(kidArray);
    }
}

int SocketUtils::sendData(QByteArray data) {
    qDebug() << "SocketUtils-->"
             << "TX--->" << data.toHex();
    if (client->isWritable() && this->isConnected) {
        try {
            return client->write(data);
        } catch (std::exception& e) {
            qDebug() << "SocketUtils-->"
                     << "Send error:" << e.what();
            return -1;
        }
    } else {
        qDebug() << "SocketUtils-->"
                 << "not writable";
        return -1;
    }
}
