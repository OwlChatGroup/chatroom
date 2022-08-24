#include "owlchatmain.h"
#include "ui_owlchatmain.h"
#include "useritemdelegate.h"
#include "UserListData.h"
#include "ItemInfo.h"
#include "global.h"
#include "unit.h"
#include "ItemInfo.h"
#include <addnewfriends.h>

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
#include <QDateTime>
#include <QTimer>
#include <QTime>
#include <QDebug>


#include <QTextBlock>
#include <QTextDocument>
#include <QTextDocumentFragment>
#include <QImage>
#include <QImageReader>
#include <QFile>
#include <QFileInfo>
#include <ApiUtils/api_utils.h>

#define min(a,b) ((a)<(b)? (a) :(b))
#define max(a,b) ((a)>(b)? (a) :(b))
ApiUtils* OwlChatMain::api;
uint8_t OwlChatMain::myID;

//预存用户列表数据
const QStringList headpics = {
    ":/images/1.png", ":/images/2.png", ":/images/3.png",
    ":/images/4.png", ":/images/5.png", ":/images/6.png",
};
const QStringList names = {
    "蓝之羽", "韦小宝", "扶桑梅川内酷",
    "爱新觉罗玄烨", "努尔哈赤", "西门吹雪",
};
const QStringList userids = {
    "lanzu520", "ake520", "wodeneikuzainali",
    "daqingchaohuangdi", "woshishui333", "ximenchuixue666",
};

OwlChatMain::OwlChatMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OwlChatMain)
{
    ui->setupUi(this);
    this->setWindowIcon((QIcon(":/images/appIcon.ico")));
    this->setWindowIconText("OwlChat");
    this->setWindowTitle("OwlChat");
    myInfo = new UserItemData();
    friendInfo = new UserItemData();

//    api = new ApiUtils();
    api->getUserInfo(11);
    api->getUserDetail(11u);
    qDebug() << "myID" << myID;
    qDebug() << "myID" << myID;


    //添加头像图片
    QIcon icon;
    icon.addFile(":/images/3.png");
    ui->btnHeadIcon->setIcon(icon);
    ui->btnHeadIcon->setIconSize(QSize(58,58));
    //    ui->btnHeadIcon->setMask(region);
    QBitmap qb(":/images/mask.png");
    QBitmap qb2 =  qb.scaled(50,50);
    ui->btnHeadIcon->setMask(QRegion(ui->btnHeadIcon->rect(),QRegion::RegionType::Ellipse));
    //    ui->btnHeadIcon->setMask(qb);
    resizeDir = nodir;   //初始化检测方向为无
    setWindowFlags(Qt::FramelessWindowHint);  //设置无边框
    setMouseTracking(true); //开启鼠标追踪
    setMinimumSize(90, 200);
    //    connect(ui->buttonClose, SIGNAL(clicked()), this, SLOT(close()));
    //    connect(ui->buttonMin, SIGNAL(clicked()), this, SLOT(showMinimized()));
    //插入列表数据
    m_pModel = new QStandardItemModel;
    m_pModel2 = new QStandardItemModel;
    for (int i=0; i<headpics.size(); ++i) {
        QStandardItem *pItem = new QStandardItem;
        UserItemData ItemData;
        ItemData.sUserName = names.at(i);
        ItemData.sUserID = 123;
        ItemData.sHeadPic = headpics.at(i);
        pItem->setData(QVariant::fromValue(ItemData), Qt::UserRole+1);
        m_pModel->appendRow(pItem);
    }
    for (int i=0; i<headpics.size(); ++i) {
        QStandardItem *pItem = new QStandardItem;
        UserItemData ItemData;
        ItemData.sUserName = names.at(i);
        ItemData.sUserID = 123;
        ItemData.sHeadPic = headpics.at(i);
        pItem->setData(QVariant::fromValue(ItemData), Qt::UserRole+1);
        m_pModel2->appendRow(pItem);
    }
    UserItemDelegate* pUserItemDelegate = new UserItemDelegate;
    ui->listView->setItemDelegate(pUserItemDelegate);
    ui->listView->clearFocus();
    QStandardItemModel *newModel = new QStandardItemModel();
    ui->listView->setModel(newModel);
    ui->listView->setModel(m_pModel2);
    //改变ChatList的颜色
    isChatList=true;
    changeColor(isChatList);
    //没有选择会话时候不能按
    ui->btnMore->setEnabled(false);
    ui->btnExpression->setEnabled(false);
    ui->btnFolder->setEnabled(false);
    ui->btnSend->setEnabled(false);
    //接受文件拖拽
    ui->tedit_send_msg->setAcceptDrops(false);
    this->setAcceptDrops(true);
    //设置头像
    myHeadIcon = ":/images/right.png";
    friendHeadIcon = ":/images/left.jpg";

    //表情
    ep = new ExpressionPanel(this);
    ep->hide();

    //两个按钮
    ui->btnFolder->setToolTip(tr("选择文件"));
    ui->btnExpression->setToolTip(tr("选择表情"));
    connect(ep,&ExpressionPanel::onClicked,this,&OwlChatMain::handle_expression);


    //添加新朋友
    add = new AddNewFriends(this);
    add->hide();
    connect(add,&AddNewFriends::onAddClicked,this,&OwlChatMain::handle_add);

    //好友更多信息
    mo = new FriendShow(this);
    mo->hide();
    mo->load_friend_info(friendInfo);
    connect(mo,&FriendShow::delete_dialog,this,&OwlChatMain::handle_delete_dialog);
    connect(mo,&FriendShow::delete_friend,this,&OwlChatMain::handle_delete_friend);
    connect(mo,&FriendShow::add_to_blacklist,this,&OwlChatMain::handle_add_to_blacklist);

    //群组更多信息
    UserItemDelegate* pUserItemDelegate2 = new UserItemDelegate;
    //    ui->listView->setItemDelegate(pUserItemDelegate2);
    mo_g = new GroupShow(this);
    mo_g->init_fun(pUserItemDelegate2);
    mo_g->load_users(m_pModel);
    mo_g->hide();
    connect(mo_g,&GroupShow::add_new_friend,this,&OwlChatMain::handle_add_new_friend);
    connect(mo_g,&GroupShow::delete_dialog,this,&OwlChatMain::handle_delete_dialog_group);
    connect(mo_g,&GroupShow::quit_group,this,&OwlChatMain::handle_quit_group);

    connect(mo_g,&GroupShow::add_member,this,&OwlChatMain::handle_add_member);
    connect(mo_g,&GroupShow::delete_member,this,&OwlChatMain::handle_delete_member);
    connect(mo_g,&GroupShow::delete_all,this,&OwlChatMain::handle_delete_all);
    //个人更多信息
    pm = new PersonalMain(this);
    pm->hide();
    pm->load_person_info(myInfo);
    connect(pm,&PersonalMain::changeMyStatus,this,&OwlChatMain::handle_chang_my_status);

    //是否添加好友按钮
    ifAdd = new IfAddNewFriendButton(this);
    ifAdd->hide();
}

