#include "mainwindow.h"
#include "ui_mainwindow.h"

int arr[2] = {};
int madeList[12][2];
int mlc = 0;
bool isValid = true;

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
    ui->errmsg->hide();
    ui->errmsg_eco->hide();
    // example --
    ui->inp1->setText(QString::number(78));
    ui->inp2->setText(QString::number(75));
    ui->inp3->setText(QString::number(79));
    ui->inp4->setText(QString::number(88));
    ui->inp5->setText(QString::number(97));
    ui->inp6->setText(QString::number(88));
    ui->inp1_eco->setText(QString::number(78));
    ui->inp2_eco->setText(QString::number(75));
    ui->inp3_eco->setText(QString::number(79));
    ui->inp4_eco->setText(QString::number(88));
    ui->inp5_eco->setText(QString::number(97));
    ui->inp6_eco->setText(QString::number(88));
    //
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

void MainWindow::searchData(int k, int e)
{
    std::fill(&madeList[0][0], &madeList[0][0]+sizeof(madeList), 0);
    ui->listWidget->clear();
    ui->listWidget_eco->clear();
    mlc = 0;
    bool success = false;
    QSqlQuery query("SELECT * FROM marks");
    //if(e == 1) QSqlQuery query("SELECT * FROM marks WHERE NOT field = 'Medicine'");
    if (!query.isValid())
    {
         qDebug() << "SQL error: "<< query.lastError().text() << endl;
    }
    while (query.next())
    {
       int id = query.value(0).toInt();
       int se = query.value(3).toInt();
       int en = query.value(4).toInt();

       int upperbound = getBound(se, en)[0] + 2;
       int lowerbound = getBound(se, en)[1];
       int difference = upperbound - lowerbound;

       if(k >= lowerbound)
       {
           madeList[mlc][0] = id;
           madeList[mlc][1] = (k - lowerbound > difference) ? 100 : ceil((double(k - lowerbound)/(double)difference)*100.0);
           mlc++;
       }

       //qDebug() << id << ":" << upperbound << ", " << lowerbound;
       success = true;
    }
}

QString MainWindow::colorcode(int c)
{
    if(c > 79) return "green";
    else if(c > 39) return "orange";
         else return "red";
}

