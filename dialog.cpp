#include "dialog.h"
#include "ui_dialog.h"
#include "ItemInfo.h"
#include "global.h"
#include "unit.h"

#include <QDateTime>
#include <QTimer>
#include <QTime>
#include <QDebug>

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
#include <QTextBlock>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QImage>
#include <QImageReader>
//#pragma execution_character_set("utf-8")
#define DATE_TIME       QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->tedit_send_msg->setAcceptDrops(false);
    this->setAcceptDrops(true);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::dragEnterEvent(QDragEnterEvent *e)
{
    //if(e->mimeData()->urls()[0].fileName().right(4).compare(".jpg")==0)
    if(e->mimeData()->hasFormat("text/uri-list"))
    {
        e->acceptProposedAction();
    }
    else
    {
        e->ignore();
    }
}

void Dialog::dropEvent(QDropEvent *e)
{
    QList<QUrl> urls = e->mimeData()->urls();

    if(urls.empty())
        return;

    for (int i=0; i < urls.size();i++)
    {
        QString fileName = urls[i].toLocalFile();
        QString extendName = fileName.right(4).toLower();
        QUrl Uri ( QString ( "file://%1" ).arg ( fileName ) );
        if(extendName.compare(".jpg")==0 || extendName.compare(".jpeg")==0 || extendName.compare(".png")==0 || extendName.compare(".bmp")==0 || extendName.compare(".gif")==0)
        {
            //图片
            QImage image = QImageReader ( fileName ).read();

            QTextDocument * textDocument = ui->tedit_send_msg->document();
            textDocument->addResource( QTextDocument::ImageResource, Uri, QVariant ( image ) );
            QTextCursor cursor = ui->tedit_send_msg->textCursor();
            QTextImageFormat imageFormat;
            int nSmall = image.width()/150;
            imageFormat.setWidth( image.width()/nSmall );
            imageFormat.setHeight( image.height()/nSmall );
            imageFormat.setName( Uri.toString() );
            cursor.insertImage(imageFormat);
            m_sAcPicList.push_back(fileName);
        }
        else
        {
            //插入文件图标
            QImage image = QImageReader (":/images/file.png").read();

            QTextDocument * textDocument = ui->tedit_send_msg->document();
            textDocument->addResource( QTextDocument::ImageResource, Uri, QVariant ( image ) );
            QTextCursor cursor = ui->tedit_send_msg->textCursor();
            QTextImageFormat imageFormat;

            imageFormat.setWidth( image.width() );
            imageFormat.setHeight( image.height() );
            imageFormat.setName( Uri.toString() );
            cursor.insertImage(imageFormat);

            m_sAcFileList.push_back(fileName);
        }

    }

    //QString fileName = urls.first().toLocalFile();

//    QTextDocumentFragment fragment;
//    fragment = QTextDocumentFragment::fromHtml(QString("<img src='%1'>").arg(fileName));
//    ui->tedit_send_msg->textCursor().insertFragment(fragment);


}

