#include "register.h"
#include "ui_register.h"
#include <QGraphicsDropShadowEffect>
#include <QColor>
#include <QPainter>
#include <QImage>
#include <QImageReader>
#include <sstream>

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    //去除边框
    setWindowFlags(Qt::WindowType::FramelessWindowHint);

    //设置头像为yuan
    ui->label_17->setMask(QRegion(ui->label_17->rect(),QRegion::RegionType::Ellipse));
    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
    shadow_effect->setOffset(0, 0);
        //阴影颜色
    shadow_effect->setColor(QColor(38, 78, 119, 127));
        //阴影半径
    shadow_effect->setBlurRadius(50);
     ui->label_18->setGraphicsEffect(shadow_effect);


    isLeftPressDown = false;
    this->dir = QNONE;
    this->setMinimumHeight(100);
    this->setMinimumWidth(150);
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowSystemMenuHint );
    this->setMouseTracking(true);
    ui->lineEdit_2->setValidator(new QIntValidator(ui->lineEdit_2));
    QRegExp regx("[a-zA-Z0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, ui->lineEdit_3);
    ui->lineEdit_3->setValidator(validator);

   this->Rapi = new ApiUtils();
    connect(Rapi, SIGNAL(onRegisterCallback(uint8_t)), this, SLOT(registerCallback(uint8_t)));




        ui->label_7->hide();
        ui->label_8->hide();
        ui->label_16->hide();



        ui->imagelabel->setGeometry(0, 0, 444, 540);

        //添加自定义图片和文案描述
        const QList<std::pair<QString, QString>>&& imagepairlst = { std::make_pair(":/imge_login/page1.png", QString::fromLocal8Bit("")),
                                                                    std::make_pair(":/imge_login/page2.png", QString::fromLocal8Bit("")),
                                                                    std::make_pair(":/imge_login/page3.png", QString::fromLocal8Bit("")),
                                                                    std::make_pair(":/imge_login/page4.png", QString::fromLocal8Bit("")),
                                                                    std::make_pair(":/imge_login/page5.png", QString::fromLocal8Bit("")) };

        ui->imagelabel->addImage(imagepairlst);

//        ui->comboBox->lineEdit()->setPlaceholderText("-请选择-");
//        ui->comboBox_2->lineEdit()->setPlaceholderText("-请选择-");
//        ui->comboBox_3->lineEdit()->setPlaceholderText("-请选择-");


}


Register::~Register()
{
    delete ui;
}



void Register::registerCallback(uint8_t status)
{
    qDebug() << "Register--->" << TASK_STATUS_MSG[status];
if(status == TASK_STATUS::SUCCESS){
    qDebug()<<"successs";
    QMessageBox::information(this,"提示","注册成功");
    this->hide();
}
if(status == TASK_STATUS::USER_EXISTED_ERROR){
    qDebug()<<"USER_EXISTED_ERROR";
    QMessageBox::information(this,"提示","该用户已注册");

}


}
void Register::on_pushButton_clicked()
{
    QString	uName= ui->lineEdit->text();
    if(ui ->lineEdit_4->text()!= ui->lineEdit_3->text())
    {
        QMessageBox::warning(this,"警告","两次密码不相等");
        return;
        }

        QString qid = ui ->lineEdit_2->text();
        uint32_t _id = qid.toInt();
        QString _pwd = ui ->lineEdit_4->text();
        QString _nickname = ui ->lineEdit_5->text();
        uint8_t _gender = ui->comboBox->currentIndex();
//        QString _gender = QString::number(gender,10);
        if(_gender == 0){
            QMessageBox::warning(this,"警告","请选择您的性别");
            return;
        }
        QString age = ui->lineEdit->text();
        uint8_t _age = age.toInt();
        uint8_t _city = ui->comboBox_2->currentIndex();
//        QString _city = QString::number(city,10);
        if(_city == 0){
            QMessageBox::warning(this,"警告","请选择您所在的省份");
            return;
        }
        uint8_t _job = ui->comboBox_3->currentIndex();
        if(_job == 0){
            QMessageBox::warning(this,"警告","请选择您的职业");
            return;
        }
//        QString _job = QString::number(job,10);
        qDebug()<<"re"<<_id<<"  "<< _pwd<<"  "<<_nickname<<"  "<<_gender<<"  "<<_age<<"  "<<_city<<"  "<<_job;

        Rapi->onRegister(_id, _pwd,_nickname,_gender,_age,_city,_job);
//        QString packData = "#2|" + qid + "|" + _pwd + "|" + _nickname + "|" + _gender + "|" + _age + "|" + _city + "|" + _job + "&";
//        qDebug()<<packData;
//        client->write(packData.toLocal8Bit());
    }






void Register::on_btnMin_clicked()
{
    //最小化窗口
    this->showMinimized();
}

void Register::on_btnClose_clicked()
{
    this->close();
}













