#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QTcpSocket>
#include <QDebug>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPoint>
#include <QCursor>
#include <QRect>
#include <ApiUtils/api_utils.h>
#include <register.h>
#include <owlchatmain.h>
#define PADDING 2


namespace Ui{
class Login;
}
enum Direction{
    UP = 0,
    DOWN=1,
    LEFT,
    RIGHT,
    LEFTTOP,
    LEFTBOTTOM,
    RIGHTBOTTOM,
    RIGHTTOP,
    NONE
};
class Login : public QDialog
{
    Q_OBJECT
public:
    explicit Login(QWidget *parent = 0);
    static ApiUtils* api;

    ~Login();
public slots:
//    void handconnected();
    void loginCallback(uint8_t);
    void on_btnLogin_clicked();
    void on_btnRegister_clicked();
private slots:
    void on_btnMin_clicked();
    void on_btnClose_clicked();

protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
private:
    bool isLeftPressDown;
    QPoint dragPosition;
    Direction dir;
    Ui::Login *ui;
    Register *reg;
    OwlChatMain * mw;
    void region(const QPoint &cursorPoint);
};




#endif // LOGIN_H
