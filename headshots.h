#ifndef HEADSHOTS_H
#define HEADSHOTS_H

#include <QDialog>

namespace Ui {
class Headshots;
}

class Headshots : public QDialog
{
    Q_OBJECT

public:
    explicit Headshots(QWidget *parent = 0);
    ~Headshots();
    QString headshotsName;

private slots:

    void on_btn_1_clicked();

    void on_btn_2_clicked();

    void on_btn_3_clicked();

    void on_btn_4_clicked();

    void on_btn_5_clicked();

    void on_btn_6_clicked();

private:
    Ui::Headshots *ui;

signals:
    void onClicked();
};

#endif // HEADSHOTS_H