OwlChatMain::~OwlChatMain()
{
    delete ui;
}


void OwlChatMain::mousePressEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton)  //每当按下鼠标左键就记录一下位置
    {
        dragPosition = event->globalPos() - frameGeometry().topLeft();  //获得鼠标按键位置相对窗口左上面的位置
    }


}

//void OwlChatMain::testEdge(QMouseEvent *event)
//{
//    int diffLeft = event->globalPos().x() - frameGeometry().left();      //计算鼠标距离窗口上下左右有多少距离
//    int diffRight = event->globalPos().x() - frameGeometry().right();
//    int diffTop = event->globalPos().y() - frameGeometry().top();
//    int diffBottom = event->globalPos().y() - frameGeometry().bottom();

//    Qt::CursorShape cursorShape;

//    if(diffTop < EDGE_MARGIN && diffTop>=0){                              //根据 边缘距离 分类改变尺寸的方向
//        if(diffLeft < EDGE_MARGIN && diffLeft>=0){
//            resizeDir = topLeft;
//            cursorShape = Qt::SizeFDiagCursor;
//        }
//        else if(diffRight > -EDGE_MARGIN && diffRight<=0){
//            resizeDir = topRight;
//            cursorShape = Qt::SizeBDiagCursor;
//        }
//        else{
//            resizeDir = top;
//            cursorShape = Qt::SizeVerCursor;
//        }
//    }
//    else if(abs(diffBottom) < EDGE_MARGIN && diffBottom<=0){
//        if(diffLeft < EDGE_MARGIN && diffLeft>=0){
//            resizeDir = bottomLeft;
//            cursorShape = Qt::SizeBDiagCursor;
//        }
//        else if(diffRight > -EDGE_MARGIN && diffRight<=0){
//            resizeDir = bottomRight;
//            cursorShape = Qt::SizeFDiagCursor;
//        }
//        else{
//            resizeDir = bottom;
//            cursorShape = Qt::SizeVerCursor;
//        }
//    }
//    else if(abs(diffLeft) < EDGE_MARGIN){
//        resizeDir = left;
//        cursorShape = Qt::SizeHorCursor;
//    }
//    else if(abs(diffRight) < EDGE_MARGIN){
//        resizeDir = right;
//        cursorShape = Qt::SizeHorCursor;
//    }
//    else{
//        resizeDir = nodir;
//        cursorShape = Qt::ArrowCursor;
//    }

