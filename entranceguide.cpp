#include "entranceguide.h"
#include "ui_entranceguide.h"

int bounds[2] = {};
int madeList[99][2];
int sortedList[99][2];
int mlc = 0;
bool isValid = true;
bool isMale = true;

EntranceGuide::EntranceGuide(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EntranceGuide)
{
    QFile File(":/styles/stylesheet.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    connectdb("/home/frenco/ent.db");
    ui->setupUi(this);
    ui->centralWidget->setStyleSheet(StyleSheet);
    setState();
    ui->err_msg->hide();
    ui->err_msg_eco->hide();
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

void EntranceGuide::connectdb(const QString& path)
{
   QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
   db.setDatabaseName(path);
   if (!db.open()) qDebug() << "Error: connection with database fail";
   else qDebug() << "Database: connection ok";
}

int* EntranceGuide::getBound(int i, int j)
{
    int avg = (i+j)/2;
    if(i-j < 5 || i-j > -5){
        bounds[0] = avg+5;
        bounds[1] = avg-5;
    } else {
        if(i > j) {
            bounds[0] = (avg+(i-j));
            bounds[1] = (avg-(i-j));
        }
        else {
            bounds[0] = (avg+(j-i));
            bounds[1] = (avg-(j-i));
        }
    }
    return bounds;
}

void EntranceGuide::searchData(int k, int e)
{
    for(int i=0;i<99;i++)
    {
        madeList[i][0]=0;
        madeList[i][1]=101;
    }
    std::fill(&sortedList[0][0], &sortedList[0][0]+sizeof(sortedList), 200);
    // clear the previous values && set mlc to 0
    //std::fill(&madeList[0][0], &madeList[0][0]+sizeof(madeList), 0);
    ui->main_listView->clear();
    ui->main_listView_eco->clear();
    mlc = 0;
    bool success = false;
    // perpare a query to execute later
    // make gender
    QString gender = isMale ? "male" : "female";
    QSqlQuery query;
    if(e == 1) query.prepare("SELECT * FROM "+ gender +" WHERE NOT field = 'Medicine'"); // for ECO
    else query.prepare("SELECT * FROM "+ gender);

    query.exec(); // execute the query

    if (!query.isValid()) qDebug() << "SQL error: "<< query.lastError().text() << endl; // checks if the query is valid
    while (query.next())
    {
       int id = query.value(0).toInt();
       int se = query.value(2).toInt();
       int en = query.value(3).toInt();

       int upperbound = getBound(se, en)[0] + 2;
       int lowerbound = getBound(se, en)[1];
       int difference = upperbound - lowerbound;

       if(k >= lowerbound)
       {
           madeList[mlc][0] = id;
           madeList[mlc][1] = (k - lowerbound > difference) ? 100 : ceil((double(k - lowerbound)/(double)difference)*100.0);
           mlc++;
       }
       success = true;
    }

    for(int i=0;i<99;i++)
    {
        for(int j=99;j >= 0;j--)
        {
            if(madeList[i][1]>=sortedList[j][1])
            {
                madeList[i][1] = sortedList[j][1];
                madeList[i][0] = sortedList[j][0];
            }
        }
        for(int j=0;j < 99;j++)
        {
            if(madeList[i][1] == sortedList[j][1])
            {
                sortedList[j][1] = 200;
                break;
            }
        }
    }
}
int EntranceGuide::validate(int v)
{
    if(v < 40 || v > 100) isValid = false;
    return v;
}

QString EntranceGuide::colorcode(int c)
{
    if(c > 79) return "green";
    else if(c > 39) return "orange";
         else return "red";
}
QString EntranceGuide::getMaxmark(int t)
{
    QString gender = isMale ? "male" : "female";
    QSqlQuery query("SELECT se,en from "+ gender +" WHERE id = "+ QString::number(t));
    query.first();
    int se = query.value(0).toInt();
    int en = query.value(1).toInt();
    return (se > en) ? QString::number(se) : QString::number(en);
}

QString EntranceGuide::getMinmark(int t)
{
    QString gender = isMale ? "male" : "female";
    QSqlQuery query("SELECT se,en from "+ gender +" WHERE id = "+ QString::number(t));
    query.first();
    int se = query.value(0).toInt();
    int en = query.value(1).toInt();
    return (se < en) ? QString::number(se) : QString::number(en);
}

QString EntranceGuide::padded(QString s)
{
    switch (s.length())
    {
        case 1: return "  " + s;
        case 2: return " " + s;
        default: return s;
    }
}

void EntranceGuide::setState()
{
    // gender
    const QString genderOriginalStyleSheet = "QPushButton { background-color: #F2F2F2; color: #1976D2; border: 0; border-radius: 10px; padding: 10px 15px; margin: 0 20px; } QPushButton:pressed { background-color: #1976D2; color: #F2F2F2; }";
    const QString genderChangedStyleSheet = "QPushButton { background-color: #1976D2; color: #FFFFFF; border: 0; border-radius: 10px; padding: 10px 15px; margin: 0 20px; } QPushButton:pressed { background-color: #1976D2; color: #F2F2F2; }";
    if(isMale)
    {
        ui->male_btn->setStyleSheet(genderOriginalStyleSheet + genderChangedStyleSheet);
        ui->female_btn->setStyleSheet(genderOriginalStyleSheet);
    } else {
        ui->male_btn->setStyleSheet(genderOriginalStyleSheet);
        ui->female_btn->setStyleSheet(genderOriginalStyleSheet + genderChangedStyleSheet);
    }
    // sidebar
    const QString sidebarOriginalStyleSheet = "QPushButton { border: 0;text-align: left; padding: 15px 40px;}";
    const QString sidebarChangedStyleSheet = "QPushButton { background-color: #FFFFFF; border-right: 2px solid #1976D2; }";
    switch(ui->stackedWidget->currentIndex())
    {
        case 0:
        ui->home_btn->setStyleSheet(sidebarOriginalStyleSheet+sidebarChangedStyleSheet);
        ui->guide_btn->setStyleSheet(sidebarOriginalStyleSheet);
        ui->links_btn->setStyleSheet(sidebarOriginalStyleSheet);
        ui->abroad_btn->setStyleSheet(sidebarOriginalStyleSheet);
        ui->about_btn->setStyleSheet(sidebarOriginalStyleSheet);
        break;
        case 1:
        ui->home_btn->setStyleSheet(sidebarOriginalStyleSheet);
        ui->guide_btn->setStyleSheet(sidebarOriginalStyleSheet+sidebarChangedStyleSheet);
        ui->links_btn->setStyleSheet(sidebarOriginalStyleSheet);
        ui->abroad_btn->setStyleSheet(sidebarOriginalStyleSheet);
        ui->about_btn->setStyleSheet(sidebarOriginalStyleSheet);
        break;
        case 2:
        ui->home_btn->setStyleSheet(sidebarOriginalStyleSheet);
        ui->guide_btn->setStyleSheet(sidebarOriginalStyleSheet);
        ui->links_btn->setStyleSheet(sidebarOriginalStyleSheet+sidebarChangedStyleSheet);
        ui->abroad_btn->setStyleSheet(sidebarOriginalStyleSheet);
        ui->about_btn->setStyleSheet(sidebarOriginalStyleSheet);
        break;
        case 3:
        ui->home_btn->setStyleSheet(sidebarOriginalStyleSheet);
        ui->guide_btn->setStyleSheet(sidebarOriginalStyleSheet);
        ui->links_btn->setStyleSheet(sidebarOriginalStyleSheet);
        ui->abroad_btn->setStyleSheet(sidebarOriginalStyleSheet+sidebarChangedStyleSheet);
        ui->about_btn->setStyleSheet(sidebarOriginalStyleSheet);
        break;
        case 4:
        ui->home_btn->setStyleSheet(sidebarOriginalStyleSheet);
        ui->guide_btn->setStyleSheet(sidebarOriginalStyleSheet);
        ui->links_btn->setStyleSheet(sidebarOriginalStyleSheet);
        ui->abroad_btn->setStyleSheet(sidebarOriginalStyleSheet);
        ui->about_btn->setStyleSheet(sidebarOriginalStyleSheet+sidebarChangedStyleSheet);
        break;
    };
    // bioeco btns
    const QString routersOriginalStyleSheet = "QPushButton { border: 1px solid #1976D2; border-radius: 10px; padding: 10px 20px; background-color: #FFFFFF; color: #1976D2; margin: 50px 10px; }";
    const QString routersChangedStyleSheet = "QPushButton { border: 1px solid #1976D2; border-radius: 10px; padding: 10px 20px; background-color: #1976D2; color: #FFFFFF; margin: 50px 10px; }";
    switch(ui->inner_stackedWidget->currentIndex())
    {
        case 0:
        ui->bio_btn->setStyleSheet(routersOriginalStyleSheet+routersChangedStyleSheet);
        ui->eco_btn->setStyleSheet(routersOriginalStyleSheet);
        break;
        case 1:
        ui->bio_btn->setStyleSheet(routersOriginalStyleSheet);
        ui->eco_btn->setStyleSheet(routersOriginalStyleSheet+routersChangedStyleSheet);
        break;
    }
}
void EntranceGuide::on_male_btn_clicked()
{
    isMale = true;
    setState();
}

void EntranceGuide::on_female_btn_clicked()
{
    isMale = false;
    setState();
}

void EntranceGuide::on_home_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    setState();
}

void EntranceGuide::on_guide_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    setState();
}

