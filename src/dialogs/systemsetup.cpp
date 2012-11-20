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

    // ui->digitalSystem

    ui->port->insertItems(0, ports);
    ui->transferSpeed->insertItems(0, transferSpeeds);

    ui->dial->setMinimum(0);
    ui->dial->setMaximum(2000);

}

SystemSetup::~SystemSetup()
{
    delete ui;
}
