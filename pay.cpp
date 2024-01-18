#include "pay.h"
#include "ui_pay.h"
#include<QLabel>

pay::pay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::pay)
{
    ui->setupUi(this);

   // clientDataProcess *clientDP2 = new clientDataProcess;
   // connect(this,&pay::sendComsumeInfo,clientDP2,&clientDataProcess::processComsumeInfo);



}

pay::~pay()
{
    delete ui;
}

void pay::initialPayDialog()
{
    show();
    ui->pathlabel->setWordWrap(true);
    QPixmap pixmap(":/res/png/generate.png");
    pixmap = pixmap.scaled(ui->pixlabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->pixlabel->setPixmap(pixmap);
    ui->pixlabel->setAlignment(Qt::AlignCenter);
}

void pay::setPath(const QString &path)
{

    ui->pathlabel->setText(path);

}

void pay::setMoneyLabel(const qint8 &money)
{
   // qint8 money=path*tickets;
    cost = money;
    //QString str=QString("张数：%1一共是：%2元").arg(tickets,money);
    ui->moneylabel->setText(QString::asprintf("这是%d张票，一共是%d元",tickets,money));
}

void pay::targetStation(QString startStationName, QString endStationName)
{

    this->startS = startStationName;
    this->endS = endStationName;

}



void pay::on_ticket1_pb_clicked()
{
    tickets=1;
    ui->moneylabel->setText(QString::asprintf("这是%d张票，一共是%d元",tickets,cost));
}


void pay::on_ticket2_pb_clicked()
{
    tickets=2;
     ui->moneylabel->setText(QString::asprintf("这是%d张票，一共是%d元",tickets,cost*tickets));

}


void pay::on_exit_pb_clicked()
{
    this->close();
}


void pay::on_pb_pay_clicked()

{

    qDebug()<<this->startS<<this->endS<<this->cost<<this->tickets;

    emit sendComsumeInfo(this->startS,this->endS,this->cost,this->tickets);


    this->close();
}

