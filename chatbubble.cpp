#include "chatbubble.h"
#include <QDebug>

#include "global.h"
//#include "myapp.h"

#include <QFileInfo>
#include <QFileIconProvider>
#include <QDesktopServices>
#include <QClipboard>
#include <QLabel>
#include <QMovie>

#include "ItemInfo.h"

BubbleList::BubbleList(QWidget *parent) :
    QWidget(parent)
{
    initVars();
    initWgts();
    initStgs();
    initConns();
}

BubbleList::~BubbleList()
{
    d->clear();
}

/*!
  private utility functions
*/
void BubbleList::initVars()
{
}

void BubbleList::initWgts()
{
    mainLayout = new QHBoxLayout(this);
    scrollbar = new QScrollBar(this);
    d = new BubbleListPrivate(this);
    //    this->setMinimumWidth(300);

    mainLayout->addWidget(d);
    //mainLayout->addWidget(m_pSlider);
    mainLayout->addWidget(scrollbar);
    setLayout(mainLayout);
}

void BubbleList::initStgs()
{
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    scrollbar->setStyleSheet("QScrollBar:vertical"
                             "{"
                             "width:8px;"
                             "background:rgba(0,0,0,0%);"
                             "margin:0px,0px,0px,0px;"
                             "padding-top:9px;"
                             "padding-bottom:9px;"
                             "}"
                             "QScrollBar::handle:vertical"
                             "{"
                             "width:8px;"
                             "background:rgba(0,0,0,25%);"
                             " border-radius:4px;"
                             "min-height:20;"
                             "}"
                             "QScrollBar::handle:vertical:hover"
                             "{"
                             "width:8px;"
                             "background:rgba(0,0,0,50%);"
                             " border-radius:4px;"
                             "min-height:20;"
                             "}"
                             "QScrollBar::add-line:vertical"
                             "{"
                             "height:9px;width:8px;"
                             "border-image:url(:/images/a/3.png);"
                             "subcontrol-position:bottom;"
                             "}"
                             "QScrollBar::sub-line:vertical"
                             "{"
                             "height:9px;width:8px;"
                             "border-image:url(:/images/a/1.png);"
                             "subcontrol-position:top;"
                             "}"
                             "QScrollBar::add-line:vertical:hover"
                             "{"
                             "height:9px;width:8px;"
                             "border-image:url(:/images/a/4.png);"
                             "subcontrol-position:bottom;"
                             "}"
                             "QScrollBar::sub-line:vertical:hover"
                             "{"
                             "height:9px;width:8px;"
                             "border-image:url(:/images/a/2.png);"
                             "subcontrol-position:top;"
                             "}"
                             "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                             "{"
                             "background:rgba(0,0,0,10%);"
                             "border-radius:4px;"
                             "}"
                             );


    scrollbar->setRange(0,d->itemCount());
    scrollbar->setValue(scrollbar->maximum());
    scrollbar->setOrientation(Qt::Orientation::Vertical);
    scrollbar->show();

    //    scrollbar->hide();
}

void BubbleList::initConns()
{
    connect(scrollbar,SIGNAL(valueChanged(int)),d,SLOT(setCurrentIndex(int)));
    connect(d,SIGNAL(sig_setMaximum(int)),this,SLOT(setMaximum(int)));
    connect(d,SIGNAL(sig_setCurrentIndex(int)),scrollbar,SLOT(setValue(int)));
    connect(d,SIGNAL(sig_itemClicked(QString)),this,SIGNAL(sig_itemClicked(QString)));
    connect(d,SIGNAL(signalDownloadFile(QString)),this,SIGNAL(signalDownloadFile(QString)));
}

void BubbleList::calcGeo()
{

}

/*!
  private slot functions
*/
void BubbleList::setMaximum(int max)
{
    if(max == 0)
    {
        scrollbar->hide();
    }else{
        scrollbar->show();
    }
    scrollbar->setRange(0,max);
}

/*!
  protected functions of PYFlashList
*/
void BubbleList::resizeEvent(QResizeEvent *)
{

}

/*!
  public interfaces of PYFlashList
*/
void BubbleList::addItem(ItemInfo *item)
{
    d->addItem(item);
    scrollbar->setRange(0,d->itemCount());
    scrollbar->setValue(d->itemCount());
}

void BubbleList::addItems(QVector<ItemInfo *> items)
{
    if (items.size() < 1) return;
    d->addItems(items);
}

void BubbleList::clear()
{
    d->clear();
}

void BubbleList::render()
{
    d->render();
}

/*************************************/
/*!   BubbleListPrivate section    !*/
/*************************************/
BubbleListPrivate::BubbleListPrivate(QWidget* parent):QWidget(parent)
{
    initVars();
    initSettings();
}
/*!
  private utility functions of BubbleListPrivate
 */
void BubbleListPrivate::initVars()
{
    m_currIndex = 0;
    m_VisibleItemCnt = 0;
    m_ItemCounter = 0;

    m_bAllJobsDone = false;
    m_bHover       =  false;

    m_font = QFont("黑体", 12);

    // 右键菜单
    textRightButtonMenu = new QMenu(this);
    //    textRightButtonMenu->addAction("保存图片");
    //    textRightButtonMenu->addSeparator();
    textRightButtonMenu->addAction("复制文本");
    textRightButtonMenu->hide();
    connect(textRightButtonMenu, SIGNAL(triggered(QAction*)), this, SLOT(SltTextMenuClicked(QAction*)));

    // 右键菜单
    picRightButtonMenu = new QMenu(this);
    picRightButtonMenu->addAction("保存图片");
    picRightButtonMenu->addSeparator();
    picRightButtonMenu->addAction("复制到粘贴板");
    picRightButtonMenu->hide();

    // 右键菜单
    fileRightButtonMenu = new QMenu(this);
    //    fileRightButtonMenu->addAction("下载文件");
    //    fileRightButtonMenu->addSeparator();
    fileRightButtonMenu->addAction("打开文件");
    fileRightButtonMenu->addAction("打开文件目录");
    fileRightButtonMenu->hide();

    connect(fileRightButtonMenu, SIGNAL(triggered(QAction*)), this, SLOT(SltFileMenuClicked(QAction*)));
}

void BubbleListPrivate::initSettings()
{
    setMouseTracking(true);
}

void BubbleListPrivate::calcGeo()
{
#if 0
    // 计算当前可以刷新的item个数
    qDebug() << "calc Geo" << m_currIndex;
    int nCnt = 0;
    int nHeight = 0;
    int nCurrIndex = m_currIndex;

    while (nCurrIndex > 1) {
        nHeight = m_IIVec.at(nCurrIndex)->getItemHeight();
        nHeight += ITEM_SPACE;
        if (nHeight + 20 >= this->height()) break;
        nCnt++;
        nCurrIndex--;
    }

    m_VisibleItemCnt = nCnt;
    //    m_currIndex = nCurrIndex - nCnt;
#else
    m_VisibleItemCnt = height() / (ITEM_HEIGHT + ITEM_SPACE + 10) + 1;
#endif

    int InvisibleItemCnt = m_IIVec.count() - m_VisibleItemCnt;

    if (InvisibleItemCnt >= 0)
    {
        emit sig_setMaximum(InvisibleItemCnt);
    }
}

//滚轮
void BubbleListPrivate::wheelUp()
{
//    if (m_currIndex > m_IIVec.size()) {
//        m_currIndex = m_IIVec.size()-2;
//    }
//    m_currIndex++;
    if(m_currIndex < m_IIVec.size()-2){
        m_currIndex++;
    }else{
        if(m_IIVec.size()>=2){
            m_currIndex = m_IIVec.size()-2;
        }else if(m_IIVec.size() == 1){
            m_currIndex = m_IIVec.size()-1;
        }else{
            m_currIndex++;
        }
    }
    update();
    emit sig_setCurrentIndex(itemCount() - m_currIndex);
}

void BubbleListPrivate::wheelDown()
{


    // 优化处理，在没添加的时候下滑动会崩溃
//    m_currIndex=1;
    if (m_currIndex < 0)
    {
        m_currIndex = 0;
    }
    m_currIndex--;
    if (m_currIndex < 0)
    {
        m_currIndex = 0;
    }
    update();
    emit sig_setCurrentIndex(itemCount() - m_currIndex);
}

/**
 * @brief BubbleListPrivate::SltFileMenuClicked
 * @param acction
 */
void BubbleListPrivate::SltFileMenuClicked(QAction *action)
{
    QString strText = m_IIVec.at(m_selectedIndex)->GetText();
    QFileInfo fileInfo(strText);

    // 菜单操作
    if (action->text() == "下载文件")
    {
        // 如果文件存在，直接打开目录
        if (QFile::exists(strText)) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absolutePath()));
            return;
        }

        // 获取文件拓展名，因为服务器下载文件只需要名字即可
        Q_EMIT signalDownloadFile(fileInfo.fileName());
    }
    else if (action->text() == "打开文件") {
        // 如果文件存在，直接打开目录
        if (QFile::exists(strText)) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(strText));
            return;
        }
    }
    else if (action->text() == "打开文件目录") {
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absolutePath()));
    }
}

