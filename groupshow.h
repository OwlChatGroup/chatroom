#ifndef GROUPSHOW_H
#define GROUPSHOW_H

#include <QDialog>
#include <QStandardItemModel>
#include "useritemdelegate.h"
#include "UserListData.h"
#include "unit.h"
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
class GroupShow;
}

class GroupShow : public QDialog
{
    Q_OBJECT

public:
    explicit GroupShow(QWidget *parent = 0);
    ~GroupShow();

    void load_users(QStandardItemModel *Model);
    void init_fun(UserItemDelegate* pUserItemDelegate);
    void on_btnEdit_clicked_2();
    int index();

   int  selectIndex;
   UserItemData friendInfo;
   QStandardItemModel *Model;

   bool ifAddMember = false;

private:
    Ui::GroupShow *ui;
    bool ifEdit;


signals:
    void delete_dialog();
    void add_new_friend();
    void quit_group();

    void delete_member();
    void add_member();
    void delete_all();
private slots:
    void on_btnDelete_clicked();
    void on_btnQuit_clicked();
    void on_btnAdd_clicked();
    void on_listView_clicked(const QModelIndex &index);
    void on_btnEdit_clicked();
    void on_btnDeleteAll_clicked();
    void on_btnAddMember_clicked();
    void on_btnDeleteMember_clicked();
};

#endif // GROUPSHOW_H


