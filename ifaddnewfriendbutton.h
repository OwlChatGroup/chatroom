#ifndef IFADDNEWFRIENDBUTTON_H
#define IFADDNEWFRIENDBUTTON_H

#include <QDialog>

namespace Ui {
class IfAddNewFriendButton;
}

class IfAddNewFriendButton : public QDialog
{
    Q_OBJECT

public:
    explicit IfAddNewFriendButton(QWidget *parent = 0);
    ~IfAddNewFriendButton();

private:
    Ui::IfAddNewFriendButton *ui;
};

#endif // IFADDNEWFRIENDBUTTON_H