void BubbleListPrivate::SltTextMenuClicked(QAction *action)
{
    QString strText = m_IIVec.at(m_selectedIndex)->GetText();
    // 菜单操作
    if (action->text() == "复制文本")
    {
        QClipboard *clipboard = QApplication::clipboard();   //获取系统剪贴板指针
        clipboard->setText(strText);		             //设置剪贴板内容
    }
}

/*!
  protected functions of BubbleListPrivate
*/
void BubbleListPrivate::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::HighQualityAntialiasing|QPainter::Antialiasing);//抗锯齿
    drawBackground(&painter);//绘制背景：气泡展示区的背景
    drawItems(&painter);//绘制ITEM
}

void BubbleListPrivate::mouseMoveEvent(QMouseEvent *e)
{
    int nItemY = ITEM_SPACE;
    if(m_currIndex < 0)
    {
        m_currIndex=0;
    }
    for (int nIndex = m_currIndex; nIndex < m_IIVec.count(); nIndex++)
    {
        if (nItemY > this->height()) {
            break;
        }

        int nY = this->height() - nItemY;
        QRectF bubbleRect = m_IIVec.at(nIndex)->GetBobbleRect();
        if ((e->pos().y() < (nY) && (e->pos().y() > (nY - bubbleRect.height()))) &&
                ((e->pos().x() > bubbleRect.x()) &&
                 (e->pos().x() < (bubbleRect.x() + bubbleRect.width()))))
        {
            quint8 nType = m_IIVec.at(nIndex)->GetMsgType();

            // 如果是图片或文件，可以直接打开
            if (Picture == nType || Files == nType) {
                this->setCursor(Qt::PointingHandCursor);
                m_bHover = true;
                m_nHoverItemIndex = nIndex;
                update();
                return;
            }
        }

        // 如果有改变，需要改变回来
        if (m_bHover) {
            m_bHover = false;
            m_nHoverItemIndex = -1;
            update();
            return;
        }
        nItemY += bubbleRect.height() + ITEM_SPACE;
    }

    this->setCursor(Qt::ArrowCursor);
}