void Dialog::on_btn_sendmsg_clicked()
{
    QString text = ui->tedit_send_msg->toPlainText();
    QChar qcSlit = (QChar)(0xfffc);
    QStringList textList = text.split(qcSlit);//拆分

    // 把最后一个回车换行符删掉
    while (text.endsWith("\n"))
    {
        text.remove(text.length() - 1, 1);
    }

    // 判断消息是否为空
    if (text.isEmpty()) {
        QPoint point = ui->widgetMsg->mapToGlobal(ui->btn_sendmsg->pos());
        QToolTip::showText(point, tr("发送消息内容不能为空，请重新输入！"));
        return;
    }

    //判断是否要插入当前时间
    bool bNeedShowTime = false;
    if(m_lastShowMsgTime.isNull())
    {
        m_lastShowMsgTime = QDateTime::currentDateTime();
        bNeedShowTime = true;
    }
    else
    {
        qint64 nTimeSub = m_lastShowMsgTime.msecsTo(QDateTime::currentDateTime());
        double dMinute = ((double)nTimeSub/1000.0)/60.0;

        //设置不活动时间大于3分钟才再次显示时间
        if((dMinute - 3.0) > 0.000001)
        {

            bNeedShowTime = true;
        }
        m_lastShowMsgTime = QDateTime::currentDateTime();
    }

    //插入时间
    if(bNeedShowTime)
    {
        ItemInfo *itemTime = new ItemInfo();
        itemTime->SetDatetime(DATE_TIME);
        itemTime->SetMsgType(DateTime);
        itemTime->SetOrientation(None);
        ui->widget_bubble_show->addItem(itemTime);
    }

    //检查是否有图片，有的话，先发送图片
    for (int i=0; i<m_sAcPicList.size(); i++)
    {
        //构建气泡消息:图片
        ItemInfo *picitemInfo = new ItemInfo();
        picitemInfo->SetName("lanzhiyu");
        picitemInfo->SetDatetime(DATE_TIME);
        picitemInfo->SetHeadPixmap(":/images/1.png");
        picitemInfo->SetText(m_sAcPicList[i]);
        picitemInfo->SetOrientation(Right);
        picitemInfo->SetMsgType(Picture);

        // 加入聊天界面
        ui->widget_bubble_show->addItem(picitemInfo);
    }

    //检查是否有文件，有的话，发送文件
    for (int i=0; i<m_sAcFileList.size(); i++)
    {
        //构建气泡消息:文件
        ItemInfo *FileitemInfo = new ItemInfo();
        FileitemInfo->SetName("lanzhiyu");
        FileitemInfo->SetDatetime(DATE_TIME);
        FileitemInfo->SetHeadPixmap(":/images/1.png");

        QFileInfo fileInfo(m_sAcFileList[i]);
        int lasl=fileInfo.size();

        FileitemInfo->SetText(m_sAcFileList[i]);
        FileitemInfo->SetFileSizeString(QString::number(fileInfo.size()));
        FileitemInfo->SetOrientation(Right);
        FileitemInfo->SetMsgType(Files);

        // 加入聊天界面
        ui->widget_bubble_show->addItem(FileitemInfo);
    }



    int nEmptyNum = 0;
    for (int i=0; i<textList.size(); i++)
    {
        if(textList[i].isEmpty())
        {
            nEmptyNum++;
            continue;
        }
        // 构建气泡消息
        ItemInfo *itemInfo = new ItemInfo();
        itemInfo->SetName("moonlight");
        itemInfo->SetDatetime(DATE_TIME);
        itemInfo->SetHeadPixmap(":/images/1.png");
        itemInfo->SetText(textList[i]);
        itemInfo->SetOrientation(Right);

        // 加入聊天界面
        ui->widget_bubble_show->addItem(itemInfo);
    }


    if(textList.size() > 0)
    {

        //模拟收信息
        ItemInfo *itemInfo2 = new ItemInfo();
        itemInfo2->SetName("bluesky");
        itemInfo2->SetDatetime(DATE_TIME);
        itemInfo2->SetHeadPixmap(":/images/2.png");

        //QString sMsg = QString("Text msg num=%1,Picture num=%2,Files num=%3").arg(textList.size()-nEmptyNum).arg(m_sAcPicList.size()).arg(m_sAcFileList.size());
        QString sMsg=QString("收到你发送的文字消息数：%1，图片数:%2，文件数:%3").arg(textList.size()-nEmptyNum).arg(m_sAcPicList.size()).arg(m_sAcFileList.size());
        itemInfo2->SetText(sMsg);
        itemInfo2->SetOrientation(Left);
        ui->widget_bubble_show->addItem(itemInfo2);
    }


    m_sAcFileList.clear();
    m_sAcPicList.clear();//清空图片列表

    // 清除输入框
    ui->tedit_send_msg->clear();
    ui->tedit_send_msg->setFocus();
}

//void Dialog::on_verticalScrollBar_valueChanged(int value)
//{
//    double dOffSet = static_cast<double>(value) / static_cast<double>(ui->verticalScrollBar->maximum());
//    ui->widget_bubble_show->move(0,-(ui->widget_bubble_show->height() - 350) * dOffSet);

//}