void EntranceGuide::on_links_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    setState();
}

void EntranceGuide::on_abroad_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    setState();
}

void EntranceGuide::on_about_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    setState();
}

void EntranceGuide::on_bio_btn_clicked()
{
    ui->inner_stackedWidget->setCurrentIndex(0);
    setState();
}

void EntranceGuide::on_eco_btn_clicked()
{
    ui->inner_stackedWidget->setCurrentIndex(1);
    setState();
}

void EntranceGuide::on_search_btn_clicked()
{
    const int total =
            validate(ui->inp1->text().toInt()) +
            validate(ui->inp2->text().toInt()) +
            validate(ui->inp3->text().toInt()) +
            validate(ui->inp4->text().toInt()) +
            validate(ui->inp5->text().toInt()) +
            validate(ui->inp6->text().toInt());
    QString gender = isMale ? "male" : "female";

    searchData(total, 0);
    ui->totalmarks->clear();
    if(isValid)
    {
        ui->totalmarks->setText(QString::number(total));
        ui->err_msg->hide();
        for (int i=0;i<99;i++)
        {
            if(madeList[i][0] != 0)
            {
                QString order = "";
                if(ui->field_box->currentText() != "None")
                {
                    order = " AND field = '" + ui->field_box->currentText() + "'";
                }
                QSqlQuery query("SELECT name,field FROM "+ gender +" WHERE id = "+QString::number(madeList[i][0])+ order);
                while(query.next())
                {
                    QWidget* mainWidget = new QWidget();
                    QWidget* firstWidet = new QWidget();
                    QWidget* secondWidget = new QWidget();
                    QWidget* spacer = new QWidget();
                    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

                    QHBoxLayout* mainHbl = new QHBoxLayout(mainWidget);
                    QHBoxLayout* firstHbl = new QHBoxLayout(firstWidet);
                    QHBoxLayout* secondHbl = new QHBoxLayout(secondWidget);
                    QLabel* percent_label = new QLabel(padded(QString::number(madeList[i][1])) + "%");
                    QLabel* name_label = new QLabel(query.value(0).toString());
                    QLabel* upIcon = new QLabel();
                    QLabel* downIcon = new QLabel();
                    QLabel* maxmark_label = new QLabel(getMaxmark(madeList[i][0]));
                    QLabel* minmark_label = new QLabel(getMinmark(madeList[i][0]));

                    percent_label->setMargin(13);
                    percent_label->setStyleSheet("QLabel {font-family:'Source Code Pro';color: "+ colorcode(madeList[i][1]) +"}");

                    QFont percent_label_font = percent_label->font();
                    percent_label_font.setPointSize(16);
                    percent_label->setFont(percent_label_font);

                    firstHbl->addWidget(percent_label);
                    firstHbl->addWidget(name_label);
                    firstHbl->setSizeConstraint(QLayout::SetFixedSize);
                    firstWidet->setLayout(firstHbl);

                    upIcon->setPixmap(QPixmap("/home/frenco/Qt/projects/ueis/icons_up.png"));
                    downIcon->setPixmap(QPixmap("/home/frenco/Qt/projects/ueis/icons_down.png"));

                    secondHbl->addWidget(upIcon);
                    secondHbl->addWidget(maxmark_label);
                    secondHbl->addWidget(downIcon);
                    secondHbl->addWidget(minmark_label);
                    secondHbl->setSizeConstraint(QLayout::SetFixedSize);
                    secondWidget->setLayout(secondHbl);

                    mainHbl->addWidget(firstWidet);
                    mainHbl->addWidget(spacer);
                    mainHbl->addWidget(secondWidget);
                    mainHbl->setSizeConstraint(QLayout::SetMaximumSize);
                    mainWidget->setLayout(mainHbl);

                    QListWidgetItem* mainListWidgetItem = new QListWidgetItem;
                    mainListWidgetItem->setSizeHint(mainWidget->sizeHint());
                    ui->main_listView->addItem(mainListWidgetItem);

                    ui->main_listView->setItemWidget(mainListWidgetItem, mainWidget);
                    //ui->listWidget->addItem(QString::number(madeList[i][1]) + "%     " + query.value(0).toString() + " (" + query.value(1).toString() + ")");
                }
            }
        }
    }
    else {
        ui->err_msg->show();
    }
    isValid = true;
}