//    QApplication::setOverrideCursor(cursorShape);
//}

void OwlChatMain::mouseMoveEvent(QMouseEvent * event)
{
    if (event->buttons() & Qt::LeftButton){                 //如果左键是按下的
        if(resizeDir == nodir){                             //如果鼠标不是放在边缘那么说明这是在拖动窗口
            move(event->globalPos() - dragPosition);
        }
        else{
            int ptop,pbottom,pleft,pright;                   //窗口上下左右的值
            ptop = frameGeometry().top();
            pbottom = frameGeometry().bottom();
            pleft = frameGeometry().left();
            pright = frameGeometry().right();
            if(resizeDir & top){                               //检测更改尺寸方向中包含的上下左右分量
                if(height() == minimumHeight()){
                    ptop = min(event->globalY(),ptop);
                }
                else if(height() == maximumHeight()){
                    ptop = max(event->globalY(),ptop);
                }
                else{
                    ptop = event->globalY();
                }
            }
            else if(resizeDir & bottom){
                if(height() == minimumHeight()){
                    pbottom = max(event->globalY(),ptop);
                }
                else if(height() == maximumHeight()){
                    pbottom = min(event->globalY(),ptop);
                }
                else{
                    pbottom = event->globalY();
                }
            }

            if(resizeDir & left){                        //检测左右分量
                if(width() == minimumWidth()){
                    pleft = min(event->globalX(),pleft);
                }
                else if(width() == maximumWidth()){
                    pleft = max(event->globalX(),pleft);
                }
                else{
                    pleft = event->globalX();
                }
            }
            else if(resizeDir & right){
                if(width() == minimumWidth()){
                    pright = max(event->globalX(),pright);
                }
                else if(width() == maximumWidth()){
                    pright = min(event->globalX(),pright);
                }
                else{
                    pright = event->globalX();
                }
            }
            setGeometry(QRect(QPoint(pleft,ptop),QPoint(pright,pbottom)));
        }
    }
    //    else testEdge(event);   //当不拖动窗口、不改变窗口大小尺寸的时候  检测鼠标边缘


}
void OwlChatMain::mouseReleaseEvent(QMouseEvent *event)
{
    if(resizeDir != nodir){         //还原鼠标样式
        //        testEdge(event);
    }
}

void OwlChatMain::enterEvent(QEvent * event)
{
    QWidget::enterEvent(event);
}

void OwlChatMain::leaveEvent(QEvent * event)
{
    //    QApplication::setOverrideCursor(Qt::IBeamCursor);
    QWidget::leaveEvent(event);
}

void OwlChatMain::setDialogWidget(QWidget *w)
{
    //    ui->dialogwidget->layout()->addWidget(w);
}

//下面处理右上角按钮



void OwlChatMain::on_btnClose_clicked()
{
    this->close();
}

void OwlChatMain::on_btnMin_clicked()
{
    this->showMinimized();
}

void OwlChatMain::on_btnSkin_clicked()
{

}

//处理列表数据

