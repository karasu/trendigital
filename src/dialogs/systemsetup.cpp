#include "systemsetup.h"
#include "ui_systemsetup.h"

#include <QString>
#include <QStringList>
#include <QDebug>

#include "../traininterface.h"

SystemSetup::SystemSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SystemSetup)
{
    ui->setupUi(this);

    QStringList ports;

#if defined(Q_OS_WIN)
    ports << "COM1" << "COM2" << "COM3" << "COM4" << "COM5" << "COM6" << "COM7" << "COM8" << "COM9" << "COM10";
#else
    ports << "/dev/ttyS0" << "/dev/ttyS1" << "/dev/ttyS2" << "/dev/ttyS3" << "/dev/ttyS4" << "/dev/ttyS5" << "/dev/ttyS6" << "/dev/ttyS7" << "/dev/ttyS8" << "/dev/ttyS9";
#endif

    QStringList transferSpeeds;
    transferSpeeds << "2400 bps" << "4800 bps" << "9600 bps" << "16457 bps" << "16600 bps" << "19200 bps" << "38400 bps" << "57600 bps" << "115200 bps";

    ui->COMMPort->insertItems(0, ports);
    ui->baudRate->insertItems(0, transferSpeeds);

    ui->dial->setMinimum(0);
    ui->dial->setMaximum(4000);
}

QString SystemSetup::pluginName()
{
    return ui->digitalSystem->currentText();
}

int SystemSetup::readingDelay()
{
    return ui->dial->value();
}

void SystemSetup::setReadingDelay(int value)
{
    ui->dial->setValue(value);
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
    int index = ui->COMMPort->findText(port);
    ui->COMMPort->setCurrentIndex(index);
}

void SystemSetup::setSpeed(int baudRate)
{
    int index = ui->baudRate->findText(QString::number(baudRate), Qt::MatchContains);
    ui->baudRate->setCurrentIndex(index);
}

SystemSetup::~SystemSetup()
{
    delete ui;
}

void SystemSetup::saveSettings(TrainInterface *interface)
{
    Q_ASSERT(interface->name() == ui->digitalSystem->currentText());

    // save interface settings

    interface->setBaudRate(ui->baudRate->currentText().remove(" bps").toInt());
    interface->setCommPort(ui->COMMPort->currentText());
    // interface->m_dataBits =
    // interface->m_handshake =
    // interface->m_parityCheck =
    // interface->m_stopBits =
    interface->setIp(ui->ip->text());
    // interface->m_ipProtocol =
    // interface->m_ipPort =
}
