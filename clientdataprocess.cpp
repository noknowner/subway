#include "clientdataprocess.h"
#include"datastruct.h"
#include<QTime>
#include<QJsonDocument>
#include<QJsonObject>
#include <QNetworkInterface>
#include <QStringList>

extern devInfo m_devInfo;
extern hostInfo HostInFo;

clientDataProcess::clientDataProcess(QObject *parent)
    : QObject{parent}
{

    m_client = new QTcpSocket(this);
    QTimer *timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [=]{
            if (!m_client->isOpen()) {
                m_client->connectToHost(HostInFo.hostIP,HostInFo.hostPort);
                if(m_client->waitForConnected()){
                    qDebug()<<"conn ok";
                    timer->stop(); // 连接成功，停止定时器
                       m_pheart= new HeartbeatThread();
                      m_pheart->initHeartCounter();
                      m_pheart->setHeartType(Heart_Client);


                      emit sendConnected();
                          connect(m_client,&QTcpSocket::readyRead,this,&clientDataProcess::myRead);

                        connect(m_client,&QTcpSocket::readyRead,m_pheart,&HeartbeatThread::onResetCounter);

                         connect(m_pheart, &HeartbeatThread::sendClientHeartbeatPackage, this, &clientDataProcess::sendClientHeartbeat);


                        connect(m_pheart, &HeartbeatThread::heartbeatBrokenClient, this, &clientDataProcess::handleHeartbeatBroken);


                } else {
                    qDebug()<<"conn fail"<<m_client->errorString();
                    //m_client->disconnect();
                    // timer->start(500);
                    emit sendDisconnected();
                }
            }
        });

        timer->start(500); // 每隔0.5秒尝试连接

    // m_client = new QTcpSocket(this);
    // // 启动tcp默认的保活
    // //m_client->setSocketOption(QAbstractSocket::KeepAliveOption,true);

    //  m_client->connectToHost(HostInFo.hostIP,HostInFo.hostPort);
    //  if(m_client->waitForConnected()){
    //      qDebug()<<"conn ok";

    // //     m_pheart= new HeartbeatThread();
    // //     m_pheart->initHeartCounter();
    // //     m_pheart->setHeartType(Heart_Client);



    //      connect(m_client,&QTcpSocket::readyRead,this,&clientDataProcess::myRead);

    // //     connect(m_client,&QTcpSocket::readyRead,m_pheart,&HeartbeatThread::onResetCounter);

    // //     connect(m_pheart, &HeartbeatThread::sendClientHeartbeatPackage, this, &clientDataProcess::sendClientHeartbeat);


    // //     connect(m_pheart, &HeartbeatThread::heartbeatBrokenClient, this, &clientDataProcess::handleHeartbeatBroken);



    //  }else{
    //     qDebug()<<"conn fail"<<m_client->errorString();
    //      m_client->disconnect();
    // }


     QStringList ipList = getLocalIPAddresses();

     m_devInfo.devIP = ipList.at(0);



}

void clientDataProcess::initial()
{
    // m_client = new QTcpSocket(this);
    // 启动tcp默认的保活
    //m_client->setSocketOption(QAbstractSocket::KeepAliveOption,true);

      // this->m_client->connectToHost(HostInFo.hostIP,HostInFo.hostPort);
      // if(m_client->waitForConnected()){
      //     qDebug()<<"conn ok";
      //    connect(m_client,&QTcpSocket::readyRead,this,&clientDataProcess::myRead);

   //      m_pheart= new HeartbeatThread();
   //      m_pheart->initHeartCounter();
   //      m_pheart->setHeartType(Heart_Client);

   //      connect(m_client,SIGNAL(readyRead()),this,SLOT(myRead()));


   //      connect(m_client,&QTcpSocket::readyRead,m_pheart,&HeartbeatThread::onResetCounter);

   //      connect(m_pheart, &HeartbeatThread::sendClientHeartbeatPackage, this, &clientDataProcess::sendClientHeartbeat);


   //      connect(m_pheart, &HeartbeatThread::heartbeatBrokenClient, this, &clientDataProcess::handleHeartbeatBroken);



    // }else{
    //     qDebug()<<"conn fail"<<m_client->errorString();
    //      m_client->disconnect();
    // }


   //  QStringList ipList = getLocalIPAddresses();

   //  m_devInfo.devIP = ipList.at(0);

}
void clientDataProcess::reConnect()
{

    this->m_client->connectToHost(HostInFo.hostIP,HostInFo.hostPort);
    if(m_client->waitForConnected()){
        qDebug()<<"conn ok";
        connect(m_client,&QTcpSocket::readyRead,this,&clientDataProcess::myRead);
        m_pheart= new HeartbeatThread();
        m_pheart->initHeartCounter();
        m_pheart->setHeartType(Heart_Client);

        connect(m_client,&QTcpSocket::readyRead,this,&clientDataProcess::myRead);

        connect(m_client,&QTcpSocket::readyRead,m_pheart,&HeartbeatThread::onResetCounter);

        connect(m_pheart, &HeartbeatThread::sendClientHeartbeatPackage, this, &clientDataProcess::sendClientHeartbeat);


        connect(m_pheart, &HeartbeatThread::heartbeatBrokenClient, this, &clientDataProcess::handleHeartbeatBroken);

        emit sendConnected();

    }else{
        qDebug()<<"conn fail"<<m_client->errorString();
        m_client->disconnect();
         emit sendDisconnected();
    }
}
void clientDataProcess::startHeartBeat()
{
         m_pheart= new HeartbeatThread();
         m_pheart->initHeartCounter();
         m_pheart->setHeartType(Heart_Client);

         connect(m_client,&QTcpSocket::readyRead,m_pheart,&HeartbeatThread::onResetCounter);

         connect(m_pheart, &HeartbeatThread::sendClientHeartbeatPackage, this, &clientDataProcess::sendClientHeartbeat);


         connect(m_pheart, &HeartbeatThread::heartbeatBrokenClient, this, &clientDataProcess::handleHeartbeatBroken);

}

