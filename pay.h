#ifndef PAY_H
#define PAY_H

#include <QWidget>
namespace Ui {
class pay;
}

class pay : public QWidget
{
    Q_OBJECT

public:
    explicit pay(QWidget *parent = nullptr);
    ~pay();

public:

 signals:
     void sendComsumeInfo(QString startStationName,QString endStationName,qint16 fare,qint8 tickets);

 public slots:
     void initialPayDialog();
     void setPath(const QString &path);
     void setMoneyLabel(const qint8 &money);
     void targetStation(QString startStationName,QString endStationName);


 private slots:
     void on_ticket1_pb_clicked();

     void on_ticket2_pb_clicked();

     void on_exit_pb_clicked();

     void on_pb_pay_clicked();

 private:
     qint8 tickets=1;
     qint16  cost;
     QString startS;
     QString endS;
     //clientDataProcess *clientDP2;

 private:
    Ui::pay *ui;


};

#endif // PAY_H
