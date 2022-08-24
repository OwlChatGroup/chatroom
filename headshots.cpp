#include "headshots.h"
#include "ui_headshots.h"

Headshots::Headshots(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Headshots)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}

Headshots::~Headshots()
{
    delete ui;
}

void Headshots::on_btn_1_clicked()
{
    headshotsName = "1(1).png";
    this->hide();
    emit onClicked();
}

void Headshots::on_btn_2_clicked()
{
    headshotsName = "2.png";
    this->hide();
    emit onClicked();
}

void Headshots::on_btn_3_clicked()
{
    headshotsName = "3.png";
    this->hide();
    emit onClicked();
}

void Headshots::on_btn_4_clicked()
{
    headshotsName = "4.png";
    this->hide();
    emit onClicked();
}

void Headshots::on_btn_5_clicked()
{
    headshotsName = "5.png";
    this->hide();
    emit onClicked();
}

void Headshots::on_btn_6_clicked()
{
    headshotsName = "6.png";
    this->hide();
    emit onClicked();
}