void EntranceGuide::on_search_btn_eco_clicked()
{
    const int total =
            validate(ui->inp1_eco->text().toInt()) +
            validate(ui->inp2_eco->text().toInt()) +
            validate(ui->inp3_eco->text().toInt()) +
            validate(ui->inp4_eco->text().toInt()) +
            validate(ui->inp5_eco->text().toInt()) +
            validate(ui->inp6_eco->text().toInt());
    QString gender = isMale ? "male" : "female";

    searchData(total, 1);
    ui->totalmarks_eco->clear();
    if(isValid)
    {
        ui->totalmarks_eco->setText(QString::number(total));
        ui->err_msg_eco->hide();
        for (int i=0;i<99;i++)
        {
            if(madeList[i][0] != 0)
            {
                QString order = "";
                if(ui->field_box_eco->currentText() != "None")
                {
                    order = " AND field = '" + ui->field_box_eco->currentText() + "'";
                }
                QSqlQuery query("SELECT name,field FROM "+ gender +" WHERE id = "+QString::number(madeList[i][0])+ order);
                while(query.next())
                {
                    QWidget* mainWidget = new QWidget();
                    QWidget* firstWidet = new QWidget();
                    QWidget* secondWidget = new QWidget();
                    QWidget* spacer = new QWidget();
                    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

                    QHBoxLayout* mainHbl = new QHBoxLayout(mainWidget);
                    QHBoxLayout* firstHbl = new QHBoxLayout(firstWidet);
                    QHBoxLayout* secondHbl = new QHBoxLayout(secondWidget);
                    QLabel* percent_label = new QLabel(padded(QString::number(madeList[i][1])) + "%");
                    QLabel* name_label = new QLabel(query.value(0).toString());
                    QLabel* upIcon = new QLabel();
                    QLabel* downIcon = new QLabel();
                    QLabel* maxmark_label = new QLabel(getMaxmark(madeList[i][0]));
                    QLabel* minmark_label = new QLabel(getMinmark(madeList[i][0]));

                    percent_label->setMargin(13);
                    percent_label->setStyleSheet("QLabel {font-family:'Source Code Pro';color: "+ colorcode(madeList[i][1]) +"}");

                    QFont percent_label_font = percent_label->font();
                    percent_label_font.setPointSize(16);
                    percent_label->setFont(percent_label_font);

                    firstHbl->addWidget(percent_label);
                    firstHbl->addWidget(name_label);
                    firstHbl->setSizeConstraint(QLayout::SetFixedSize);
                    firstWidet->setLayout(firstHbl);

                    upIcon->setPixmap(QPixmap("/home/frenco/Qt/projects/ueis/icons_up.png"));
                    downIcon->setPixmap(QPixmap("/home/frenco/Qt/projects/ueis/icons_down.png"));

                    secondHbl->addWidget(upIcon);
                    secondHbl->addWidget(maxmark_label);
                    secondHbl->addWidget(downIcon);
                    secondHbl->addWidget(minmark_label);
                    secondHbl->setSizeConstraint(QLayout::SetFixedSize);
                    secondWidget->setLayout(secondHbl);

                    mainHbl->addWidget(firstWidet);
                    mainHbl->addWidget(spacer);
                    mainHbl->addWidget(secondWidget);
                    mainHbl->setSizeConstraint(QLayout::SetMaximumSize);
                    mainWidget->setLayout(mainHbl);

                    QListWidgetItem* mainListWidgetItem = new QListWidgetItem;
                    mainListWidgetItem->setSizeHint(mainWidget->sizeHint());
                    ui->main_listView_eco->addItem(mainListWidgetItem);

                    ui->main_listView_eco->setItemWidget(mainListWidgetItem, mainWidget);
                    //ui->listWidget->addItem(QString::number(madeList[i][1]) + "%     " + query.value(0).toString() + " (" + query.value(1).toString() + ")");
                }
            }
        }
    }
    else {
        ui->err_msg_eco->show();
    }
    isValid = true;
}

