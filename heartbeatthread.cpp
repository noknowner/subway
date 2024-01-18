#include "heartbeatthread.h"
#include <QDebug>
#include <QApplication>
#include<QTime>
HeartbeatThread::HeartbeatThread(): m_pThread(nullptr)
    ,m_heartType(Heart_NULL),m_nCounter(0),m_pTimer(nullptr)
{
    init();
    connectPart();
}

HeartbeatThread::~HeartbeatThread()
{
    if (m_pTimer->isActive()) {
        m_pTimer->stop();
    }

    m_pTimer->deleteLater();
    m_pTimer = nullptr;

    m_pThread->quit();
    m_pThread->wait();
    m_pThread->deleteLater();
}


void HeartbeatThread::init()
{
    m_pThread = new QThread;
    m_pTimer = new QTimer;
    m_pTimer->setInterval(TIMER_OUT_T);    //1.5秒启动一次
    m_pTimer->moveToThread(m_pThread);
    moveToThread(m_pThread);
    m_pThread->start();
}

void HeartbeatThread::connectPart()
{
    connect(m_pThread, &QThread::started, this, &HeartbeatThread::threadStartRun, Qt::DirectConnection);
    connect(m_pThread, &QThread::finished, this, &HeartbeatThread::threadStopRun, Qt::DirectConnection);
    connect(m_pTimer, &QTimer::timeout, this, &HeartbeatThread::onTimerout);
}


void HeartbeatThread::setHeartType(HEART_TYPE hType)
{
    m_heartType = hType;
}


void HeartbeatThread::initHeartCounter()
{

        m_nCounter = 0;

}



void HeartbeatThread::onInsertCounterChanged(int nPtr)
{

    //qDebug() <<nPtr;
    m_nCounter=nPtr;

}

void HeartbeatThread::onResetCounter()
{
    m_nCounter = 0;
}


void HeartbeatThread::threadStartRun()
{
    qDebug() << "HeartbeatThread thread start running, thread ID: " << QThread::currentThreadId();
    m_pTimer->start();
}

void HeartbeatThread::threadStopRun()
{
    if(m_pTimer->isActive()){
        qDebug() << "delete timer...";
        m_pTimer->stop();
    }
    m_pTimer->deleteLater();
    m_pTimer = nullptr;
    qDebug() << "HeartbeatThread thread has exited, thread ID: " << QThread::currentThreadId();

    emit threadFinished();

}

void HeartbeatThread::destoryResource()
{
    m_pThread->quit();
    m_pThread->wait();
}

void HeartbeatThread::onTimerout()
{
    if(Heart_Client == m_heartType){

       // qDebug() <<"m_nCounter:"<<m_nCounter;
        ++m_nCounter;



        if(m_nCounter > HEART_TIME){


            emit heartbeatBrokenClient();
            //qDebug() <<"broken";
            //超过3次，进行推送断掉
        }
        else if (m_nCounter <= HEART_TIME) {



        }
        qDebug()<<QString("当前时间:%1 心跳阈值为 %2").arg(QTime::currentTime().toString()).arg(m_nCounter);

        emit sendClientHeartbeatPackage();
        qDebug() <<"send";
        //onResetCounter();
        //qDebug() << "HeartbeatThread thread is running, thread ID: " << QThread::currentThreadId();
    }

    else{
        qDebug() << "hearbeat is null...";
    }

}