void OwlChatMain::on_listView_clicked(const QModelIndex &index)
{
    int nRow = index.row();
    int nCol = index.column();
    int nCount = ui->listView->model()->rowCount();
    selectIndex_last= selectIndex;
    selectIndex = index.row();

    ui->btnMore->setEnabled(true);
    ui->btnExpression->setEnabled(true);
    ui->btnFolder->setEnabled(true);
    ui->btnSend->setEnabled(true);

    if(selectIndex_last != selectIndex){
        mo->hide();
        mo_g->hide();
    }


    //获取选中的用户信息
    QVariant var = index.data(Qt::UserRole+1);
    *friendInfo = var.value<UserItemData>();
    mo->load_friend_info(friendInfo);

    //    QString sUserName = friendInfo.sUserName;
    //    QString sUserID = "123";
    ui->chatName->setText(friendInfo->sUserName);
    //    ui->chatRoom->setText(sUserName);
    //    ui->chatRoom->setText(sUserID);
    //    ui->chatRoom->append(QString::asprintf("当前选中：第%d/%d行",nRow+1,nCount));
    //    ui->txt_name->setText(sUserName);
    //    ui->txt_uid->setText(sUserID);

    //    ui->txt_msg->setText(QString::asprintf("当前选中：第%d行",nRow+1));

    //添加信息
    if(!friendInfo->isBlocked){
        if(friendInfo->sMsgTpye == MsgTpye::AddMsg){
            ifAdd->setGeometry(400,40,490,53);
            ifAdd->show();
        }else{
            ifAdd->hide();
        }
    }

    //切换头像
    friendHeadIcon.clear();
    //    friendHeadIcon = headpics.at(nRow);
    //清空聊天界面
    ui->widget_bubble_show->clear();
}

//处理发送按钮


//拖拽文件
void OwlChatMain::dragEnterEvent(QDragEnterEvent *e)
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