/**
 * @brief BubbleListPrivate::mousePressEvent
 * 鼠标右键按下
 * @param e
 */
void BubbleListPrivate::mousePressEvent(QMouseEvent *e)
{
    if (Qt::RightButton == e->button()) {
        int nItemY = ITEM_SPACE;
        for (int nIndex = m_currIndex; nIndex < m_IIVec.count(); nIndex++)
        {
            if (nItemY > this->height()) {
                break;
            }


            int nY = this->height()-nItemY; ;
            int nRight = 0;
            if(m_IIVec.at(nIndex)->GetOrientation() == Right)
            {
                nRight = 20;
            }
            int npos = e->pos().y();

            QRectF bubbleRect = m_IIVec.at(nIndex)->GetBobbleRect();

            if (((e->pos().y() < (nY)) &&
                 (e->pos().y()+nRight > (nY - bubbleRect.height()))
                 )
                    &&
                    ((e->pos().x() > bubbleRect.x()) &&
                     (e->pos().x() < (bubbleRect.x() + bubbleRect.width()))
                     )
                    )
            {
                // 当前选中item
                m_selectedIndex = nIndex;

                // 如果是图片或文件，可以直接打开
                if (Picture == m_IIVec.at(nIndex)->GetMsgType()) {
                    picRightButtonMenu->popup(e->globalPos());
                    return;
                }
                else if (Files == m_IIVec.at(nIndex)->GetMsgType()) {
                    fileRightButtonMenu->popup(e->globalPos());
                    return;
                }
                else if (Text == m_IIVec.at(nIndex)->GetMsgType()) {
                    textRightButtonMenu->popup(e->globalPos());
                    return;
                }
            }
            nItemY += bubbleRect.height() + ITEM_SPACE;
        }
    }
}