EntranceGuide::~EntranceGuide()
{
    delete ui;
}

// University Links

void EntranceGuide::on_pushButton_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.um1ygn.edu.mm/en/home/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_2_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.um2ygn.edu.mm/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_3_clicked()
{
    QDesktopServices::openUrl(QUrl("https://ytu.edu.mm/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_4_clicked()
{
    QDesktopServices::openUrl(QUrl("https://www.uy.edu.mm/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_5_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.yueco.edu.mm/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_6_clicked()
{
    QDesktopServices::openUrl(QUrl("https://www.yufl.edu.mm/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_7_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.mmu.edu.mm/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_8_clicked()
{
    QDesktopServices::openUrl(QUrl("http://udmyangon-edu.com/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_9_clicked()
{
    QDesktopServices::openUrl(QUrl("https://www.nmdc.edu.mm/en/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_10_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.umty.gov.mm/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_11_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.yuoe.edu.mm/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_12_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.mmmc.edu.mm/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_13_clicked()
{
    QDesktopServices::openUrl(QUrl("http://mu.edu.mm/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_14_clicked()
{
    QDesktopServices::openUrl(QUrl("http://utycc.moe.edu.mm/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_15_clicked()
{
    QDesktopServices::openUrl(QUrl("http://mtu.moe.edu.mm/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_16_clicked()
{
    QDesktopServices::openUrl(QUrl("http://maeu.edu.mm/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_17_clicked()
{
    QDesktopServices::openUrl(QUrl("https://www.ucsm.edu.mm/ucsm/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_18_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.yau.edu.mm/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_19_clicked()
{
    QDesktopServices::openUrl(QUrl("https://www.myanmar-edu.org/mueucap-university-forestry-yezin", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_20_clicked()
{
    QDesktopServices::openUrl(QUrl("https://www.uvsyezin.edu.mm/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_21_clicked()
{
    QDesktopServices::openUrl(QUrl("https://mandalay.edu.mm/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_22_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.culture.gov.mm/Universities/NUAC_YGN/default.asp", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_23_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.ummg.gov.mm/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_24_clicked()
{
    QDesktopServices::openUrl(QUrl("https://www.umtgi.org/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_25_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.ummdy.gov.mm/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_26_clicked()
{
    QDesktopServices::openUrl(QUrl("http://udmmandalay.gov.mm/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_27_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.tcu.edu.mm/", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_28_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.ltcbagan.com.mm/en/node/8", QUrl::TolerantMode));
}

void EntranceGuide::on_pushButton_29_clicked()
{
    QDesktopServices::openUrl(QUrl("https://uit.edu.mm/", QUrl::TolerantMode));
}
// Links end
