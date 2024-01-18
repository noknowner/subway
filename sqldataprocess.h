#ifndef SQLDATAPROCESS_H
#define SQLDATAPROCESS_H

#include <QObject>
#include<QSqlDatabase>
#include<QPoint>
#include<QRgb>
#include<QMap>
#include<QList>

struct stationInfo {
    QString stationName;
    qint8 isChange;
    QPoint coordinates;
    stationInfo()
    {


    }
};

struct lineInfo {
    qint16 lineNumber;
    qint16 stationNum;
    QRgb rgbR;
    //QList<stationInfo> lineInfoSum;
    QMultiMap<qint16,stationInfo> lineInfoMap;
    lineInfo()
    {


    }
};

class SqlDataProcess : public QObject
{
    Q_OBJECT
public:
    explicit SqlDataProcess(QObject *parent = nullptr);

    QList<stationInfo> stationInfoList;
    QList<lineInfo> lineInfoList;

private:
    void stationInfoExtract();
    void lineInfoExtract();


signals:



private:


    //QSqlDatabase *db;



};

#endif // SQLDATAPROCESS_H
