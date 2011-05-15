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

bool Virtual::read(int *value, bool sleep) { return true; }

bool Virtual::write(int value, bool sleep, bool purge) { return true; }

bool Virtual::read(BYTE *msg, DWORD bytesToRead, bool sleep) { return true; }

bool Virtual::write(BYTE *msg, DWORD bytesToWrite, bool sleep, bool purge) { return true; }

void Virtual::open() {}

void Virtual::close() {}

void Virtual::purge() {}

bool Virtual::isOpen() { return true; }

void Virtual::setDefaultSetup() {}

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