/**
 * @brief BubbleListPrivate::mouseDoubleClickEvent
 * 鼠标双击item
 * @param e
 */
void BubbleListPrivate::mouseDoubleClickEvent(QMouseEvent *e)
{
    int nItemY = ITEM_SPACE;
    for (int nIndex = m_currIndex; nIndex < m_IIVec.count(); nIndex++)
    {
        if (nItemY > this->height()) {
            break;
        }

        int nY = this->height() - nItemY;
        QRectF bubbleRect = m_IIVec.at(nIndex)->GetBobbleRect();
        if ((e->pos().y() < (nY) && (e->pos().y() > (nY - bubbleRect.height()))) &&
                ((e->pos().x() > bubbleRect.x()) &&
                 (e->pos().x() < (bubbleRect.x() + bubbleRect.width()))))
        {
            // 如果是图片或文件，可以直接打开
            if (Picture == m_IIVec.at(nIndex)->GetMsgType() /*|| Files == m_IIVec.at(nIndex)->GetMsgType()*/)
            {
                QString strFile = m_IIVec.at(nIndex)->GetText();
                // 如果文件存在，可以打开
                if (QFile::exists(strFile)) {
                    QDesktopServices::openUrl(QUrl::fromLocalFile(strFile));
                }
                return;
            }

            Q_EMIT sig_itemClicked(m_IIVec.at(nIndex)->GetText());
            break;
        }
        nItemY += bubbleRect.height() + ITEM_SPACE;
    }
}

// resize
void BubbleListPrivate::resizeEvent(QResizeEvent *)
{
    calcGeo();
}

void BubbleListPrivate::leaveEvent(QEvent *)
{
    m_HoverRect = QRectF();

    //    update();
}
void BubbleListPrivate::showEvent(QShowEvent *)
{
    calcGeo();
}

void BubbleListPrivate::wheelEvent(QWheelEvent *e)
{
    if(e->delta() > 0)
    {
        wheelUp();
    }else{
        wheelDown();

    }
}

//绘制背景：气泡展示区的背景
void BubbleListPrivate::drawBackground(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);
#if 0
    QLinearGradient BgGradient(QPoint(0,0),QPoint(0,height()));
    BgGradient.setColorAt(0.0, QColor(64,65,66));
    BgGradient.setColorAt(1.0, QColor(64,65,66));
    painter->setBrush(BgGradient);
#else
    //背景颜色
    painter->setBrush(QBrush(QColor(245, 245, 245)));
#endif
    painter->drawRect(rect());
    painter->restore();
}

