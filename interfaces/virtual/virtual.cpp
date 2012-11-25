/**************************************************************************
    This file is part of Tren Digital.

    Tren Digital is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tren Digital is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/
#include "virtual.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"


QString Virtual::name() { return QString("virtual"); }

QString Virtual::version() { return QString("0.0.1"); }

bool Virtual::read(int *value) { return true; }

bool Virtual::write(int value) { return true; }

bool Virtual::read(BYTE *msg, DWORD bytesToRead) { return true; }

bool Virtual::write(BYTE *msg, DWORD bytesToWrite) { return true; }

bool Virtual::open() { return true; }

void Virtual::close() {}

void Virtual::flush() {}

bool Virtual::isOpen() { return true; }

void Virtual::setDefaultSetup()
{     
    #if defined(Q_OS_WIN)
    m_commPort = "COM1";
    #else
    m_commPort = "/dev/ttyS0";
    #endif

    m_baudRate = 2400;
    m_dataBits = 8;
    m_stopBits = 2;
    m_parityCheck = 0;
    m_handshake = 0;

    m_ip = "";
    m_ipPort = "";
    m_ipProtocol = "";
}

QString Virtual::commPort() { return m_commPort; }

int Virtual::baudRate() { return m_baudRate; }

QString Virtual::ip() { return m_ip; }

void Virtual::setCommPort(QString commPort) { m_commPort = commPort; }

void Virtual::setBaudRate(int baudRate) { m_baudRate = baudRate; }

void Virtual::setIp(QString ip) { m_ip = ip; }

bool Virtual::stop() { return true; }

bool Virtual::go() { return true; }

bool Virtual::setLokoInReverse(int address, bool auxFunc, bool direction) { return true; }

bool Virtual::setLokoSpeedAndLight(int address, int speed, bool light) { return true; }

bool Virtual::setLokoFunctions(int address, bool *functions, int numFunctions) { return true; }

bool Virtual::switchElement(int address, bool on) { return true; }

bool Virtual::readFeedBackModule(int module, bool *status) { return true; }

bool Virtual::readFeedBackModules(bool *status) { return true; }

bool Virtual::restartFeedbackReading() { return true; }

void Virtual::convertLokoSpeed(int speed) {}

bool Virtual::writeLokoInfo(int address) { return true; }

bool Virtual::readLokoInfo(int address) { return true; }

void Virtual::readDirectionAndLightInfo(int byte) {}

void Virtual::writeDirectionAndLightInfo(int *byte) {}

void Virtual::emergencyStop() {}

void Virtual::reverse() {}

// New for TCPIP Interfaces

void Virtual::storeLoko(int address, char *name, int *id) {}

bool Virtual::deleteAllLokos() { return true; }

void Virtual::storeSwitch(int address, int *id) {}

bool Virtual::deleteAllSwitches() { return true; }

int Virtual::getMFXLokosInfo(int *id, int *address, QString *name) { return 0; }

int Virtual::getMFXSID() { return 0; }

Q_EXPORT_PLUGIN2(virtual, Virtual)