void OwlChatMain::dropEvent(QDropEvent *e)
{
    QList<QUrl> urls = e->mimeData()->urls();

    if(urls.empty())
        return;

    for (int i=0; i < urls.size();i++)
    {
        QString fileName = urls[i].toLocalFile();
        QString extendName = fileName.right(4).toLower();
        QUrl Uri ( QString ( "file://%1" ).arg ( fileName ) );
        if(extendName.compare(".jpg")==0 || extendName.compare("jpeg")==0 || extendName.compare(".png")==0 || extendName.compare(".bmp")==0 || extendName.compare(".gif")==0)
        {
            //            图片
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

//键盘事件
void OwlChatMain::keyReleaseEvent(QKeyEvent *event)     // 按键释放事件
{
    if(event->key() == Qt::Key_Return){
        on_btnSend_clicked();
    }
    if(event->key() == Qt::Key_Enter){
        on_btnSend_clicked();
    }
}

//点击label
bool OwlChatMain::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)//mouse button pressed
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if(mouseEvent->button() == Qt::LeftButton)
        {
            if(obj == ui->chatName)
            {
                qDebug()<<"success!";
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return QObject::eventFilter(obj, event);
    }
}




void OwlChatMain::on_btnExpression_clicked()
{
    if(ep->isHidden()){
        ep->setGeometry(400,250,400,175);
        ep->show();
    }else{
        ep->hide();
    }
}

void OwlChatMain::on_btnFolder_clicked(QString fp)
{
    QString filePath = NULL;
    //    QString fp = ep->emojiName;
    if(NULL!=fp){
        filePath = fp;
    }else{
        filePath = QFileDialog::getOpenFileName(this,"open","../");
    }
    //    filePath = QFileDialog::getOpenFileName(this,"open","../");
    qDebug() << filePath;
    if(false == filePath.isEmpty())
    {
        fileName.clear();
        fileSize = 0;

        //获取文件信息
        QFileInfo info(filePath);
        fileName = info.fileName();
        fileSize = info.size();

        sendSize = 0;

        //只读方式打开
        file.setFileName(filePath);
        bool isOK = file.open(QIODevice::ReadOnly);
        if(false == isOK)
        {
            qDebug() << "只读方式打开文件失败";
        }

        //        ui->tedit_send_msg->append(filePath);

        //


        QString fileName = filePath;
        QString extendName = fileName.right(4).toLower();
        QUrl Uri ( QString ( "file://%1" ).arg ( fileName ) );
        if(extendName.compare(".jpg")==0 || extendName.compare("jpeg")==0 || extendName.compare(".png")==0 || extendName.compare(".bmp")==0 || extendName.compare(".gif")==0)
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



        //QString fileName = urls.first().toLocalFile();

        //    QTextDocumentFragment fragment;
        //    fragment = QTextDocumentFragment::fromHtml(QString("<img src='%1'>").arg(fileName));
        //    ui->tedit_send_msg->textCursor().insertFragment(fragment);


    }
}

void OwlChatMain::handle_expression()
{
    QString fileName = QString(":/images/emoji/%1").arg(ep->emojiName);

    QString extendName = fileName.right(4).toLower();
    QUrl Uri ( QString ( ":/images/emoji/image_emoticon.png"));
    if(extendName.compare(".jpg")==0 || extendName.compare("jpeg")==0 || extendName.compare(".png")==0 || extendName.compare(".bmp")==0 || extendName.compare(".gif")==0)
    {
        //图片
        QImage image = QImageReader ( ":/images/emoji/image_emoticon.png" ).read();
        QTextDocument * textDocument = ui->tedit_send_msg->document();
        textDocument->addResource( QTextDocument::ImageResource, Uri, QVariant ( image ) );
        m_sAcPicList.push_back(fileName);
    }
    on_btnSend_clicked();
    qDebug() << fileName;
}





void OwlChatMain::handle_add()
{
    QStandardItem *pItem = new QStandardItem;
    UserItemData ItemData = add->friendInfo;
    //    ItemData.sUserName = add->friendInfo;
    //    ItemData.sUserID = 1;
    //    ItemData.sUserStatus = Status::offline;
    //    ItemData.sHeadPic = headpics.at(2);
    pItem->setData(QVariant::fromValue(ItemData), Qt::UserRole+1);
    //    m_pModel->insertRow(0,pItem);
    //    m_pModel->removeRows(3,1);

    //    ItemData.sUserName = names.at(1);
    //    ItemData.sUserID = 1;
    //    ItemData.sHeadPic = headpics.at(1);
    //    ItemData.sMsgTpye = MsgTpye::AddMsg;
    //    pItem->setData(QVariant::fromValue(ItemData), Qt::UserRole+1);
    m_pModel->appendRow(pItem);
    qDebug() <<  m_pModel->indexFromItem(pItem);
    ui->listView->clearFocus();
    QStandardItemModel *newModel = new QStandardItemModel();
    ui->listView->setModel(newModel);
    ui->listView->setModel(m_pModel);
}

void OwlChatMain::on_btnChatList_clicked()
{
    selectIndex=-1;
    ui->btnMore->setEnabled(false);
    ui->btnExpression->setEnabled(false);
    ui->btnFolder->setEnabled(false);
    isChatList=true;
    changeColor(isChatList);
    mo->hide();
    ui->listView->clearFocus();
    QStandardItemModel *newModel = new QStandardItemModel();
    ui->listView->setModel(newModel);
    ui->listView->setModel(m_pModel);
}

void OwlChatMain::on_btnFriendsList_clicked()
{
    selectIndex=-1;
    ui->btnMore->setEnabled(false);
    ui->btnExpression->setEnabled(false);
    ui->btnFolder->setEnabled(false);
    isChatList=false;
    changeColor(isChatList);
    mo->hide();
    ui->listView->clearFocus();
    QStandardItemModel *newModel = new QStandardItemModel();
    ui->listView->setModel(newModel);
    ui->listView->setModel(m_pModel2);
}

void OwlChatMain::changeColor(bool isChatList)
{
    if(isChatList){
        ui->btnChatList->setStyleSheet(
                    "QPushButton"
                    "{"
                    "background-color:rgb(169,189,35);"
                    "image: url(:/images/friend.png);"
                    "}"
                    "QPushButton:hover"
                    "{"
                    "background-color:rgb(169,189,35);"
                    "}");
        ui->btnFriendsList->setStyleSheet(
                    "QPushButton"
                    "{"
                    "background-color:rgb(250,250,250);"
                    "image: url(:/images/group.png);"
                    "}"
                    "QPushButton:hover"
                    "{"
                    "background-color:rgb(169,189,35);"
                    "}");
    }else{
        ui->btnChatList->setStyleSheet(
                    "QPushButton"
                    "{"
                    "background-color:rgb(250,250,250);"
                    "image: url(:/images/friend.png);"
                    "}"
                    "QPushButton:hover"
                    "{"
                    "background-color:rgb(169,189,35);"
                    "}");
        ui->btnFriendsList->setStyleSheet(
                    "QPushButton"
                    "{"
                    "background-color:rgb(169,189,35);"
                    "image: url(:/images/group.png);"
                    "}"
                    "QPushButton:hover"
                    "{"
                    "background-color:rgb(169,189,35);"
                    "}"
                    );
    }
}

void OwlChatMain::on_pushButton_2_clicked()
{
    qDebug() << "___";
    qDebug() << m_sAcPicList;
    qDebug() << m_sAcFileList;
}
void OwlChatMain::on_btnAddFriends_clicked()
{
    if(add->isHidden()){
        add->setGeometry(50,40,350,300);
        add->show();
    }else{
        add->hide();
    }
}

void OwlChatMain::on_btnMore_clicked()
{
    add->hide();
    mo_g->on_btnEdit_clicked_2();

    if(friendInfo->sMsgTpye == MsgTpye::Single){
        mo->change_dialog_type(isChatList);
        mo->change_blacklist(friendInfo->isBlocked);
        if(mo->isHidden()){
            mo->setGeometry(542,90,350,320);
            mo->show();
        }else{
            mo->hide();
        }
    }else if(friendInfo->sMsgTpye == MsgTpye::Group){
        mo_g->setModal(m_pModel);
        if(mo_g->isHidden()){
            mo_g->setGeometry(542,90,350,320);
            mo_g->show();
        }else{
            mo_g->hide();
        }
    }

}

void OwlChatMain::on_btnHeadIcon_clicked()
{
    pm->on_btnInit();
    if(pm->isHidden()){
        pm->setGeometry(50,40,350,595);
        pm->show();
    }else{
        pm->hide();
    }
    add->hide();
}

void OwlChatMain::receive_from_remote(ItemInfo *item)
{
    //模拟收信息
    item->SetName("bluesky");
    item->SetDatetime(DATE_TIME);
    item->SetHeadPixmap(friendHeadIcon);
    //QString sMsg = QString("Text msg num=%1,Picture num=%2,Files num=%3").arg(textList.size()-nEmptyNum).arg(m_sAcPicList.size()).arg(m_sAcFileList.size());
    item->SetOrientation(Right);
    ui->widget_bubble_show->addItem(item);
}

void OwlChatMain::handle_delete_dialog()
{
    ifAdd->hide();
    m_pModel->removeRow(selectIndex);
    if(isChatList){
        ui->listView->clearFocus();
        QStandardItemModel *newModel = new QStandardItemModel();
        ui->listView->setModel(newModel);
        ui->listView->setModel(m_pModel);
    }
    mo->hide();
}

void OwlChatMain::handle_delete_friend()
{
    ifAdd->hide();
}

void OwlChatMain::handle_add_to_blacklist()
{
    QStandardItem *pItem = new QStandardItem;

    friendInfo->isBlocked =!friendInfo->isBlocked;
    ifAdd->hide();
    //    selectIndex=-1;
    mo->load_friend_info(friendInfo);
    mo->change_blacklist(friendInfo->isBlocked);
    pItem->setData(QVariant::fromValue(*friendInfo), Qt::UserRole+1);


    if(isChatList){
        m_pModel->removeRow(selectIndex);
        m_pModel->appendRow(pItem);
        ui->listView->clearFocus();
        QStandardItemModel *newModel = new QStandardItemModel();
        ui->listView->setModel(newModel);
        ui->listView->setModel(m_pModel);
    }else{
        m_pModel2->removeRow(selectIndex);
        m_pModel2->appendRow(pItem);
        ui->listView->clearFocus();
        QStandardItemModel *newModel = new QStandardItemModel();
        ui->listView->setModel(newModel);
        ui->listView->setModel(m_pModel2);
    }

    //处理按钮
    ui->btnMore->setEnabled(false);
    ui->btnExpression->setEnabled(false);
    ui->btnFolder->setEnabled(false);
    selectIndex=-1;
}

void OwlChatMain::handle_chang_my_status()
{
    switch (pm->myStatus) {
    case 0:{
        myInfo->sUserStatus = Status::online;
        ui->statusPoint->setStyleSheet("QLabel"
                                       "{"
                                       "background-color: rgb(44,221,113);"
                                       "border-radius :8px;"
                                       "}");
        break;
    }

    case 1 :{
        myInfo->sUserStatus = Status::hide;
        ui->statusPoint->setStyleSheet("QLabel"
                                       "{"
                                       "background-color: rgb(64,66,68);"
                                       "border-radius :8px;"
                                       "}");
        break;
    }
    case 2:{
        myInfo->sUserStatus = Status::offline;
        ui->statusPoint->setStyleSheet("QLabel"
                                       "{"
                                       "background-color: rgb(221,0,27);"
                                       "border-radius :8px;"
                                       "}");
        break;
    }
    default:
        break;
    }
}

void OwlChatMain::on_btnSend_clicked()
{
    QString text = ui->tedit_send_msg->toPlainText();
    text = text.simplified();
    text.replace("\n", "");
    QChar qcSlit = (QChar)(0xfffc);
    QStringList textList = text.split(qcSlit);//拆分

    // 把最后一个回车换行符删掉
    while (text.endsWith("\n"))
    {
        text.remove(text.length() - 1, 1);
    }

    // 判断消息是否为空
    //    if (text.isEmpty()) {
    //        QPoint point = ui->widgetMsg->mapToGlobal(ui->btn_sendmsg->pos());
    //        QToolTip::showText(point, tr("发送消息内容不能为空，请重新输入！"));
    //        return;
    //    }

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
        if((dMinute - 0.03) > 0.000001)
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
        picitemInfo->SetHeadPixmap(myHeadIcon);
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
        FileitemInfo->SetHeadPixmap(myHeadIcon);

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
        itemInfo->SetHeadPixmap(myHeadIcon);
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
        itemInfo2->SetHeadPixmap(friendHeadIcon);

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


void OwlChatMain::handle_add_new_friend()
{
    UserItemData info = mo_g->friendInfo;
    add->load_friend_info(info);
    add->show();

}

void OwlChatMain::handle_delete_dialog_group()
{
    ifAdd->hide();
    add->hide();
    m_pModel->removeRow(selectIndex);
    if(isChatList){
        ui->listView->clearFocus();
        QStandardItemModel *newModel = new QStandardItemModel();
        ui->listView->setModel(newModel);
        ui->listView->setModel(m_pModel);
    }
    mo_g->hide();
}

void OwlChatMain::handle_quit_group()
{
    add->hide();
}

void OwlChatMain::handle_add_member()
{
    //    on_btnFriendsList_clicked();
    mo_g->load_users(m_pModel2);
    qDebug() << "add_member";
}

void OwlChatMain::handle_delete_member()
{
    qDebug() << "delete_member";
}

void OwlChatMain::handle_delete_all()
{
    qDebug() << "delete_all";
}

void OwlChatMain::setMyID(uint8_t myID_2)
{
myID = myID_2;
}
//网络
void OwlChatMain::onLoginCallback(uint8_t s)
{

}

void OwlChatMain::onRegisterCallback(uint8_t s)
{

}

void OwlChatMain::sendMessageCallback(uint8_t s, uint32_t msgID)
{

}

void OwlChatMain::recvMessageCallback(uint32_t fromUserID, uint32_t sessionID, uint64_t time, uint32_t msgID, uint8_t msg_type, QString content)
{

}

void OwlChatMain::getFriendListCallback(QList<D_UserBasicInfo> infolist)  // Caution list might be zero
{

}

void OwlChatMain::getUserInfoCallback(D_UserBasicInfo info)
{

}

void OwlChatMain::getUserDetailCallback(D_UserDetailInfo info)
{

}

void OwlChatMain::onFriendAddCallback(uint8_t s, uint32_t userID_client)
{

}

void OwlChatMain::onFriendDeleteCallback(uint8_t s, uint32_t userID_client)
{

}

//@回应好友请求回调
void OwlChatMain::onFriendAcceptCallback(uint8_t s, uint32_t userID_client)
{

}

//@收到好友请求回调
void OwlChatMain::onFriendRequestCallback(uint32_t fromUserID, QString verify_msg)
{

}

//@收到对方回执回调
void OwlChatMain::onFriendResultCallback(uint32_t userID_client, bool isAccepted)
{

}

void OwlChatMain::onGroupCreateCallback(uint8_t s1, uint32_t s2)
{

}

void OwlChatMain::onGroupAddCallback(uint8_t s)
{

}
