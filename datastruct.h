#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include<QString>

enum TypeInfo{
    HEART_CHECK_REQ,   // 心跳检测请求
    HEART_CHECK_RES,   // 心跳检测响应
};
enum logType{
    LOG_SUCCESS,   // 登入成功

    LOG_FAILED_1,   // 登入失败，设备配置信息冲突或不存在

    LOG_FAILED_2  //登入失败，管理员用户名或密码错误
};
struct Head
{
    int type;
    int len;
};

struct devInfo
{
    QString devIP;
    QString devID;
    QString devName;


    devInfo()
    {

        devIP="192.168.1.1";
        devID="1314213";
        devName = "client-01";


    }
};



struct hostInfo
{
    QString hostIP;
    qint64 hostPort;
    hostInfo() {

        hostIP = "127.0.0.1";
        hostPort = 8898;

    }
};

extern hostInfo HostInFo;

struct HeartCheckReq
{
    Head head;
    devInfo machineInfo;
    HeartCheckReq() {
        head.type = HEART_CHECK_REQ;
        head.len = sizeof(HeartCheckReq);
    }
};


#endif // DATASTRUCT_H
