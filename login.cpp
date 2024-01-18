#include "login.h"
#include "ui_login.h"
#include<QMessageBox>
#include<QFile>
#include<QJsonDocument>
#include<QJsonObject>
#include <QByteArray>
#include <QDebug>
#include <QSettings>
#include<QJsonArray>
#include"datastruct.h"
#include<QMessageBox>
login::login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);
    ui->pushButton_signUp->setEnabled(false);
    funcInit();



}

login::~login()
{


    delete ui;

}



void login::funcInit()
{
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);//输入的时候就显示圆点 
    readJson();
    QSettings settings("config.ini", QSettings::IniFormat);

    // 检查配置文件中是否存在数据，如果存在则读取
    if (!settings.allKeys().isEmpty()){

        ui->pushButton_signUp->setEnabled(true);

    }else {
        ui->pushButton_signUp->setEnabled(false);
    }

}

void login::readJson()
{

    QFile file(":/res/json/config.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "文件打开失败!";
        return;
    } else {
        qDebug() <<"文件打开成功!";
    }

    // 读取JSON数据
    QByteArray fileData = file.readAll();
    QJsonDocument jdc = QJsonDocument::fromJson(fileData);
    QJsonObject obj = jdc.object();

    // 查找名为 "info" 的数组
    QJsonArray infoArray = obj.value("info").toArray();

    // 遍历数组以获取信息
    QString account;
    QString password;
    for (const QJsonValue& value : infoArray) {
        QJsonObject data = value.toObject();
        QString type = data.value("type").toString();
        QString vaalue = data.value("value").toString();

        if (type == "ACCOUNT") {
            account = vaalue;
            ui->checkBox_usr->setChecked(true);
        } else if (type == "PASSWORD") {
            password = vaalue;
            ui->checkBox_pwd->setChecked(true);
        }
    }

    // 将读取到的账户和密码信息设置到UI元素
    ui->lineEdit_username->setText(account);
    ui->lineEdit_password->setText(password);
    file.close();


}

void login::writeJson()
{
    QFile file(":/res/json/config.json");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "文件打开失败!";
        return;

    }
    else
    {
        qDebug() <<"文件打开成功!";
    }

    // 读取现有JSON数据
    QByteArray fileData = file.readAll();
    QJsonDocument jdc = QJsonDocument::fromJson(fileData);
    QJsonObject obj = jdc.object();

    // 创建一个数组并添加信息
    QJsonArray accountInfo;

    bool flag = ui->checkBox_usr->isChecked();
    if (flag)
    {
        QJsonObject accountData;
        accountData["type"] = "ACCOUNT";
        accountData["value"] = ui->lineEdit_username->text();
        accountInfo.append(accountData);
    }

    flag = ui->checkBox_pwd->isChecked();
    if (flag)
    {
        QJsonObject passwordData;
        passwordData["type"] = "PASSWORD";
        passwordData["value"] = ui->lineEdit_password->text();
        accountInfo.append(passwordData);
    }

    // 将数组添加到JSON对象
    obj["info"] = accountInfo;

    // 将整个JSON对象写入文件
    jdc = QJsonDocument(obj);
    file.seek(0);
    file.write(jdc.toJson());
    file.resize(file.pos());
    file.flush();
    file.close();
}

void login::on_pushButton_signUp_clicked()
{
    QString name = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    // 发出登录信号
    emit logSignal();
    emit sendLoginInfo(name,password);

}

void login::canLog()
{
    ui->pushButton_signUp->setEnabled(true);
    ui->pushButton_reconnect->setEnabled(false);

}

void login::cannotLog()
{
    ui->pushButton_signUp->setEnabled(false);
    ui->pushButton_reconnect->setEnabled(true);
}


void login::on_pushButton_clear_clicked()
{
    ui->lineEdit_password->clear();
    ui->lineEdit_username->clear();
}

void login::logAccept(int i)
{
    if(i == LOG_SUCCESS){

        writeJson();
        ui->pushButton_signUp->setEnabled(false);
        hide();

    }

    else if(i == LOG_FAILED_1)
    {
       QMessageBox::critical(nullptr, "错误", "登入失败，设备配置信息冲突或不存在");


    }

    else if(i == LOG_FAILED_2)
    {
         QMessageBox::critical(nullptr, "错误", "登入失败，管理员用户名或密码错误 ");


    }





}


void login::on_pushButton_Setting_clicked()
{
    DevConfigure *devconfig = new DevConfigure(this);
   // connect(devconfig,&DevConfigure::iniEmpty,this,&login::setPbEnable);
    devconfig->show();
}

void login::setPbEnable()
{

    ui->pushButton_signUp->setEnabled(false);


}


void login::on_pushButton_reconnect_clicked()
{
    emit sendReconnect();
}