//绘制ITEM
void BubbleListPrivate::drawItems(QPainter *painter)
{
    if(m_IIVec.count() == 0)
    {
        return ;
    }

    // 绘制item区域
    painter->save();

    qreal nItemY = 0;
    int nWidth = this->width();
    nWidth = (0 == nWidth % 2) ? nWidth : nWidth + 1;

    // 起始位置抬高
    nItemY = ITEM_SPACE + ITEM_TITLE_HEIGHT;

    for (int nIndex = m_currIndex; nIndex < m_IIVec.count(); nIndex++)
    {
        if (nItemY > this->height()) {
            break;
        }

        // 解析消息内容
        QString strMsg = m_IIVec.at(nIndex)->GetText();
        quint8 msgType = m_IIVec.at(nIndex)->GetMsgType();

        quint8 nOrientation = m_IIVec.at(nIndex)->GetOrientation();
        QString strPixmap   = m_IIVec.at(nIndex)->GetStrPixmap();

        qreal bubbleWidth = 0;
        qreal bubbleHeight = 0;
        // 计算右边的宽度x
        int nX = 0;
        int nY = 0;

        QPixmap pixmap;
        QPainterPath path;
        // 气泡和内容的矩形框
        QRectF msgRect   ;
        QRectF bobbleRect;
        QRectF TimeRect;

        // --------------------计算气泡和文字的绘制范围 ----------------------------//
        switch (msgType) {
        case Text:
        {
            // 计算文字的宽度
            QFontMetrics fm(m_font);

            bubbleWidth = fm.width(strMsg);
            bubbleHeight = fm.height();

            QStringList strMsgList = strMsg.split("\n");
            qreal nLineWidth = 0;
            qreal nLineHeight = 0;

            // 换行符会影响计算宽度和高度
            for (int i = 0; i < strMsgList.size(); i++) {
                if (nLineWidth < fm.width(strMsgList.at(i)))
                {
                    nLineWidth = fm.width(strMsgList.at(i));
                    nLineWidth = nLineWidth < (nWidth * 2 / 3) ? nLineWidth: (nWidth * 2 / 3);
                }

                double dLineNum =  bubbleWidth/nLineWidth;// (((nLineWidth / ((nWidth * 2 / 3))) + 1) * ITEM_HEIGHT / 3);
                int nLineNum = (int)dLineNum;
                int nLineHeightTemp = nLineNum * (fm.height() * 1.25);
                nLineHeight = nLineWidth < (nWidth * 2 / 3) ? fm.height() * 1.15 : nLineHeightTemp;
                bubbleHeight += nLineHeight;
            }

            bubbleWidth = nLineWidth;
#if 0
            //            bubbleHeight = bubbleWidth < (nWidth * 2 / 3) ? ITEM_HEIGHT : (((bubbleWidth / ((nWidth * 2 / 3))) + 1) * ITEM_HEIGHT / 2);
            //            bubbleWidth = bubbleWidth < (nWidth * 2 / 3) ? bubbleWidth: (nWidth * 2 / 3);
            //            bubbleWidth = bubbleWidth < 30 ? 30 : bubbleWidth;
#endif

            // 文字初始化高度
            nY = this->height() - nItemY - bubbleHeight;
        }
            break;
        case Audio:

            break;
        case DateTime:
        {
            nY = this->height() - nItemY - 20;
        }
            break;
        case Picture:
        {
            pixmap = QPixmap(strMsg);
            if (pixmap.isNull()) {
                pixmap = QPixmap(":/resource/background/ic_picture.png");
            }
            // 图片过大限制
            if (pixmap.width() > 200 || pixmap.height() > 100)
            {
                int nSmall=pixmap.width()/200;
                pixmap = pixmap.scaled(pixmap.width()/nSmall, pixmap.height()/nSmall);
            }

            bubbleWidth = pixmap.width();
            bubbleHeight = pixmap.height() + 10;

            // 文字初始化高度
            nY = this->height() - nItemY - bubbleHeight;

        }
            break;
        case Files:
        {
            QFileInfo fileInfo(strMsg);
            QString strSize = m_IIVec.at(nIndex)->GetFileSizeString();

            // 文件图标

            pixmap = QPixmap(":/resource/images/ic_zip.png");

            // 先判断文件是否存在
            if (QFile::exists(strMsg)) {
                strSize = "文件大小: ";
                strSize += myHelper::CalcSize(fileInfo.size());
#ifdef Q_OS_WIN32
                QFileIconProvider provider;
                QIcon icon = provider.icon(fileInfo);
                pixmap = icon.pixmap(48, 48);
#endif
            }
            else {
                strSize = "文件未下载 ";
            }

            // 计算文字的宽度
            QFont font("微软雅黑", 10);
            QFontMetrics fm(font);
            // 显示文件名，路径不显示
            strMsg = fileInfo.fileName();
            // 计算气泡的size
            if (fm.width(strMsg) < fm.width(strSize)) {
                bubbleWidth = fm.width(strSize) + pixmap.width() + 20;
            }
            else {
                bubbleWidth = fm.width(strMsg) + pixmap.width() + 20;
            }

            bubbleHeight = 60;

            bubbleWidth = bubbleWidth < (nWidth * 2 / 3) ? bubbleWidth: (nWidth * 2 / 3);
            bubbleWidth = bubbleWidth < 60 ? 60 : bubbleWidth;

            strMsg += "\n";
            strMsg += strSize;

            // 文字初始化高度
            nY = this->height() - nItemY - bubbleHeight;
        }
            break;
        default:
            break;
        }

        // --------------------绘制气泡和文字 ----------------------------//
        if (Right == nOrientation)//我的气泡
        {
            painter->save();
            painter->setPen(QPen(QColor("#D3D3D3"), 1));
            // 绘制边框
            painter->drawRoundedRect(nWidth - 55, nY, 50, 50, 2, 2);
            // 绘制头像
            QPixmap pixmap(strPixmap);
            if (!pixmap.isNull()) {
                painter->drawPixmap(nWidth - 55 + 1, nY + 1, 48, 48, pixmap);
            }
            painter->restore();

            // 绘制名字
            painter->save();
            // 计算文字的宽度
            QFont font("微软雅黑", 8);
            QFontMetrics fm(font);
            QString strTitle = m_IIVec.at(nIndex)->GetName();

            painter->setPen(QColor("#666666"));
            painter->setFont(font);
            painter->drawText(nWidth - 55 - fm.width(strTitle) - 20, nY + fm.height() / 2, strTitle);
            painter->restore();

            // 根据消息内容计算气泡区域
            nX = (nWidth ) - 95 - bubbleWidth;
            if (nX < 0) {
                nX = -bubbleWidth - 95 + nWidth ;
            }

            // 计算气泡框
            nY += ITEM_TITLE_HEIGHT;
            bobbleRect = QRectF(nX, nY, bubbleWidth + 20, bubbleHeight);
            msgRect    = QRectF(nX + 10, nY, bubbleWidth, bubbleHeight);

            painter->save();
            // 计算气泡右边的三角
            path.addRoundedRect(bobbleRect, 10, 10);
            path.moveTo(nWidth - 75, nY + 15);
            path.lineTo(nWidth - 65, nY + 20);
            path.lineTo(nWidth - 75, nY + 25);
            //            painter->fillPath(path, itemGradient);
            painter->setPen(Qt::red);
            painter->fillPath(path, QColor(158,230,86));
            painter->restore();
        }
        else if (Left == nOrientation)//对方的气泡
        {
            // 绘制头像
            painter->save();
            painter->setPen(QColor("#D3D3D3"));
            // 绘制边框
            painter->drawRoundedRect(5, nY, 50, 50, 2, 2);

            // 绘制头像
            QPixmap pixmap(strPixmap);
            if (!pixmap.isNull()) {
                painter->drawPixmap(6, nY + 1, 48, 48, pixmap);
            }

            painter->restore();

            nX = 75;
            // 绘制名字
            painter->save();
            // 计算文字的宽度
            QFont font("微软雅黑", 8);
            QFontMetrics fm(font);

            QString strTitle = m_IIVec.at(nIndex)->GetName();// + " ";
            //strTitle += m_IIVec.at(nIndex)->GetDatetime();

            painter->setPen(QColor("#666666"));
            painter->setFont(font);
            painter->drawText(nX, nY + fm.height() / 2, strTitle);
            painter->restore();

            // 绘制气泡区域
            painter->save();
            nY += ITEM_TITLE_HEIGHT;
            bobbleRect = QRectF(nX, nY, bubbleWidth + 20, bubbleHeight);
            // 绘制文字区域
            msgRect = QRectF(nX + 10, nY, bubbleWidth, bubbleHeight);

            // 左边三角
            path.addRoundedRect(bobbleRect, 5, 5);
            path.moveTo(nX, nY + 15);
            path.lineTo(nX - 10, nY + 20);
            path.lineTo(nX, nY + 25);
            // 背景色
            painter->fillPath(path, QColor(255,255,255));
            painter->restore();
        }
        else
        {
            //绘制当前时间
            painter->save();
            // 计算文字的宽度
            QFont font("微软雅黑", 8);
            QFontMetrics fm(font);
            QString strTitle = m_IIVec.at(nIndex)->GetDatetime();

            nY += ITEM_TITLE_HEIGHT;
            TimeRect = QRect(0, nY, nWidth, 20);

            painter->setPen(QColor("#666666"));
            painter->setFont(font);
            painter->drawText(TimeRect, Qt::AlignCenter, strTitle);
            painter->restore();
        }

        painter->setPen(QColor("#000000"));
        painter->setFont(m_font);

        if (Text == msgType) {
            painter->drawText(msgRect, strMsg, Qt::AlignVCenter | Qt::AlignLeft);
        }
        else if (Picture == msgType) {
            //            QMovie *movie = new QMovie("d:/01.gif");
            //            for (int i=0;i<movie->frameCount();i++)
            //            {
            //                movie->jumpToFrame(i);
            //                QImage image = movie->currentImage();
            //                //painter->drawImage(nX + 10, nY + 5,pixmap.width(), pixmap.height(),&image);
            //                //myHelper::Sleep(1);
            //                painter->drawPixmap(nX + 10, nY + 5, pixmap.width(), pixmap.height(), QPixmap::fromImage(image));
            //            }

            painter->drawPixmap(nX + 10, nY + 5, pixmap.width(), pixmap.height(), pixmap);

        }
        else if (DateTime == msgType) {
            //painter->drawPixmap(nX + 10, nY + 5, pixmap.width(), pixmap.height(), pixmap);
        }
        else if (Files == msgType) {
            painter->drawPixmap(nX + 10, nY + 5 + (48 - pixmap.height()) / 2, pixmap.width(), pixmap.height(), pixmap);
            msgRect.setX(msgRect.x() + pixmap.width() + 15);
            painter->save();
            painter->setPen(Qt::blue);
            QFont font("微软雅黑", 10);
            font.setUnderline(m_bHover && (nIndex == m_nHoverItemIndex));
            painter->setFont(font);
            painter->drawText(msgRect, strMsg, Qt::AlignVCenter | Qt::AlignLeft);
            painter->restore();
        }

        m_IIVec.at(nIndex)->SetBobbleRect(bobbleRect);
        nItemY += (bobbleRect.height()) + ITEM_SPACE + ITEM_TITLE_HEIGHT;
    }
    painter->restore();
}


