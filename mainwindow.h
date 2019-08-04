#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <math.h>
#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void updateStack();
    void connectdb(const QString&);
    void searchData(int, int);
    int* getBound(int, int);
    int validate(int);
    QString colorcode(int);
    ~MainWindow();

private slots:
    void on_homeBtn_clicked();
    void on_guideBtn_clicked();
    void on_linkBtn_clicked();
    void on_abroadBtn_clicked();
    void on_aboutBtn_clicked();
    void on_bioBtn_clicked();
    void on_ecoBtn_clicked();
    void on_searchBtn_clicked();

    void on_SearchBtn_eco_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
