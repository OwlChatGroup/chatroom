#include "addnewfriends.h"
#include "ui_addnewfriends.h"

AddNewFriends::AddNewFriends(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewFriends)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}

AddNewFriends::~AddNewFriends()
{
    delete ui;
}


void AddNewFriends::on_btnOK_clicked()
{
    friendInfo.sUserName = ui->nameLine->text();
    friendInfo.sUserID = ui->idLine->text().toInt();
    switch (ui->statusLine->text().toInt()) {
    case 0:
        friendInfo.sUserStatus = Status::online;
        break;
    case 1:
        friendInfo.sUserStatus = Status::hide;
        break;
    case 2:
        friendInfo.sUserStatus = Status::offline;
        break;
    default:
        break;
    }

    switch (ui->msgLine->text().toInt()) {
    case 0:
    friendInfo.sMsgTpye=MsgTpye::Group;
        break;
    case 1:
    friendInfo.sMsgTpye=MsgTpye::Single;
        break;
    case 2:
    friendInfo.sMsgTpye=MsgTpye::AddMsg;
        break;
    default:
        break;
    }
    switch (ui->blockLine->text().toInt()) {
    case 0:
        friendInfo.isBlocked = false;
        break;
    case 1:
        friendInfo.isBlocked = true;
        break;
    default:
        break;
    }
    emit onAddClicked();
    hide();
}

void AddNewFriends::on_cancel_clicked()
{
    hide();
}

void AddNewFriends::load_friend_info(UserItemData info)
{
    ui->nameLine->setText(info.sUserName);
    ui->idLine->setText(QString("%1").arg(info.sUserID));
    ui->blockLine->setText("1");
}
