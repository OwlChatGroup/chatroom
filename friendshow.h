#ifndef FRIENDSHOW_H
#define FRIENDSHOW_H

#include <QDialog>
#include <UserListData.h>

namespace Ui {
class FriendShow;
}

class FriendShow : public QDialog
{
    Q_OBJECT

public:
    explicit FriendShow(QWidget *parent = 0);
    ~FriendShow();
    void change_dialog_type(bool isChatList);

    void load_friend_info(UserItemData *person);
    void change_blacklist(bool ifBlock);

private:
    Ui::FriendShow *ui;

signals:
    void delete_friend();
    void delete_dialog();
    void add_to_blacklist();


private slots:

    void on_btnDeleteDialog_clicked();
    void on_btnAddBlackList_clicked();

};

#endif // FRIENDSHOW_H
