#include "devconfigure.h"
#include "ui_devconfigure.h"
#include"datastruct.h"
#include <QSettings>

devInfo m_devInfo;
hostInfo HostInFo;
DevConfigure::DevConfigure(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DevConfigure)
{
    ui->setupUi(this);
    // ui->lineEdit_setDevName->setText(m_devInfo.devName);
    // ui->lineEdit_setDevSN->setText(m_devInfo.devID);
    // ui->lineEdit_setIP->setText(HostInFo.hostIP);
    // ui->lineEdit_setPort->setText(QString::number( HostInFo.hostPort));
    readConfigFile();
}

DevConfigure::~DevConfigure()
{
    delete ui;
}

void DevConfigure::readConfigFile() {
    QSettings settings(":/res/config/config.ini", QSettings::IniFormat);

    // 检查配置文件中是否存在数据，如果存在则读取
    if (!settings.allKeys().isEmpty()) {
        m_devInfo.devID = settings.value("DeviceID").toString();
        m_devInfo.devName = settings.value("DeviceName").toString();
        HostInFo.hostIP = settings.value("HostIP").toString();
        HostInFo.hostPort = settings.value("HostPort").toInt();
        ui->lineEdit_setDevSN->setText(m_devInfo.devID);
        ui->lineEdit_setDevName->setText(m_devInfo.devName);
        ui->lineEdit_setIP->setText(HostInFo.hostIP);
        ui->lineEdit_setPort->setText(QString::number(HostInFo.hostPort));

    }
    else {  // 如果配置文件为空，填入默认值
        ui->lineEdit_setDevSN->setText("");
        ui->lineEdit_setDevName->setText("");
        ui->lineEdit_setIP->setText("");
        ui->lineEdit_setPort->setText("");
        emit iniEmpty();

    }

    // 更新 UI

}

void DevConfigure::writeConfigFile() {
    QSettings settings(":/res/config/config.ini", QSettings::IniFormat);

    // 写入配置信息
    settings.setValue("DeviceID", m_devInfo.devID);
    settings.setValue("DeviceName", m_devInfo.devName);
    settings.setValue("HostIP", HostInFo.hostIP);
    settings.setValue("HostPort", HostInFo.hostPort);
}

void DevConfigure::on_pb_config_clicked() {
   // readConfigFile(); // 读取配置文件

    // 获取用户输入
    m_devInfo.devID = ui->lineEdit_setDevSN->text();
    m_devInfo.devName = ui->lineEdit_setDevName->text();
    HostInFo.hostIP = ui->lineEdit_setIP->text();
    HostInFo.hostPort = ui->lineEdit_setPort->text().toInt();

    writeConfigFile(); // 将用户输入写入配置文件
}

void DevConfigure::on_pb_cancel_clicked()
{
    close();
}