void Register::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        isLeftPressDown = false;
        if(dir != QNONE) {
            this->releaseMouse();
            this->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
}

void Register::mousePressEvent(QMouseEvent *event)
{
    switch(event->button()) {
    case Qt::LeftButton:
        isLeftPressDown = true;
        if(dir != QNONE) {
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

void Register::mouseMoveEvent(QMouseEvent *event)
{
    QPoint gloPoint = event->globalPos();
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());

    if(!isLeftPressDown) {
        this->region(gloPoint);
    } else {

        if(dir != QNONE) {
            QRect rMove(tl, rb);

            switch(dir) {
            case QLEFT:
                if(rb.x() - gloPoint.x() <= this->minimumWidth())
                    rMove.setX(tl.x());
                else
                    rMove.setX(gloPoint.x());
                break;
            case QRIGHT:
                rMove.setWidth(gloPoint.x() - tl.x());
                break;
            case QUP:
                if(rb.y() - gloPoint.y() <= this->minimumHeight())
                    rMove.setY(tl.y());
                else
                    rMove.setY(gloPoint.y());
                break;
            case QDOWN:
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            case QLEFTTOP:
                if(rb.x() - gloPoint.x() <= this->minimumWidth())
                    rMove.setX(tl.x());
                else
                    rMove.setX(gloPoint.x());
                if(rb.y() - gloPoint.y() <= this->minimumHeight())
                    rMove.setY(tl.y());
                else
                    rMove.setY(gloPoint.y());
                break;
            case QRIGHTTOP:
                rMove.setWidth(gloPoint.x() - tl.x());
                rMove.setY(gloPoint.y());
                break;
            case QLEFTBOTTOM:
                rMove.setX(gloPoint.x());
                rMove.setHeight(gloPoint.y() - tl.y());
                break;
            case QRIGHTBOTTOM:
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

void Register::region(const QPoint &cursorGlobalPoint)
{
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());
    int x = cursorGlobalPoint.x();
    int y = cursorGlobalPoint.y();

    if(tl.x() + PADDING >= x && tl.x() <= x && tl.y() + PADDING >= y && tl.y() <= y) {
        // 宸︿笂瑙?
        dir = QLEFTTOP;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if(x >= rb.x() - PADDING && x <= rb.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        // 鍙充笅瑙?
        dir = QRIGHTBOTTOM;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if(x <= tl.x() + PADDING && x >= tl.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        //宸︿笅瑙?
        dir = QLEFTBOTTOM;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= rb.x() && x >= rb.x() - PADDING && y >= tl.y() && y <= tl.y() + PADDING) {
        // 鍙充笂瑙?
        dir = QRIGHTTOP;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= tl.x() + PADDING && x >= tl.x()) {
        // 宸﹁竟
        dir = QLEFT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    } else if( x <= rb.x() && x >= rb.x() - PADDING) {
        // 鍙宠竟
        dir = QRIGHT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }else if(y >= tl.y() && y <= tl.y() + PADDING){
        // 涓婅竟
        dir = QUP;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    } else if(y <= rb.y() && y >= rb.y() - PADDING) {
        // 涓嬭竟
        dir = QDOWN;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }else {
        // 榛樿
        dir = QNONE;
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}




void Register::on_lineEdit_4_editingFinished()
{
    if(ui ->lineEdit_4->text()!= ui->lineEdit_3->text())
    {
        ui->label_7->show();
        }
    if(ui ->lineEdit_4->text()== ui->lineEdit_3->text())
    {
        ui->label_7->hide();
        }
}



void Register::on_addhs_clicked()
{
    qDebug()<<"ASdadsa";
    hs = new Headshots(this);
    hs->setGeometry(450,0,348,55);
    hs->show();
     connect(hs,&Headshots::onClicked,this,&Register::handle_headshots);


}
void Register::handle_headshots()
{



    QString fileName = QString(":/images/%1").arg(hs->headshotsName);
    qDebug()<<fileName;
    QPixmap pix;
    pix.load(fileName);
    pix = pix.scaled(80,80,Qt::KeepAspectRatioByExpanding);
    ui->label_17->setPixmap(pix);
    ui->label_17->installEventFilter(this);



}

void Register::on_lineEdit_2_textEdited(const QString &arg1)
{
    QString qstr1=ui ->lineEdit_2->text();
    qDebug()<<"str1"<<qstr1;
    if(qstr1=="")
    {
        ui->label_8->show();
        }
    if(qstr1!="")
    {
        ui->label_8->hide();
        }
}

void Register::on_lineEdit_5_textEdited(const QString &arg1)
{
    QString qstr2=ui ->lineEdit_5->text();
    qDebug()<<"str1"<<qstr2;
    if(qstr2=="")
    {
        ui->label_16->show();
        }
    if(qstr2!="")
    {
        ui->label_16->hide();
        }
}
