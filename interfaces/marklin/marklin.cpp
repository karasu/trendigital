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
#include "marklin.h"

QString Marklin::name()
{
    return QString("marklin");
}

QString Marklin::version()
{
    return QString("0.0.1");
}

bool Marklin::read(int *value)
{
    char tmp;

    if (m_serial.read(&tmp, 1) > 0)
    {
        *value = (int)tmp;
        return true;
    }

    return false;
}

bool Marklin::write(int value)
{
    char tmp = (char)value;

    m_serial.flush();

    if (m_serial.write(&tmp, 1) > 0)
    {
        return true;
    }

    return false;
}

bool Marklin::read(BYTE *msg, DWORD bytesToRead)
{
    // not used in this interface
    return false;
}

bool Marklin::write(BYTE *msg, DWORD bytesToWrite)
{
    // not used in this interface
    return false;
}

bool Marklin::open()
{
    return m_serial.open(QextSerialPort::ReadWrite);
}

void Marklin::close()
{
    m_serial.close();
}

void Marklin::flush()
{
    m_serial.flush();
}

bool Marklin::isOpen()
{
    return m_serial.isOpen();
}

void Marklin::setDefaultSetup()
{
    #ifdef _TTY_POSIX_
        m_serial.setPortName("/dev/ttyS0");
    #else
        m_serial.setPortName("COM1");
    #endif

    m_serial.setBaudRate(BAUD2400);
    m_serial.setDataBits(DATA_8);
    m_serial.setStopBits(STOP_2);
    m_serial.setParity(PAR_NONE);
    m_serial.setFlowControl(FLOW_HARDWARE);
    m_serial.setQueryMode(QextSerialPort::Polling);
    m_serial.setTimeout(200); // set timeout to 200 ms
}

bool Marklin::stop()
{
    return write(0x61);
}

bool Marklin::go()
{
    return write(0x60);
}

bool Marklin::setLokoInReverse(int address, bool auxFunc, bool direction)
{
    direction = true; // not used in this interface

    bool ok = true;

    if (address < 1 || address > 80)
    {
        ok = false;
    }

    int aux = 0;

    if (auxFunc)
    {
        aux = 16;
    }

    int val = aux;

    if (!write(val)) ok = false;
    if (!write(address)) ok = false;

    val = 15 + aux;

    if (!write(val)) ok = false;
    if (!write(address)) ok = false;

    return ok;
}

bool Marklin::setLokoSpeedAndLight(int address, int speed, bool light)
{
    // fixme
    return false;
}

bool Marklin::setLokoFunctions(int address, bool *functions, int numFunctions)
{
    // fixme
    return false;
}

bool Marklin::switchElement(int address, bool on)
{
    // fixme
    return false;
}

bool Marklin::readFeedBackModule(int module, bool *status)
{
    // fixme
    return false;
}

bool Marklin::readFeedBackModules(bool *status)
{
    // fixme
    return false;
}

bool Marklin::restartFeedbackReading()
{
    // fixme
    return false;
}

void Marklin::convertLokoSpeed(int speed)
{
    // fixme
}

bool Marklin::writeLokoInfo(int address)
{
    // fixme
    return false;
}

bool Marklin::readLokoInfo(int address)
{
    // fixme
    return false;
}

void Marklin::readDirectionAndLightInfo(int byte)
{
    // fixme
}

void Marklin::writeDirectionAndLightInfo(int *byte)
{
    // fixme
}

void Marklin::emergencyStop(void)
{
    // fixme
}

void Marklin::reverse(void)
{
    // fixme
}

/* **************************
   New for TCPIP Interfaces.
   Not used in this interface
   ************************** */

void Marklin::storeLoko(int address, char *name, int *id)
{
}

bool Marklin::deleteAllLokos()
{
    return false;
}

void Marklin::storeSwitch(int address, int *id)
{
}

bool Marklin::deleteAllSwitches()
{
    return false;
}

int Marklin::getMFXLokosInfo(int *id, int *address, QString *name)
{
    return 0;
}

int Marklin::getMFXSID()
{
    return 0;
}

Q_EXPORT_PLUGIN2(marklin, Marklin)
