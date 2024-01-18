/*********************************************************************************
*Author:zhouFuLiang
*Date: 2023-03-21  10:45
*Description: 心跳包线程类 每次隔开指定的时间进行一次心跳发送 收到回复即清零 未收到即+1 如此
* 设定指定的时间触发一次，指定的时间次数即可判断是否断开连接
**********************************************************************************/
#ifndef HEARTBEATTHREAD_H
#define HEARTBEATTHREAD_H

#define  HEART_TIME     3               //心跳次数
#define  TIMER_OUT_T    5000           //timer设置的时常

#include <QObject>
#include <QThread>
#include <QMap>
#include <QTimer>

enum HEART_TYPE{
    Heart_NULL = 0,
    Heart_Server,
    Heart_Client,
};

class HeartbeatThread : public QObject
{
    Q_OBJECT
public:
    HeartbeatThread();
    ~HeartbeatThread();
    /*
     * @brief 设置当前心跳包为何种心跳（服务端创建即Heart_Server 客户端创建即Heart_Client）
     * @param hType 心跳包类型
     * @param
     * @return
    */
    void setHeartType(HEART_TYPE hType);
    /*
     * @brief 初始化心跳包计数器容器 全部置0
     * @param
     * @param
     * @return
    */
    void initHeartCounter();
    //void threadFinished();
signals:
    //心跳包断掉(服务器收到指定客户端断掉)
    void heartbeatBrokenSer(int nPtr);
    //心跳包断掉（客户端自己断掉了）
    void heartbeatBrokenClient();

    //服务器发送心跳包
    void sendServHeartbeatPackage();
    //客户端发送心跳包
    void sendClientHeartbeatPackage();

    void threadFinished();


private:
    void init();
    void connectPart();

public slots:
    /*
     * @brief 当客户端连接时，将插入一个计数器，对应ptr
     * @param ptr客户端唯一标识
     * @param
     * @return
    */
    void onInsertCounterChanged(int nPtr);
    /*
     * @brief 将计数器置零(针对如果是客户端时)
     * @param
     * @param
     * @return
    */
    void onResetCounter();
    void destoryResource();

private slots:
    void threadStartRun();
    void threadStopRun();


    void onTimerout();

private:
    QThread *m_pThread;
    HEART_TYPE m_heartType;
    int m_nCounter;                    //计数器（client）
    QTimer *m_pTimer;
};

#endif // HEARTBEATTHREAD_H
