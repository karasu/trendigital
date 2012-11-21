#include "systemsetup.h"
#include "ui_systemsetup.h"

#include <QString>
#include <QStringList>

SystemSetup::SystemSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SystemSetup)
{
    ui->setupUi(this);

    QStringList ports;
    ports << "COM1" << "COM2" << "COM3" << "COM4" << "COM5" << "COM6" << "COM7" << "COM8" << "COM9" << "COM10";

    QStringList transferSpeeds;
    transferSpeeds << "2400 bps" << "4800 bps" << "9600 bps" << "16457 bps" << "16600 bps" << "19200 bps" << "38400 bps" << "57600 bps" << "115200 bps";

    ui->port->insertItems(0, ports);
    ui->transferSpeed->insertItems(0, transferSpeeds);

    ui->dial->setMinimum(0);
    ui->dial->setMaximum(2000);
}

QString SystemSetup::pluginName()
{
    return ui->digitalSystem->currentText();
}

void SystemSetup::setPluginNames(QStringList pluginNameList, QString pluginName)
{
    ui->digitalSystem->clear();

    ui->digitalSystem->insertItems(0, pluginNameList);

    int index = ui->digitalSystem->findText(pluginName);

    ui->digitalSystem->setCurrentIndex(index);
}

void SystemSetup::setCommPort(QString port)
{
    int index = ui->port->findText(port);
    ui->port->setCurrentIndex(index);
}

void SystemSetup::setSpeed(int baudRate)
{
    int index = ui->transferSpeed->findText(QString::number(baudRate), Qt::MatchContains);
    ui->transferSpeed->setCurrentIndex(index);
}

SystemSetup::~SystemSetup()
{
    delete ui;
}
