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

bool Marklin::read(int *value, bool sleep)
{
    // fixme
    return false;
}

bool Marklin::write(int value, bool sleep, bool purge)
{
    // fixme
    return false;
}

bool Marklin::read(BYTE *msg, DWORD bytesToRead, bool sleep)
{
    // fixme
    return false;
}

bool Marklin::write(BYTE *msg, DWORD bytesToWrite, bool sleep, bool purge)
{
    // fixme
    return false;
}

void Marklin::open()
{
    // fixme
}

void Marklin::close()
{
    // fixme
}

void Marklin::purge()
{
    // fixme
}

bool Marklin::isOpen()
{
    // fixme
    return false;
}

void Marklin::setDefaultSetup()
{
    // fixme
}

bool Marklin::stop()
{
    // fixme
    return false;
}

bool Marklin::go()
{
    // fixme
    return false;
}

bool Marklin::setLokoInReverse(int address, bool auxFunc, bool direction)
{
    // fixme
    return false;
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

// New for TCPIP Interfaces

void Marklin::storeLoko(int address, char *name, int *id)
{
    // fixme
}

bool Marklin::deleteAllLokos()
{
    // fixme
    return false;
}

void Marklin::storeSwitch(int address, int *id)
{
    // fixme
}

bool Marklin::deleteAllSwitches()
{
    // fixme
    return false;
}

int Marklin::getMFXLokosInfo(int *id, int *address, QString *name)
{
    // fixme
    return 0;
}

int Marklin::getMFXSID()
{
    // fixme
    return 0;
}

Q_EXPORT_PLUGIN2(marklin, Marklin)
