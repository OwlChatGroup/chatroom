#ifndef PERSONALMAIN_H
#define PERSONALMAIN_H

#include <QDialog>
#include <UserListData.h>
#include <QWidget>
#include <QtGui>
#include <QTime>
#include <QStandardItemModel>
#include <QFile>
#include <expressionpanel.h>
#include <addnewfriends.h>
#include <friendshow.h>
#include <personalmain.h>
#include "ItemInfo.h"
#include "ifaddnewfriendbutton.h"
#include "groupshow.h"

namespace Ui {
class PersonalMain;
}

class PersonalMain : public QDialog
{
    Q_OBJECT

public:
    explicit PersonalMain(QWidget *parent = 0);
    ~PersonalMain();

    void load_person_info(UserItemData *person);
    bool eventFilter(QObject *obj, QEvent *event);
    int myStatus;

    void on_btnInit();
private slots:
    void on_btnPassword_clicked(bool flag);

    void on_btnConfirm_clicked();

    void on_statusEdit_currentIndexChanged(int index);

    void on_btnSave_clicked();

    void on_btnPassword_clicked();


private:
    Ui::PersonalMain *ui;
    bool ifEditPassword;
    bool ifEditInfo;

signals:
    void changeMyStatus();
};


#endif // PERSONALMAIN_H