void clientDataProcess::myRead()
{
    QByteArray buffer = m_client->readAll();
    qDebug()<<buffer.data();
    // 只是简单的打印输出，还没有做解包处理

    // 解析JSON数据
    QJsonDocument jsonDoc = QJsonDocument::fromJson(buffer);

    if (jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();
        if (jsonObj.contains("devReqType")) {
            QString reqType = jsonObj["devReqType"].toString();

            if (reqType == "server-Heart-res") {

                QJsonObject heartDetails = jsonObj["HeartDetails"].toObject();
                // int len = heartDetails["len"].toInt();
                int type = heartDetails["type"].toInt();

                if(type == HEART_CHECK_RES){
                    qDebug()<<"收到 ";

                }
            }
        }

        else if(jsonObj.contains("logTypes"))
        {

             QJsonObject logtypesObject = jsonObj["logTypes"].toObject();

            if(logtypesObject.contains("error")){

                 QJsonObject errorTypesObject = logtypesObject["error"].toObject();

                if(errorTypesObject.contains("errorType"))
                 {
                     QString errorInfo = errorTypesObject["errorType"].toString();

                    if(errorInfo == "type1")
                    {
                         emit logType(LOG_FAILED_1);
                         qDebug()<<"登入失败，设备配置信息冲突或不存在 ";

                     }

                    if(errorInfo == "type2")
                    {
                        emit logType(LOG_FAILED_2);
                        qDebug()<<"登入失败，管理员用户名或密码错误 ";


                    }



                 }


            }
            else if(logtypesObject.contains("right")){

                QJsonObject rightTypesObject = logtypesObject["right"].toObject();

                if (rightTypesObject.contains("rightType")) {
                    QString rightType = rightTypesObject["rightType"].toString();

                    if (rightType == "type1") {
                        emit logType(LOG_SUCCESS);
                        emit signInInitSignal();
                        qDebug() << "登入成功";
                    }
                }

            }
            // qDebug()<<"right ";
            // emit logRight();




        }
    }
}



void clientDataProcess::processLoginInfo(QString usr, QString pwd)
{



    QJsonObject usrLogData;
    usrLogData["ACCOUNT"] = usr;
    usrLogData["PASSWORD"] = pwd;
    // 创建包含 machineID、ipAddress 和 Heart 对象的 JSON 数据
    QJsonObject jsonData;
    jsonData["devName"] = m_devInfo.devName;
    jsonData["devSN"] = m_devInfo.devID;
    jsonData["ipAddress"] = m_devInfo.devIP;
    jsonData["devReqType"] = "login";
    jsonData["loginfo"] = usrLogData;
    QJsonDocument jsonDoc(jsonData);
    QByteArray dataToSend = jsonDoc.toJson();
    // 通过TCP连接发送数据
    m_client->write(dataToSend);
}

void clientDataProcess::processComsumeInfo(QString startStationName, QString endStationName, qint16 fare, qint8 tickets)
{


    QJsonObject ticketDetails;
    ticketDetails["start_station"] = startStationName;
    ticketDetails["end_station"] = endStationName;
    ticketDetails["fare"] = fare;
    ticketDetails["tickets"] = tickets;
    // 创建包含 machineID、ipAddress 和 Heart 对象的 JSON 数据
    QJsonObject jsonData;
    jsonData["devName"] = m_devInfo.devName;
    jsonData["devSN"] = m_devInfo.devID;
    jsonData["ipAddress"] = m_devInfo.devIP;
    jsonData["devReqType"] = "TPR";
    jsonData["ticketDetails"] = ticketDetails;
    QJsonDocument jsonDoc(jsonData);
    QByteArray dataToSend = jsonDoc.toJson();
    // 通过TCP连接发送数据
    m_client->write(dataToSend);


}





QStringList clientDataProcess::getLocalIPAddresses()
{
    QStringList ipAddresses;

    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface& iface : interfaces) {
        if (iface.flags().testFlag(QNetworkInterface::IsUp) && !iface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            QList<QNetworkAddressEntry> entries = iface.addressEntries();
            for (const QNetworkAddressEntry& entry : entries) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    ipAddresses.append(entry.ip().toString());
                }
            }
        }
    }

    return ipAddresses;


}








void clientDataProcess::sendClientHeartbeat()
{


    HeartCheckReq req;
    QJsonObject HeartDetails;
    HeartDetails["type"] = req.head.type;
    HeartDetails["len"] = req.head.len;
    // 创建包含 machineID、ipAddress 和 Heart 对象的 JSON 数据
    QJsonObject jsonData;
    jsonData["devName"] = m_devInfo.devName;
    jsonData["devSN"] = m_devInfo.devID;
    jsonData["ipAddress"] = m_devInfo.devIP;
    jsonData["devReqType"] = "Heart";
    jsonData["HeartDetails"] = HeartDetails;
    // 将 JSON 数据转换为 JSON 文档
    QJsonDocument jsonDoc(jsonData);
    QByteArray dataToSend = jsonDoc.toJson();


    m_client->write(dataToSend);

}

void clientDataProcess::handleHeartbeatBroken()
{
     qDebug()<<"断开连接";

    m_client->close();
    m_client->connectToHost(HostInFo.hostIP,HostInFo.hostPort);
    if(m_client->waitForConnected()){
         m_pheart->initHeartCounter(); // 重连成功，重置阈值
        //ui->textBrowser->append("重连成功");

        qDebug()<<"重连成功";
    }
}
