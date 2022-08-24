
#ifndef  REGISTER_H
#define REGISTER_H

#include<QDialog>
#include<QMessageBox>
#include<QDebug>
#include<QtcpSocket>
#include <QMouseEvent>
#include <QPoint>
#include <QCursor>
#include <QRect>
#include <QPainter>
#include<headshots.h>

#include <ApiUtils/api_utils.h>
#define PADDING 2
namespace Ui {

class Register;

}

enum DirectionR{
    QUP = 0,
    QDOWN=1,
    QLEFT,
    QRIGHT,
    QLEFTTOP,
    QLEFTBOTTOM,
    QRIGHTBOTTOM,
    QRIGHTTOP,
    QNONE
};


class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = 0);
    explicit Register(QTcpSocket *sock,QWidget *parent = 0);
    ~Register();
private slots:
    void on_pushButton_clicked();
    void on_btnMin_clicked();
    void on_btnClose_clicked();
    void registerCallback(uint8_t);
    void on_lineEdit_4_editingFinished();


    void handle_headshots();

    void on_addhs_clicked();

    void on_lineEdit_2_textEdited(const QString &arg1);

    void on_lineEdit_5_textEdited(const QString &arg1);

private:
    void region(const QPoint &cursorPoint);
protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
private:
    bool isLeftPressDown;
    QPoint dragPosition;
    DirectionR dir;

private:
    Ui::Register *ui;
    ApiUtils* Rapi;
    Headshots * hs;
};
#endif//REGISTER_H
