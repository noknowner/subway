#include "sqldataprocess.h"
#include <QSqlError>
#include <QSqlQuery>
#include<QColor>
#include<QDebug>
SqlDataProcess::SqlDataProcess(QObject *parent)
    : QObject{parent}
{

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("station.db");
    if (!db.open()) {
        qDebug() << "无法打开数据库";
        return;
    }

    lineInfoExtract();

    //stationInfoExtract();

}

void SqlDataProcess::stationInfoExtract()
{
    // QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    // db.setDatabaseName("station.db");
    // if (!db.open()) {
    //     qDebug() << "无法打开数据库";
    //     return;
    // }
    if (!lineInfoList.isEmpty())
    {
        for (int s = 0; s < lineInfoList.size(); s++)
        {
            QSqlQuery query;
            if (query.exec("SELECT * FROM lineid" + QString::number(lineInfoList.at(s).lineNumber)))
            {
                while (query.next())
                {
                    stationInfo tmpstationInfo;
                    int x = query.value("站点坐标x").toInt();
                            int y = query.value("站点坐标y").toInt();
                          tmpstationInfo.stationName = query.value("站点名称").toString();
                              tmpstationInfo.isChange = query.value("是否换乘").toInt();
                          tmpstationInfo.coordinates = QPoint(x, y);
                              stationInfoList.append(tmpstationInfo);
                    // lineInfo currentLineInfo;
                    // currentLineInfo.lineNumber = lineInfoList.at(s).lineNumber;
                    // currentLineInfo.stationNum = lineInfoList.at(s).stationNum;
                    // currentLineInfo.rgbR = lineInfoList.at(s).rgbR;

                    // currentLineInfo.lineInfoMap.insert(lineInfoList.at(s).lineNumber, tmpstationInfo);

                    // lineInfoList.append(currentLineInfo);

                }
            }
            else
            {
                qDebug() << "查询失败：" << query.lastError().text();
                // 处理查询失败的情况，可以根据需要进行处理
            }
        }
    }
    else
    {
        qDebug() << "路线信息为空";
    }
}


void SqlDataProcess::lineInfoExtract()
{
    QSqlQuery queryinfo;
    if (queryinfo.exec("SELECT * FROM lineinfo")) {
        while (queryinfo.next()) {
            lineInfo tmpLineinfo;
            tmpLineinfo.lineNumber = queryinfo.value("路线号").toInt();
                                         QString routineColor = queryinfo.value("颜色").toString();
                      tmpLineinfo.stationNum = queryinfo.value("站点数量").toInt();
                  tmpLineinfo.rgbR = QColor(routineColor).rgb(); // 将字符串转换为有效的QRgb颜色值

            QSqlQuery query;
            if (query.exec("SELECT * FROM lineid" + QString::number(tmpLineinfo.lineNumber)))
            {
                while (query.next())
                {
                    stationInfo tmpstationInfo;
                    int x = query.value("站点坐标x").toInt();
                    int y = query.value("站点坐标y").toInt();
                    tmpstationInfo.stationName = query.value("站点名称").toString();
                    tmpstationInfo.isChange = query.value("是否换乘").toInt();
                    tmpstationInfo.coordinates = QPoint(x, y);
                    tmpLineinfo.lineInfoMap.insert(tmpLineinfo.lineNumber, tmpstationInfo);
                }
            }
            qDebug() <<tmpLineinfo.lineInfoMap.count(tmpLineinfo.lineNumber);
            lineInfoList.append(tmpLineinfo);
        }
    } else {
        qDebug() << "查询失败：" << queryinfo.lastError().text();
        // 在这里可以处理查询失败的情况
    }
}


