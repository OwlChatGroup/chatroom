#ifndef ADDNEWFRIENDS_H
#define ADDNEWFRIENDS_H

#include <QDialog>
#include <UserListData.h>

namespace Ui {
class AddNewFriends;
}

class AddNewFriends : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewFriends(QWidget *parent = 0);
    ~AddNewFriends();
    UserItemData friendInfo;
    void load_friend_info(UserItemData info);

private slots:

    void on_btnOK_clicked();

    void on_cancel_clicked();

private:
    Ui::AddNewFriends *ui;

signals:
    void onAddClicked();

};

#endif // ADDNEWFRIENDS_H
