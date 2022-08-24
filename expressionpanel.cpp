#include "expressionpanel.h"
#include "ui_expressionpanel.h"
#include <QFile>

ExpressionPanel::ExpressionPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExpressionPanel)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
}

ExpressionPanel::~ExpressionPanel()
{
    delete ui;
}

void ExpressionPanel::on_btn_1_clicked()
{
    emojiName = "image_emoticon.png";
    this->hide();
    emit onClicked();
}

void ExpressionPanel::on_btn_2_clicked()
{
    emojiName = "image_emoticon10.png";
    this->hide();
    emit onClicked();
}

void ExpressionPanel::on_btn_3_clicked()
{
    emojiName = "image_emoticon100.png";
    this->hide();
    emit onClicked();
}

void ExpressionPanel::on_btn_4_clicked()
{
    emojiName = "image_emoticon11.png";
    this->hide();
    emit onClicked();
}

void ExpressionPanel::on_btn_5_clicked()
{
    emojiName = "image_emoticon12.png";
    this->hide();
    emit onClicked();
}

void ExpressionPanel::on_btn_6_clicked()
{
    emojiName = "image_emoticon13.png";
    this->hide();
    emit onClicked();
}

void ExpressionPanel::on_btn_7_clicked()
{
    emojiName = "image_emoticon14.png";
    this->hide();
    emit onClicked();
}

void ExpressionPanel::on_btn_8_clicked()
{
    emojiName = "image_emoticon15.png";
    this->hide();
    emit onClicked();
}

void ExpressionPanel::on_btn_9_clicked()
{
    emojiName = "image_emoticon16.png";
    this->hide();
    emit onClicked();
}

void ExpressionPanel::on_btn_10_clicked()
{
    emojiName = "image_emoticon17.png";
    this->hide();
    emit onClicked();
}

void ExpressionPanel::on_btn_11_clicked()
{
    emojiName = "image_emoticon18.png";
    this->hide();
    emit onClicked();
}

void ExpressionPanel::on_btn_12_clicked()
{
    emojiName = "image_emoticon19.png";
    this->hide();
    emit onClicked();
}

void ExpressionPanel::on_btn_13_clicked()
{
    emojiName = "image_emoticon2.png";
    this->hide();
    emit onClicked();
}

void ExpressionPanel::on_btn_14_clicked()
{
    emojiName = "image_emoticon20.png";
    this->hide();
    emit onClicked();
}

void ExpressionPanel::on_btn_15_clicked()
{
    emojiName = "image_emoticon22.png";
    this->hide();
    emit onClicked();
}

void ExpressionPanel::on_btn_16_clicked()
{
    emojiName = "image_emoticon23.png";
    this->hide();
    emit onClicked();
}

void ExpressionPanel::on_btn_17_clicked()
{
    emojiName = "image_emoticon24.png";
    this->hide();
    emit onClicked();
}

void ExpressionPanel::on_btn_18_clicked()
{
    emojiName = "image_emoticon25.png";
    this->hide();
    emit onClicked();
}

void ExpressionPanel::on_btn_19_clicked()
{
    emojiName = "image_emoticon3.png";
    this->hide();
    emit onClicked();
}

void ExpressionPanel::on_btn_20_clicked()
{
    emojiName = "image_emoticon6.png";
    this->hide();
    emit onClicked();
}

void ExpressionPanel::on_btn_21_clicked()
{
    emojiName = "image_emoticon9.png";
    this->hide();
    emit onClicked();
}