/*!
  public interfaces of BubbleListPrivate
 */
void BubbleListPrivate::addItem(ItemInfo *item)
{
    m_IIVec.push_front(item);
    m_currIndex = 0;
    update();
}

void BubbleListPrivate::addItems(QVector<ItemInfo *> items)
{
    m_IIVec.clear();
    m_IIVec = items;
    m_currIndex = 0;
    update();
}

void BubbleListPrivate::clear()
{
    m_IIVec.clear();
    m_currIndex = 0;
    m_VisibleItemCnt = 0;
    m_ItemCounter = 0;

    m_bAllJobsDone = false;

    update();
}

void BubbleListPrivate::render()
{
    update();
}

/*!
  public slot functions of BubbleListPrivate
*/
void BubbleListPrivate::setCurrentIndex(int curIndex)
{
    if(curIndex == m_currIndex)
    {
        return ;
    }

    //
    m_currIndex = m_IIVec.size() - curIndex;
    if((m_IIVec.size()-1) < itemCount())
    {
        m_currIndex = itemCount() - curIndex ;
//        m_currIndex = 0;
    }

    if (m_currIndex > m_IIVec.size()) {
        m_currIndex = m_IIVec.size() - 1;
    }
    if (m_currIndex < 0)
    {
        m_currIndex = 0;
    }

    update();
    //emit sig_setCurrentIndex(m_currIndex);
}
