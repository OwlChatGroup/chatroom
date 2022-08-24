#include "login.h"
#include "ui_login.h"
#include <QMovie>
#include <QGraphicsDropShadowEffect>
#include <QColor>
#include <QPainter>
ApiUtils* Login::api;



Login:: Login(QWidget *parent):
    QDialog(parent),
    ui(new Ui::Login)
{
    //去除边框
    setWindowFlags(Qt::WindowType::FramelessWindowHint);
    //背景透明
    //setAttribute(Qt::WA_TranslucentBackground);

    ui->setupUi (this);

    isLeftPressDown = false;
    this->dir = NONE;
    this->setMinimumHeight(100);
    this->setMinimumWidth(150);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowSystemMenuHint );
    this->setMouseTracking(true);
    ui->lineEdit->setValidator(new QIntValidator(ui->lineEdit));

    this->setWindowIcon((QIcon(":/images/appIcon.ico")));

    //设置gif
    QMovie *movie = new QMovie(this);
    movie->setFileName(":/imge_login/gifLabel.gif");
    movie->start();
    ui->label_3->setMovie(movie);

    //设置头像为圆
    ui->label_6->setMask(QRegion(ui->label_6->rect(),QRegion::RegionType::Ellipse));
    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
    shadow_effect->setOffset(0, 0);
        //阴影颜色
    shadow_effect->setColor(QColor(38, 78, 119, 127));
        //阴影半径
    shadow_effect->setBlurRadius(35);
     ui->label_6->setGraphicsEffect(shadow_effect);






    api = new ApiUtils();
    connect(api, SIGNAL(onLoginCallback(uint8_t)), this, SLOT(loginCallback(uint8_t)));

}
    Login::~Login()
    {
        delete ui;
    }
//    void Login::handconnected()
//    {
//        ui->btnLogin->setEnabled(true);
//        ui->btnRegister->setEnabled(true);
//        connect(client,SIGNAL(readyRead()) ,this,SLOT(handReadyRead()));
//    }

void Login::loginCallback(uint8_t status)
{

    qDebug() << "Login--->" << TASK_STATUS_MSG[status];
if(status == TASK_STATUS::SUCCESS){
    qDebug()<<"successs";
//    QMessageBox::information(this,"提示","登录成功");;
                mw = new OwlChatMain();
                mw->show();
                this->hide();
}
if(status == TASK_STATUS::NO_USER_ERROR){
    qDebug()<<"NO_USER_ERROR";
    QMessageBox::information(this,"提示","账号不存在");;
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
}
if(status == TASK_STATUS::WRONG_PWD_ERROR){
    qDebug()<<"WRONG_PWD_ERROR";
    QMessageBox::information(this,"提示","账号或密码错误");;
    ui->lineEdit->setText("");
    ui->lineEdit_2 ->setText("");
}



}




void Login::on_btnMin_clicked()
{
    //最小化窗口
    this->showMinimized();
}

void Login::on_btnClose_clicked()
{
    //关闭窗口
    this->close();
}

void Login::on_btnLogin_clicked()
{
    QString uName = ui->lineEdit->text();
    uint32_t _uName = uName.toInt();
    QString pWord = ui->lineEdit_2->text();
    if(uName.length()==0&&pWord.length()==0){
        //请输入用户名和密码
        qDebug()<<"请输入用户名和密码";
        QMessageBox::information(this,"提示","请输入用户名和密码");
    }
    else{
        if(uName.length()==0){
            qDebug()<<"请输入用户名";
            QMessageBox::information(this,"提示","请输入用户名");
        }
        if(pWord.length()==0){
            qDebug()<<"请输入密码";
            QMessageBox::information(this,"提示","请输入密码");

        }

    }

//    //#1|admin|123&
//    QString packData ="#1|"+ uName+ "|" +pWord + "&";
//    client->write(packData.toLocal8Bit());
    qDebug()<<"_uName" << _uName <<"  "<<pWord;
    qDebug() << "myID_lo" << mw->myID;
    mw->setMyID(_uName);
    mw->api = api;
    qDebug() << "myID_lo" << mw->myID;

    api->onLogin(_uName, pWord);
}

