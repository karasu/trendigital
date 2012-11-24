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
    along with Tren Digital.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/
#ifndef TRAININTERFACE_H
#define TRAININTERFACE_H

#include <QString>
#include <QMutex>
#include <QMutexLocker>
#include <QtPlugin>

#define BYTE quint8
#define DWORD quint32

// #define block() QMutexLocker locker(&mutex)

#define isBitSet(bit,byte) ((byte>>bit)&0x01)

// These are outdated, use switchElement instead
// virtual bool switchDetour(int address, bool branch) = 0;
// virtual bool switchSemaphore(int address, bool red) = 0;

#ifdef Q_OS_WIN
#include <windows.h> // for Sleep
#endif

class TrainInterface
{
public:
    virtual ~TrainInterface() { }

    virtual QString name() = 0;
    virtual QString version() = 0;

    virtual bool read(int *value) = 0;
    virtual bool write(int value) = 0;

    virtual bool read(BYTE *msg, DWORD bytesToRead) = 0;
    virtual bool write(BYTE *msg, DWORD bytesToWrite) = 0;

    virtual bool open(void) = 0;
    virtual void close(void) = 0;
    virtual void flush(void) = 0;
    virtual bool isOpen(void) = 0;

    virtual void setDefaultSetup(void) = 0;

    virtual bool stop(void) = 0;
    virtual bool go(void) = 0;

    virtual bool setLokoInReverse(int address, bool auxFunc, bool direction) = 0;

    inline bool setLokoSpeed(int address, int speed)
    {
        return setLokoSpeedAndLight(address, speed, false);
    }

    virtual bool setLokoSpeedAndLight(int address, int speed, bool light=false) = 0;
    virtual bool setLokoFunctions(int address, bool *functions, int numFunctions) = 0;

    virtual bool switchElement(int address, bool on) = 0;

    virtual bool readFeedBackModule(int module, bool *status) = 0;
    virtual bool readFeedBackModules(bool *status) = 0;

    virtual bool restartFeedbackReading(void) = 0;

    virtual void convertLokoSpeed(int speed) = 0;
    virtual bool writeLokoInfo(int address) = 0;
    virtual bool readLokoInfo(int address) = 0;
    virtual void readDirectionAndLightInfo(int byte) = 0;
    virtual void writeDirectionAndLightInfo(int *byte) = 0;
    virtual void emergencyStop(void) = 0;
    virtual void reverse(void) = 0;

    // New for TCPIP Interfaces

    virtual void storeLoko(int address, char *name, int *id) = 0;
    virtual bool deleteAllLokos(void) = 0;

    virtual void storeSwitch(int address, int *id) = 0;
    virtual bool deleteAllSwitches(void) = 0;

    virtual int getMFXLokosInfo(int *id, int *address, QString *name) = 0;
    virtual int getMFXSID(void) = 0;

    void sleep(int ms)
    {
        if (ms > 0)
        {
    #ifdef Q_OS_WIN
            Sleep(uint(ms));
    #else
            struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
            nanosleep(&ts, NULL);
    #endif
        }
    }

    // Properties

    QMutex m_mutex;
    QString m_COMMPort;
    int m_baudRate;
    int m_stopBits;
    int m_dataBits;
    int m_parityCheck;
    int m_handshake;

    QString m_ip;
    QString m_ipPort;
    QString m_ipProtocol;
};


Q_DECLARE_INTERFACE(TrainInterface, "com.egarasyg.trendigital.traininterface/1.0");

#endif // TRAININTERFACE_H
