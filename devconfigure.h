#ifndef DEVCONFIGURE_H
#define DEVCONFIGURE_H

#include <QDialog>

namespace Ui {
class DevConfigure;
}

class DevConfigure : public QDialog
{
    Q_OBJECT

public:
    explicit DevConfigure(QWidget *parent = nullptr);
    ~DevConfigure();

private slots:
    void on_pb_config_clicked();

    void on_pb_cancel_clicked();


signals:
    void iniEmpty();
private:
    Ui::DevConfigure *ui;
    void readConfigFile();
    void writeConfigFile();
};

#endif // DEVCONFIGURE_H
