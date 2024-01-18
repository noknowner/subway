#ifndef CLIENTDATAPROCESS_H
#define CLIENTDATAPROCESS_H

#include <QObject>
#include<QTcpServer>
#include<QTcpSocket>
#include<QTimer>
#include"heartbeatthread.h"
class clientDataProcess : public QObject
{
    Q_OBJECT
public:
    explicit clientDataProcess(QObject *parent = nullptr);


public slots:
    void initial();
    void processLoginInfo(QString usr,QString pwd);
    void processComsumeInfo(QString startStationName,QString endStationName,qint16 fare,qint8 tickets);
    void reConnect();
private:
    QStringList getLocalIPAddresses();

private slots:
    void myRead();  // 收包槽函数

     void startHeartBeat();

    void sendClientHeartbeat();
    void handleHeartbeatBroken();

signals:
    void logRight();
    void logType(int i);
    void signInInitSignal();
    void sendConnected();
    void sendDisconnected();
    void sendstartHeartBeat();


private:

    QTcpSocket *m_client;
    // int m_heartCheckTimes;
    // QTimer *m_checkTimer;

    HeartbeatThread *m_pheart;

};

#endif // CLIENTDATAPROCESS_H
