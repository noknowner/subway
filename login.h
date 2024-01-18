#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include<QTcpSocket>
#include"devconfigure.h"
namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
public slots:

     void logAccept(int i);
     void on_pushButton_signUp_clicked();
     void canLog();
     void cannotLog();

private slots:


    void on_pushButton_clear_clicked();





    void on_pushButton_Setting_clicked();

    void setPbEnable();

    void on_pushButton_reconnect_clicked();

private:
    void funcInit(); //功能初始化
    void readJson();
    void writeJson();
signals:

    void sendLoginInfo(QString usr,QString pwd);
    void logSignal();

    void sendReconnect();
private:
    Ui::login *ui;

    QString m_username;
    QString m_password;
    //QTcpSocket *tcps;

    DevConfigure *devconfig;



};

#endif // LOGIN_H
