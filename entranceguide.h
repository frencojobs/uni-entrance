#ifndef ENTRANCEGUIDE_H
#define ENTRANCEGUIDE_H

#include <QUrl>
#include <QDesktopServices>
#include <QMainWindow>
#include <iostream>
#include <math.h>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

namespace Ui {
class EntranceGuide;
}

class EntranceGuide : public QMainWindow
{
    Q_OBJECT

public:
    explicit EntranceGuide(QWidget *parent = nullptr);
    void updateStack();
    void connectdb(const QString&);
    void searchData(int, int);
    int* getBound(int, int);
    int validate(int);
    QString colorcode(int);
    QString getMaxmark(int);
    QString getMinmark(int);
    ~EntranceGuide();

private slots:

    void on_male_btn_clicked();

    void on_female_btn_clicked();

    void on_home_btn_clicked();

    void on_guide_btn_clicked();

    void on_links_btn_clicked();

    void on_abroad_btn_clicked();

    void on_about_btn_clicked();

    void on_bio_btn_clicked();

    void on_eco_btn_clicked();

    void on_search_btn_clicked();

    void on_search_btn_eco_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_28_clicked();

private:
    Ui::EntranceGuide *ui;
};

#endif // ENTRANCEGUIDE_H
