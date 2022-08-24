#include "personalmain.h"
#include "ui_personalmain.h"
#include <QDebug>
#include <QMessageBox>
#include "useritemdelegate.h"
#include "UserListData.h"
#include "ItemInfo.h"
#include "global.h"
#include "unit.h"
#include "ItemInfo.h"
#include <addnewfriends.h>

#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QKeyEvent>
#include <QToolTip>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QUrl>
#include <QList>
#include <QFile>
#include <QDateTime>
#include <QTimer>
#include <QTime>
#include <QDebug>


#include <QTextBlock>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QImage>
#include <QImageReader>
#include <QFile>
#include <QFileInfo>

 PersonalMain::PersonalMain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonalMain)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    ui->headImg->installEventFilter(this);
    //隐藏密码修改
    ifEditPassword = false;
    ifEditInfo = false;

    ui->passwordOldLabel->setVisible(ifEditPassword);
    ui->passwordOldEdit->setVisible(ifEditPassword);
    ui->passwordNewLabel->setVisible(ifEditPassword);
    ui->passwordNewEdit->setVisible(ifEditPassword);
    ui->passwordConfirmLabel->setVisible(ifEditPassword);
    ui->passwordConfirmEdit->setVisible(ifEditPassword);
}

PersonalMain::~PersonalMain()
{
    delete ui;
}

void PersonalMain::on_btnPassword_clicked()
{
    ifEditPassword = !ifEditPassword;

    ui->passwordOldLabel->setVisible(ifEditPassword);
    ui->passwordOldEdit->setVisible(ifEditPassword);
    ui->passwordNewLabel->setVisible(ifEditPassword);
    ui->passwordNewEdit->setVisible(ifEditPassword);
    ui->passwordConfirmLabel->setVisible(ifEditPassword);
    ui->passwordConfirmEdit->setVisible(ifEditPassword);

    ui->accountEdit->setVisible(!ifEditPassword);
    ui->accountLabel->setVisible(!ifEditPassword);
    ui->cityEdit->setVisible(!ifEditPassword);
    ui->cityLabel->setVisible(!ifEditPassword);
    ui->ageEdit->setVisible(!ifEditPassword);
    ui->ageLabel->setVisible(!ifEditPassword);
    ui->genderEdit->setVisible(!ifEditPassword);
    ui->genderLabel->setVisible(!ifEditPassword);
    ui->jobEdit->setVisible(!ifEditPassword);
    ui->jobLabel->setVisible(!ifEditPassword);


    if(ifEditPassword){
        ui->btnPassword->setText("返回信息");
    }else{
        ui->btnPassword->setText("修改密码");
    }

}

void PersonalMain::on_btnConfirm_clicked()
{
    if(ifEditPassword){
        on_btnPassword_clicked();
    }

    ifEditInfo= !ifEditInfo;
    if(ifEditInfo){
        ui->btnConfirm->setText("确认修改");
    }else{
        ui->btnConfirm->setText("修改信息");
    }
    ui->nameEdit->setEnabled(ifEditInfo);
    ui->signatureEdit->setEnabled(ifEditInfo);
    ui->accountEdit->setEnabled(ifEditInfo);
    ui->ageEdit->setEnabled(ifEditInfo);
    ui->cityEdit->setEnabled(ifEditInfo);
    ui->genderEdit->setEnabled(ifEditInfo);
    ui->jobEdit->setEnabled(ifEditInfo);
}

void PersonalMain::load_person_info(UserItemData *person)
{
    ui->nameEdit->setText(person->sUserName);
    ui->signatureEdit->setText(person->sUserSignature);
    ui->accountEdit->setText(QString::number(person->sUserID,10));
    ui->ageEdit->setText(QString::number(person->sUserAge));
    ui->cityEdit->setCurrentIndex(person->sUserCity);
    ui->genderEdit->setCurrentIndex(person->sUserGender);
    ui->jobEdit->setCurrentIndex(person->sUserJob);
    switch (person->sUserStatus) {
    case Status::online:
        ui->statusEdit->setCurrentIndex(0);
        break;
    case Status::hide:
        ui->statusEdit->setCurrentIndex(1);
        break;
    case Status::offline:
        ui->statusEdit->setCurrentIndex(2);
        break;
    default:
        break;
    }

}

void PersonalMain::on_statusEdit_currentIndexChanged(int index)
{
    myStatus = index;
    qDebug() << index;
    emit changeMyStatus();
}

void PersonalMain::on_btnSave_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("是否保存信息？");
    QPushButton *saveButton = msgBox.addButton(tr("保存"), QMessageBox::ActionRole);
    QPushButton *cancelButton = msgBox.addButton(tr("取消"), QMessageBox::ActionRole);
    msgBox.exec();

    if (msgBox.clickedButton() == saveButton) {
        // connect
        qDebug() << "1";
    } else if (msgBox.clickedButton() == cancelButton) {
        // abort
        qDebug() << "2";
    }
    hide();
}

bool PersonalMain::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)//mouse button pressed
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent->button() == Qt::LeftButton)
        {
            if(obj == ui->headImg)
            {
                qDebug()<<"success!";
                QString filePath = NULL;
                filePath = QFileDialog::getOpenFileName(this,"open","../");
                qDebug() << filePath;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return QObject::eventFilter(obj, event);
    }
}


void PersonalMain::on_btnInit()
{
    ifEditPassword = false;

    ui->passwordOldLabel->setVisible(ifEditPassword);
    ui->passwordOldEdit->setVisible(ifEditPassword);
    ui->passwordNewLabel->setVisible(ifEditPassword);
    ui->passwordNewEdit->setVisible(ifEditPassword);
    ui->passwordConfirmLabel->setVisible(ifEditPassword);
    ui->passwordConfirmEdit->setVisible(ifEditPassword);

    ui->accountEdit->setVisible(!ifEditPassword);
    ui->accountLabel->setVisible(!ifEditPassword);
    ui->cityEdit->setVisible(!ifEditPassword);
    ui->cityLabel->setVisible(!ifEditPassword);
    ui->ageEdit->setVisible(!ifEditPassword);
    ui->ageLabel->setVisible(!ifEditPassword);
    ui->genderEdit->setVisible(!ifEditPassword);
    ui->genderLabel->setVisible(!ifEditPassword);
    ui->jobEdit->setVisible(!ifEditPassword);
    ui->jobLabel->setVisible(!ifEditPassword);


    if(ifEditPassword){
        ui->btnPassword->setText("返回信息");
    }else{
        ui->btnPassword->setText("修改密码");
    }
}
