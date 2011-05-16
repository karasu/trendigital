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
#include <math.h>

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
    #if defined(Q_OS_WIN)
        m_serial.setPortName("COM1");
    #else
        m_serial.setPortName("/dev/ttyS0");
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
    bool ok = true;

    if (address < 0 || address > 80 || speed < 0)
    {
        ok = false;
    }

    // Speed goes from 0 to 100. We have to convert it!

    speed = (int)((float)speed * 0.14f);

    int auxFunc = 0;

    if (light)
    {
        auxFunc = 16;
    }

    int val = speed + auxFunc;

    if (!write(val)) { ok = false; }

    if (!write(address)) { ok = false; }

    return ok;
}

bool Marklin::setLokoFunctions(int address, bool *functions, int numFunctions)
{
    bool ok = true;

    if (address < 0 || address > 80)
    {
        ok = false;
    }

    int val = 0;

    // Marklin 6021 only supports 4 functions per loko
    const int elements = 4;

    for (int i=0; i<elements; i++)
    {
        if (functions[i])
        {
            val += (int)pow(2, i);
        }
    }

    val += 64;

    if (!write(val)) ok = false;

    if (!write(address)) ok = false;

    return ok;
}

bool Marklin::switchElement(int address, bool on)
{
    bool ok = true;

    if (address < 0 || address > 255)
    {
        ok = false;
    }

    int val = 33;

    if (on)
    {
        val = 34;
    }

    int stopOp = 32;

    if (!write(val)) ok = false;
    if (!write(address)) ok = false;
    if (!write(stopOp)) ok = false;

    return ok;
}

bool Marklin::readFeedBackModule(int module, bool *status)
{
    bool ok = true;

    if (module < 1 || module > 31)
    {
        ok = false;
    }

    int ask = 192 + module;

    int val1 = 0, val2 = 0;

    for (int i=0; i<16; i++)
    {
        status[i] = false;
    }

    if (!write(ask))  ok = false;
    sleep(50);
    if (!read(&val1)) ok = false;
    sleep(50);
    if (!read(&val2)) ok = false;

    if (ok)
    {
        for (int i=7; i>=0; i--)
        {
            status[i] = val1 % 2;
            val1 = val1 >> 1;

            status[i+8] = val2 % 2;
            val2 = val2 >> 1;
        }
    }

    return ok;
}

bool Marklin::readFeedBackModules(bool *status)
{
    // not available in this interface
    return false;
}

bool Marklin::restartFeedbackReading()
{
    // not available in this interface
    return false;
}

void Marklin::convertLokoSpeed(int speed)
{
    // not available in this interface
}

bool Marklin::writeLokoInfo(int address)
{
    // not available in this interface
    return false;
}

bool Marklin::readLokoInfo(int address)
{
    // not available in this interface
    return false;
}

void Marklin::readDirectionAndLightInfo(int byte)
{
    // not available in this interface
}

void Marklin::writeDirectionAndLightInfo(int *byte)
{
    // not available in this interface
}

void Marklin::emergencyStop(void)
{
    // stop?
}

void Marklin::reverse(void)
{
    // fixme
}

void Marklin::storeLoko(int address, char *name, int *id)
{
    // not available in this interface
}

bool Marklin::deleteAllLokos()
{
    // not available in this interface
    return false;
}

void Marklin::storeSwitch(int address, int *id)
{
    // not available in this interface
}

bool Marklin::deleteAllSwitches()
{
    // not available in this interface
    return false;
}

int Marklin::getMFXLokosInfo(int *id, int *address, QString *name)
{
    // not available in this interface
    return 0;
}

int Marklin::getMFXSID()
{
    // not available in this interface
    return 0;
}

Q_EXPORT_PLUGIN2(marklin, Marklin)
