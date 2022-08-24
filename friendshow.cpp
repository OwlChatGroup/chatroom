#include "friendshow.h"
#include "ui_friendshow.h"

FriendShow::FriendShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FriendShow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}

FriendShow::~FriendShow()
{
    delete ui;
}



void FriendShow::change_dialog_type(bool isChatList)
{
    ui->btnDeleteDialog->setVisible(isChatList);
}

void FriendShow::on_btnDeleteDialog_clicked()
{
    emit delete_dialog();
}

void FriendShow::load_friend_info(UserItemData *person)
{
    ui->nameEdit->setText(person->sUserName);
    ui->signatureEdit->setText(person->sUserSignature);
    ui->accountEdit->setText(QString::number(person->sUserID,10));
    ui->ageEdit->setText(QString::number(person->sUserAge));
    ui->cityEdit->setCurrentIndex(person->sUserCity);
    ui->genderEdit->setCurrentIndex(person->sUserGender);
    ui->jobEdit->setCurrentIndex(person->sUserJob);
    if(!person->isBlocked)
    {
        switch (person->sUserStatus) {
        case Status::online:{
            ui->statusEdit->setText("在线");
            ui->statusPoint->setStyleSheet("QLabel"
                                           "{"
                                           "background-color: rgb(44,221,113);"
                                           "border-radius :8px;"
                                           "}");
           break;
        }

        case Status::hide :case Status::offline:{
            ui->statusEdit->setText("离线");
            ui->statusPoint->setStyleSheet("QLabel"
                                           "{"
                                           "background-color: rgb(221,0,27);"
                                           "border-radius :8px;"
                                           "}");
           break;
        }

        default:
            break;
        }
    }else{
        ui->statusEdit->setText("拉黑");
        ui->statusPoint->setStyleSheet("QLabel"
                                       "{"
                                       "background-color: rgb(64,66,68);"
                                       "border-radius :8px;"
                                       "}");
    }

}

void FriendShow::on_btnAddBlackList_clicked()
{
    hide();
    emit add_to_blacklist();
}

void FriendShow::change_blacklist(bool ifBlock)
{
    if(ifBlock){
        ui->btnAddBlackList->setText("取消拉黑");
    }else{
        ui->btnAddBlackList->setText("拉黑");
    }
}
