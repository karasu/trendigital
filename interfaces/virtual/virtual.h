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
#ifndef VIRTUAL_H
#define VIRTUAL_H

#include <QObject>
#include <QString>
#include <QtPlugin>

#include "../../TrenDigital/src/traininterface.h"

class Virtual : public QObject, public TrainInterface
{
    Q_OBJECT
    Q_INTERFACES(TrainInterface)

public:
    QString name();
    QString version();

    bool read(int *value, bool sleep=false);
    bool write(int value, bool sleep=false, bool purge=true);

    bool read(BYTE *msg, DWORD bytesToRead, bool sleep=true);
    bool write(BYTE *msg, DWORD bytesToWrite, bool sleep=true, bool purge=true);

    void open(void);
    void close(void);
    void purge(void);
    bool isOpen(void);

    void setDefaultSetup(void);

    bool stop(void);
    bool go(void);

    bool setLokoInReverse(int address, bool auxFunc, bool direction);

    bool setLokoSpeedAndLight(int address, int speed, bool light=false);
    bool setLokoFunctions(int address, bool *functions, int numFunctions);

    bool switchElement(int address, bool on);

    bool readFeedBackModule(int module, bool *status);
    bool readFeedBackModules(bool *status);

    bool restartFeedbackReading(void);

    void convertLokoSpeed(int speed);
    bool writeLokoInfo(int address);
    bool readLokoInfo(int address);
    void readDirectionAndLightInfo(int byte);
    void writeDirectionAndLightInfo(int *byte);
    void emergencyStop(void);
    void reverse(void);

    // New for TCPIP Interfaces

    void storeLoko(int address, char *name, int *id);
    void deleteAllLokos(void);

    void storeSwitch(int address, int *id);
    void deleteAllSwitches(void);

    int getMFXLokosInfo(int *id, int *address, QString *name);
    int getMFXSID(void);

};

#endif // VIRTUAL_H