void MainWindow::updateStack()
{
    const QString originalStyleSheet = "QPushButton {background-color: transparent;border-width: 2px;border-radius: 10px;color: #000000;text-align: left;padding: 10px 0 10px 25px;margin: 5px 15px 0 15px;}QPushButton:pressed {background-color: rgba(114, 133, 165, 0.43);}";
    switch(ui->stackedWidget->currentIndex())
    {
        case 0:
        ui->homeBtn->setStyleSheet(originalStyleSheet+"QPushButton{background-color: rgba(114, 133, 165, 0.43);}");
        ui->guideBtn->setStyleSheet(originalStyleSheet);
        ui->linkBtn->setStyleSheet(originalStyleSheet);
        ui->abroadBtn->setStyleSheet(originalStyleSheet);
        ui->aboutBtn->setStyleSheet(originalStyleSheet);
        break;
        case 1:
        ui->homeBtn->setStyleSheet(originalStyleSheet);
        ui->guideBtn->setStyleSheet(originalStyleSheet+"QPushButton{background-color: rgba(114, 133, 165, 0.43);}");
        ui->linkBtn->setStyleSheet(originalStyleSheet);
        ui->abroadBtn->setStyleSheet(originalStyleSheet);
        ui->aboutBtn->setStyleSheet(originalStyleSheet);
        break;
        case 2:
        ui->homeBtn->setStyleSheet(originalStyleSheet);
        ui->guideBtn->setStyleSheet(originalStyleSheet);
        ui->linkBtn->setStyleSheet(originalStyleSheet+"QPushButton{background-color: rgba(114, 133, 165, 0.43);}");
        ui->abroadBtn->setStyleSheet(originalStyleSheet);
        ui->aboutBtn->setStyleSheet(originalStyleSheet);
        break;
        case 3:
        ui->homeBtn->setStyleSheet(originalStyleSheet);
        ui->guideBtn->setStyleSheet(originalStyleSheet);
        ui->linkBtn->setStyleSheet(originalStyleSheet);
        ui->abroadBtn->setStyleSheet(originalStyleSheet+"QPushButton{background-color: rgba(114, 133, 165, 0.43);}");
        ui->aboutBtn->setStyleSheet(originalStyleSheet);
        break;
        case 4:
        ui->homeBtn->setStyleSheet(originalStyleSheet);
        ui->guideBtn->setStyleSheet(originalStyleSheet);
        ui->linkBtn->setStyleSheet(originalStyleSheet);
        ui->abroadBtn->setStyleSheet(originalStyleSheet);
        ui->aboutBtn->setStyleSheet(originalStyleSheet+"QPushButton{background-color: rgba(114, 133, 165, 0.43);}");
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
    const QString originalStyleSheet = "QPushButton {background-color: #E0F4FD;border-width: 0;border-radius: 13px;color: #2BB7F3;;text-align: center;}QPushButton:pressed {background-color: #049BDC;color: #FFFFFF;}";
    ui->bioBtn->setStyleSheet(originalStyleSheet+"QPushButton{background-color: #049BDC;color:#E0F4FD;}");
    ui->ecoBtn->setStyleSheet(originalStyleSheet);
    ui->typed->setCurrentIndex(0);
}

void MainWindow::on_ecoBtn_clicked()
{
    const QString originalStyleSheet = "QPushButton {background-color: #E0F4FD;border-width: 0;border-radius: 13px;color: #2BB7F3;;text-align: center;}QPushButton:pressed {background-color: #049BDC;color: #FFFFFF;}";
    ui->bioBtn->setStyleSheet(originalStyleSheet);
    ui->ecoBtn->setStyleSheet(originalStyleSheet+"QPushButton{background-color: #049BDC;color:#E0F4FD;}");
    ui->typed->setCurrentIndex(1);
}

int MainWindow::validate(int v)
{
    if(v < 40 || v > 100) isValid = false;
    return v;
}

void MainWindow::on_searchBtn_clicked()
{
    const int total =
            validate(ui->inp1->text().toInt()) +
            validate(ui->inp2->text().toInt()) +
            validate(ui->inp3->text().toInt()) +
            validate(ui->inp4->text().toInt()) +
            validate(ui->inp5->text().toInt()) +
            validate(ui->inp6->text().toInt());

    searchData(total, 0);

    ui->totalmarks->clear();

    if(isValid)
    {
        ui->totalmarks->setText(QString::number(total));
        ui->errmsg->hide();
        for (int i=0;i<11;i++)
        {
            if(madeList[i][0] != 0)
            {
                QString order = "";
                if(ui->comboBox->currentText() != "None")
                {
                    order = " AND field = '" + ui->comboBox->currentText() + "'";
                }
                QSqlQuery query("SELECT name,field FROM marks WHERE id = "+QString::number(madeList[i][0])+ order);
                while(query.next())
                {
                    QWidget* w = new QWidget();

                    QHBoxLayout* hbl = new QHBoxLayout(w);
                    QLabel* percent_label = new QLabel(QString::number(madeList[i][1]) + "%");
                    QLabel* name_label = new QLabel(query.value(0).toString());

                    percent_label->setMargin(10);
                    percent_label->setStyleSheet("QLabel {font-family:'Source Code Pro';color: "+ colorcode(madeList[i][1]) +"}");

                    hbl->addWidget(percent_label);
                    hbl->addWidget(name_label);
                    hbl->setSizeConstraint(QLayout::SetFixedSize);

                    w->setLayout(hbl);

                    QListWidgetItem* lwi = new QListWidgetItem;
                    lwi->setSizeHint(w->sizeHint());
                    ui->listWidget->addItem(lwi);

                    ui->listWidget->setItemWidget(lwi, w);
                    //ui->listWidget->addItem(QString::number(madeList[i][1]) + "%     " + query.value(0).toString() + " (" + query.value(1).toString() + ")");
                }
            }
        }
    }
    else {
        ui->errmsg->show();
    }
    isValid = true;
}

void MainWindow::on_SearchBtn_eco_clicked()
{
    const int total =
            validate(ui->inp1_eco->text().toInt()) +
            validate(ui->inp2_eco->text().toInt()) +
            validate(ui->inp3_eco->text().toInt()) +
            validate(ui->inp4_eco->text().toInt()) +
            validate(ui->inp5_eco->text().toInt()) +
            validate(ui->inp6_eco->text().toInt());

    searchData(total, 1);

    ui->totalmarks_eco->clear();

    if(isValid)
    {
        ui->totalmarks_eco->setText(QString::number(total));
        ui->errmsg_eco->hide();
        for (int i=0;i<11;i++)
        {
            if(madeList[i][0] != 0)
            {
                QString order = "";
                if(ui->comboBox_eco->currentText() != "None")
                {
                    order = " AND field = '" + ui->comboBox_eco->currentText() + "'";
                }
                QSqlQuery query("SELECT name,field FROM marks WHERE id = "+QString::number(madeList[i][0])+ order);
                while(query.next())
                {
                    QWidget* w = new QWidget();

                    QHBoxLayout* hbl = new QHBoxLayout(w);
                    QLabel* percent_label = new QLabel(QString::number(madeList[i][1]) + "%");
                    QLabel* name_label = new QLabel(query.value(0).toString());

                    percent_label->setMargin(10);
                    percent_label->setStyleSheet("QLabel {font-family:'Source Code Pro';color: "+ colorcode(madeList[i][1]) +"}");

                    hbl->addWidget(percent_label);
                    hbl->addWidget(name_label);
                    hbl->setSizeConstraint(QLayout::SetFixedSize);

                    w->setLayout(hbl);

                    QListWidgetItem* lwi = new QListWidgetItem;
                    lwi->setSizeHint(w->sizeHint());
                    ui->listWidget_eco->addItem(lwi);

                    ui->listWidget_eco->setItemWidget(lwi, w);
                    //ui->listWidget->addItem(QString::number(madeList[i][1]) + "%     " + query.value(0).toString() + " (" + query.value(1).toString() + ")");
                }
            }
        }
    }
    else {
        ui->errmsg_eco->show();
    }
    isValid = true;
}
