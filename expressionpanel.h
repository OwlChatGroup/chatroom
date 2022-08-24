#ifndef EXPRESSIONPANEL_H
#define EXPRESSIONPANEL_H

#include <QDialog>

namespace Ui {
class ExpressionPanel;
}

class ExpressionPanel : public QDialog
{
    Q_OBJECT

public:
    explicit ExpressionPanel(QWidget *parent = 0);
    ~ExpressionPanel();

    QString emojiName;
public slots:


private slots:

    void on_btn_1_clicked();

    void on_btn_2_clicked();

    void on_btn_3_clicked();

    void on_btn_4_clicked();

    void on_btn_5_clicked();

    void on_btn_6_clicked();

    void on_btn_7_clicked();

    void on_btn_8_clicked();

    void on_btn_9_clicked();

    void on_btn_10_clicked();

    void on_btn_11_clicked();

    void on_btn_12_clicked();

    void on_btn_13_clicked();

    void on_btn_14_clicked();

    void on_btn_15_clicked();

    void on_btn_16_clicked();

    void on_btn_17_clicked();

    void on_btn_18_clicked();

    void on_btn_19_clicked();

    void on_btn_20_clicked();

    void on_btn_21_clicked();

private:
    Ui::ExpressionPanel *ui;

signals:
    void onClicked();

};

#endif // EXPRESSIONPANEL_H
