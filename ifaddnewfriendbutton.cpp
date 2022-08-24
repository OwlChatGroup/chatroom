#include "ifaddnewfriendbutton.h"
#include "ui_ifaddnewfriendbutton.h"

IfAddNewFriendButton::IfAddNewFriendButton(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IfAddNewFriendButton)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}

IfAddNewFriendButton::~IfAddNewFriendButton()
{
    delete ui;
}
