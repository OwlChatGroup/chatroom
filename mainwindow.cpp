 #include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    setWindowFlags(Qt::WindowType::FramelessWindowHint);

    // DEMO
        api = new ApiUtils();
        connect(api, SIGNAL(onLoginCallback(uint8_t)), this, SLOT(test0(uint8_t)));
        connect(api, SIGNAL(onRegisterCallback(uint8_t)), this, SLOT(test1(uint8_t)));
        connect(api, SIGNAL(sendMessageCallback(uint8_t)), this, SLOT(test2(uint8_t)));
        connect(api, SIGNAL(recvMessageCallback(uint32_t , uint32_t , uint64_t , uint8_t , QString )), this, SLOT(test3(uint32_t , uint32_t , uint64_t , uint8_t , QString)));

}

MainWindow::MainWindow(QTcpSocket * sock,QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowType::FramelessWindowHint);
    client = sock;
    connect(client,SIGNAL(readyRead()),this,SLOT(handReadyRead()));

    // DEMO5
//        api = new ApiUtils();
//        connect(api, SIGNAL(onLoginCallback(uint8_t)), this, SLOT(test0(uint8_t)));
//        connect(api, SIGNAL(onRegisterCallback(uint8_t)), this, SLOT(test1(uint8_t)));
//        connect(api, SIGNAL(sendMessageCallback(uint8_t)), this, SLOT(test2(uint8_t)));
//        connect(api, SIGNAL(recvMessageCallback(uint32_t , uint32_t , uint64_t , uint8_t , QString )), this, SLOT(test3(uint32_t , uint32_t , uint64_t , uint8_t , QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_clieked()
{
    //#3|andy|hello&
    QString msg = ui->lineEdit->text();
    QString packData = "#3|andy|" + msg + "&";
    client->write(packData.toLocal8Bit());

}
void MainWindow::handReadyRead(){

    QByteArray recvArray = client->readAll();
    qDebug()<<recvArray;

    if (recvArray.at(0) != '#'|| recvArray.at(recvArray.size()-1)!='&')
        return;
    QString recvStr = QString::fromLocal8Bit(recvArray);
    recvStr = recvStr.mid (1,recvStr.length()-2);
    qDebug()<<recvStr;
    QStringList recvList = recvStr.split( '|');

    if(recvList.size() < 3)
        return;


}
// DEMO
void MainWindow::test0(uint8_t status) {
    qDebug() << "Login--->" << TASK_STATUS_MSG[status];

}
// DEMO
void MainWindow::test1(uint8_t status) {
    qDebug() << "Register--->" << TASK_STATUS_MSG[status];

}
// DEMO
void MainWindow::test2(uint8_t status) {
    qDebug() << "SendMsg--->" << TASK_STATUS_MSG[status];

}
void MainWindow::test3(uint32_t fromUserID, uint32_t sessionID, uint64_t time, uint8_t msg_type ,QString content) {
    qDebug() << "RecvMsg--->" << content;

}

void MainWindow::on_pushButton_2_clicked()
{
     api->onRegister(1234435, "123456&dsw!", "GDDG80", 1u, 20u, 8u, 8u);

}

void MainWindow::on_pushButton_3_clicked()
{
    api->onLogin(1234435, "123456&dsw!");
}

void MainWindow::on_pushButton_4_clicked()
{
    api->sendMessage(1234435, 12344, 0, "abc123!@#");
}
