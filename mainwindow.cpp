#include "mainwindow.h"
#include "ui_mainwindow.h"

int arr[2] = {};
int madeList[11] = {};
int mlc = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    connectdb("/home/frenco/marks.db");
    ui->setupUi(this);
    setWindowTitle("University Entrance Guide");
    setMinimumSize(1500, 1000);
    showMaximized();
    updateStack();
}

void MainWindow::connectdb(const QString& path)
{
   QSqlDatabase m_db = QSqlDatabase::addDatabase("QSQLITE");
   m_db.setDatabaseName(path);

   if (!m_db.open())
   {
      qDebug() << "Error: connection with database fail";
   }
   else
   {
      qDebug() << "Database: connection ok";
   }
}

int* MainWindow::getBound(int i, int j)
{
    if(i > j) {
        arr[0] = (j+(i-j));
        arr[1] = (j-(i-j));
    }
    else {
        arr[0] = (j+(j-i));
        arr[1] = (j-(j-i));
    }

    return arr;
}

void MainWindow::searchData(int k)
{
    std::fill(madeList, madeList+11, 0);
    ui->listWidget->clear();
    mlc = 0;
    bool success = false;
    QSqlQuery query("SELECT * FROM marks");
    if (!query.isValid())
    {
         qDebug() << "SQL error: "<< query.lastError().text() << endl;
    }
    while (query.next())
    {
       int id = query.value(0).toInt();
       int se = query.value(3).toInt();
       int en = query.value(4).toInt();

       int upperbound = getBound(se, en)[0];
       int lowerbound = getBound(se, en)[1];

       if(k >= lowerbound)
       {
           madeList[mlc] = id;
           mlc++;
       }

       //qDebug() << id << ":" << upperbound << ", " << lowerbound;
       success = true;
    }
}

void MainWindow::updateStack()
{
    const QString originalStyleSheet = "QPushButton {background-color: #B9BEBf;border-width: 0;border-radius: 0;color: #FFFFFF;text-align: right;padding-right: 10px;}QPushButton:pressed {background-color: #20AFCE;}";
    switch(ui->stackedWidget->currentIndex())
    {
        case 0:
        ui->homeBtn->setStyleSheet(originalStyleSheet+"QPushButton{background-color: #20AFCE;}");
        ui->guideBtn->setStyleSheet(originalStyleSheet);
        ui->linkBtn->setStyleSheet(originalStyleSheet);
        ui->abroadBtn->setStyleSheet(originalStyleSheet);
        ui->aboutBtn->setStyleSheet(originalStyleSheet);
        break;
        case 1:
        ui->homeBtn->setStyleSheet(originalStyleSheet);
        ui->guideBtn->setStyleSheet(originalStyleSheet+"QPushButton{background-color: #20AFCE;}");
        ui->linkBtn->setStyleSheet(originalStyleSheet);
        ui->abroadBtn->setStyleSheet(originalStyleSheet);
        ui->aboutBtn->setStyleSheet(originalStyleSheet);
        break;
        case 2:
        ui->homeBtn->setStyleSheet(originalStyleSheet);
        ui->guideBtn->setStyleSheet(originalStyleSheet);
        ui->linkBtn->setStyleSheet(originalStyleSheet+"QPushButton{background-color: #20AFCE;}");
        ui->abroadBtn->setStyleSheet(originalStyleSheet);
        ui->aboutBtn->setStyleSheet(originalStyleSheet);
        break;
        case 3:
        ui->homeBtn->setStyleSheet(originalStyleSheet);
        ui->guideBtn->setStyleSheet(originalStyleSheet);
        ui->linkBtn->setStyleSheet(originalStyleSheet);
        ui->abroadBtn->setStyleSheet(originalStyleSheet+"QPushButton{background-color: #20AFCE;}");
        ui->aboutBtn->setStyleSheet(originalStyleSheet);
        break;
        case 4:
        ui->homeBtn->setStyleSheet(originalStyleSheet);
        ui->guideBtn->setStyleSheet(originalStyleSheet);
        ui->linkBtn->setStyleSheet(originalStyleSheet);
        ui->abroadBtn->setStyleSheet(originalStyleSheet);
        ui->aboutBtn->setStyleSheet(originalStyleSheet+"QPushButton{background-color: #20AFCE;}");
        break;
    };
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_homeBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    updateStack();
}

void MainWindow::on_guideBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    updateStack();
}

void MainWindow::on_linkBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    updateStack();
}

void MainWindow::on_abroadBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    updateStack();
}

void MainWindow::on_aboutBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    updateStack();
}

void MainWindow::on_bioBtn_clicked()
{
    const QString originalStyleSheet = "QPushButton {background-color: #B9BEBF;border-width: 0;border-radius: 0;color: #FFFFFF;text-align: center;}QPushButton:pressed {background-color: #20AFCE;}";
    ui->bioBtn->setStyleSheet(originalStyleSheet+"QPushButton{background-color: #20AFCE;}");
    ui->ecoBtn->setStyleSheet(originalStyleSheet);
    ui->typed->setCurrentIndex(0);
}

void MainWindow::on_ecoBtn_clicked()
{
    const QString originalStyleSheet = "QPushButton {background-color: #B9BEBF;border-width: 0;border-radius: 0;color: #FFFFFF;text-align: center;}QPushButton:pressed {background-color: #20AFCE;}";
    ui->bioBtn->setStyleSheet(originalStyleSheet);
    ui->ecoBtn->setStyleSheet(originalStyleSheet+"QPushButton{background-color: #20AFCE;}");
    ui->typed->setCurrentIndex(1);
}

void MainWindow::on_searchBtn_clicked()
{
    const int total =
            ui->inp1->text().toInt() +
            ui->inp2->text().toInt() +
            ui->inp3->text().toInt() +
            ui->inp4->text().toInt() +
            ui->inp5->text().toInt() +
            ui->inp6->text().toInt();
    searchData(total);
    for (int i=0;i<11;i++) {
        if(madeList[i] != 0){
            QSqlQuery query("SELECT name FROM marks WHERE id = "+QString::number(madeList[i])+"");
            query.first();
            ui->listWidget->addItem(query.value(0).toString());
//            qDebug() <<  << endl;
            qDebug() << madeList[i];
        }
    }
}
