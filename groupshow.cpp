#include "groupshow.h"
#include "ui_groupshow.h"
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

GroupShow::GroupShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GroupShow)
{
    ui->setupUi(this);
     setWindowFlags(Qt::FramelessWindowHint);
     ifEdit = true;
}

GroupShow::~GroupShow()
{
    delete ui;
}

void GroupShow::load_users(QStandardItemModel *Model)
{
    ui->listView->setModel(Model);

}

void GroupShow::init_fun(UserItemDelegate *pUserItemDelegate)
{
    ui->listView->setItemDelegate(pUserItemDelegate);
}

void GroupShow::on_btnDelete_clicked()
{
    hide();
    emit delete_dialog();
}

void GroupShow::on_btnQuit_clicked()
{
    hide();
    emit quit_group();
}

void GroupShow::on_btnAdd_clicked()
{
    emit add_new_friend();
}

int GroupShow::index()
{
//   return ui->listView->selectedIndexes();
}

//处理列表数据

void GroupShow::on_listView_clicked(const QModelIndex &index)
{
    int nRow = index.row();
    selectIndex = index.row();

    //获取选中的用户信息
    QVariant var = index.data(Qt::UserRole+1);
    friendInfo = var.value<UserItemData>();

    ui->nameLabel->setText( friendInfo.sUserName);
    ui->idLabel->setText(QString("%1").arg(friendInfo.sUserID));
    ui->ageLabel->setText ( QString("%1").arg(friendInfo.sUserAge));

}

void GroupShow::on_btnEdit_clicked()
{
    ifEdit = !ifEdit;
    ifAddMember =false;
    ui->btnDeleteMember->setEnabled(ifEdit);
    ui->btnAddMember->setEnabled(ifEdit);
    ui->btnDeleteAll->setEnabled(ifEdit);

}

void GroupShow::on_btnDeleteAll_clicked()
{
    emit delete_all();
}

void GroupShow::on_btnAddMember_clicked()
{
    ifAddMember=true;
    emit add_member();
}

void GroupShow::on_btnDeleteMember_clicked()
{
    emit delete_member();
}

void GroupShow::on_btnEdit_clicked_2()
{
    ifEdit = true;
    on_btnEdit_clicked();
}

