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
{}

bool Marklin::read(BYTE *msg, DWORD bytesToRead, bool sleep)
{}

bool Marklin::write(BYTE *msg, DWORD bytesToWrite, bool sleep, bool purge)
{}

void Marklin::open()
{}

void Marklin::close()
{}

void Marklin::purge()
{}

bool Marklin::isOpen()
{}

void Marklin::setDefaultSetup()
{}

bool Marklin::stop()
{}

bool Marklin::go()
{}

bool Marklin::setLokoInReverse(int address, bool auxFunc, bool direction)
{}

bool Marklin::setLokoSpeedAndLight(int address, int speed, bool light)
{}

bool Marklin::setLokoFunctions(int address, bool *functions, int numFunctions)
{}

bool Marklin::switchElement(int address, bool on)
{}

bool Marklin::readFeedBackModule(int module, bool *status)
{}

bool Marklin::readFeedBackModules(bool *status)
{}

bool Marklin::restartFeedbackReading()
{}

void Marklin::convertLokoSpeed(int speed)
{}

bool Marklin::writeLokoInfo(int address)
{}

bool Marklin::readLokoInfo(int address)
{}

void Marklin::readDirectionAndLightInfo(int byte)
{}

void Marklin::writeDirectionAndLightInfo(int *byte)
{}

void Marklin::emergencyStop(void)
{}

void Marklin::reverse(void)
{}

// New for TCPIP Interfaces

void Marklin::storeLoko(int address, char *name, int *id)
{}

bool Marklin::deleteAllLokos()
{}

void Marklin::storeSwitch(int address, int *id)
{}

bool Marklin::deleteAllSwitches()
{}

int Marklin::getMFXLokosInfo(int *id, int *address, QString *name)
{}

int Marklin::getMFXSID()
{}

Q_EXPORT_PLUGIN2(marklin, Marklin)