void Login::on_btnRegister_clicked()
{
    reg = new Register();
    reg->show();
}

void Login::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        isLeftPressDown = false;
        if(dir != NONE) {
            this->releaseMouse();
            this->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
}

void Login::mousePressEvent(QMouseEvent *event)
{
    switch(event->button()) {
    case Qt::LeftButton:
        isLeftPressDown = true;
        if(dir != NONE) {
            this->mouseGrabber();
        } else {
            dragPosition = event->globalPos() - this->frameGeometry().topLeft();
        }
        break;
    case Qt::RightButton:
        this->close();
        break;
    default:
        QDialog::mousePressEvent(event);
    }

}

void Login::mouseMoveEvent(QMouseEvent *event)
{
    QPoint gloPoint = event->globalPos();
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());

    if(!isLeftPressDown) {
        this->region(gloPoint);
    } else {

        if(dir != NONE) {
            QRect rMove(tl, rb);

            switch(dir) {
            case LEFT:
                if(rb.x() - gloPoint.x() <= this->minimumWidth())
                    rMove.setX(tl.x());
                else
                    rMove.setX(gloPoint.x());
                break;
            case RIGHT:
                rMove.setWidth(gloPoint.x() - tl.x());
                break;
            case UP:
                if(rb.y() - gloPoint.y() <= this->minimumHeight())
                    rMove.setY(tl.y());
                else
                    rMove.setY(gloPoint.y());
                break;
            case DOWN:
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            case LEFTTOP:
                if(rb.x() - gloPoint.x() <= this->minimumWidth())
                    rMove.setX(tl.x());
                else
                    rMove.setX(gloPoint.x());
                if(rb.y() - gloPoint.y() <= this->minimumHeight())
                    rMove.setY(tl.y());
                else
                    rMove.setY(gloPoint.y());
                break;
            case RIGHTTOP:
                rMove.setWidth(gloPoint.x() - tl.x());
                rMove.setY(gloPoint.y());
                break;
            case LEFTBOTTOM:
                rMove.setX(gloPoint.x());
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            case RIGHTBOTTOM:
                rMove.setWidth(gloPoint.x() - tl.x());
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            default:

                break;
            }
            this->setGeometry(rMove);
        } else {
            move(event->globalPos() - dragPosition);
            event->accept();
        }
    }
    QDialog::mouseMoveEvent(event);
}

void Login::region(const QPoint &cursorGlobalPoint)
{
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());
    int x = cursorGlobalPoint.x();
    int y = cursorGlobalPoint.y();

    if(tl.x() + PADDING >= x && tl.x() <= x && tl.y() + PADDING >= y && tl.y() <= y) {
        // 宸︿笂瑙?
        dir = LEFTTOP;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if(x >= rb.x() - PADDING && x <= rb.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        // 鍙充笅瑙?
        dir = RIGHTBOTTOM;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if(x <= tl.x() + PADDING && x >= tl.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        //宸︿笅瑙?
        dir = LEFTBOTTOM;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= rb.x() && x >= rb.x() - PADDING && y >= tl.y() && y <= tl.y() + PADDING) {
        // 鍙充笂瑙?
        dir = RIGHTTOP;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= tl.x() + PADDING && x >= tl.x()) {
        // 宸﹁竟
        dir = LEFT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    } else if( x <= rb.x() && x >= rb.x() - PADDING) {
        // 鍙宠竟
        dir = RIGHT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }else if(y >= tl.y() && y <= tl.y() + PADDING){
        // 涓婅竟
        dir = UP;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    } else if(y <= rb.y() && y >= rb.y() - PADDING) {
        // 涓嬭竟
        dir = DOWN;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }else {
        // 榛樿
        dir = NONE;
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}
