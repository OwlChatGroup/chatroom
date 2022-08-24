#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QTime>
#include <QDebug>


QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

    QDateTime       m_lastShowMsgTime;
    QStringList     m_sAcPicList;   //拖拽入得图片列表
    QStringList     m_sAcFileList;  //拖拽入得文件列表

protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);

private slots:
    void on_btn_sendmsg_clicked();

private:
    Ui::Dialog *ui;
    // 用户相关信息保存
    //QQCell *m_cell;
    QStandardItemModel *m_model;
};
#endif // DIALOG_H
