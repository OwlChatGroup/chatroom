#ifndef  MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPoint>
#include <QCursor>
#include <QRect>
#include <ApiUtils/api_utils.h>

#define PADDING 2



namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    explicit MainWindow(QTcpSocket *client,QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_pushButton_clieked();
    void handReadyRead();

    void test0(uint8_t);
        void test1(uint8_t);
        void test2(uint8_t);
        void test3(uint32_t fromUserID, uint32_t sessionID, uint64_t time, uint8_t msg_type, QString content);
        void on_pushButton_2_clicked();

        void on_pushButton_3_clicked();

        void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *client;
    ApiUtils* api;
};




#endif // MAINWINDOW_H
