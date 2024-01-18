#include"mainwindow.h"
#include <QApplication>
#include"login.h"
#include"pay.h"
#include<QThread>
#include"clientdataprocess.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setAttribute(Qt::AA_UseHighDpiPixmaps);
    MainWindow w ;
    login *log =new login();

    pay *m_Pay = new pay();


    clientDataProcess *clientDP = new clientDataProcess();
    QThread *thread = new QThread();
    clientDP->moveToThread(thread);

    //发送登入请求
    QObject::connect(clientDP,&clientDataProcess::sendConnected,log,&login::canLog);
    QObject::connect(clientDP,&clientDataProcess::sendDisconnected,log,&login::cannotLog);
    QObject::connect(log,&login::sendReconnect,clientDP,&clientDataProcess::reConnect);
    QObject::connect(log,&login::logSignal,clientDP,&clientDataProcess::initial);

    QObject::connect(log,&login::sendLoginInfo,clientDP,&clientDataProcess::processLoginInfo);
    //收到登入请求

    QObject::connect(clientDP,&clientDataProcess::logType,log,&login::logAccept);
    //主界面显示

    QObject::connect(clientDP,&clientDataProcess::signInInitSignal,&w,&MainWindow::signInInitHandle);

    //支付界面显示
    QObject::connect(&w,&MainWindow::showPayWidget,m_Pay,&pay::initialPayDialog);
    QObject::connect(&w,&MainWindow::sendCost,m_Pay,&pay::setMoneyLabel);
    QObject::connect(&w,&MainWindow::sendPath,m_Pay,&pay::setPath);
    QObject::connect(&w,&MainWindow::targetStation,m_Pay,&pay::targetStation);

    //发送消费信息
    QObject::connect(m_Pay,&pay::sendComsumeInfo,clientDP,&clientDataProcess::processComsumeInfo);



    thread->start();
    log->show();
    return a.exec();

}
