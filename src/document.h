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
#ifndef DOCUMENT_H
#define DOCUMENT_H

// #include <QObject>
#include <QMap>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QTextStream>

class Route;
class Program;
class Loko;
class MainWindow;

#define InsertTextInfo _ITI_

typedef struct InsertTextInfo {
    QRectF rect;
    QString text;
    bool selected;

    InsertTextInfo()
    {
        rect.setRect(0,0,0,0);
        text = "";
        selected = false;
    }
} InsertTextInfo;

class Document // : public QObject
{
public:
    Document();
    virtual ~Document();

    void clear(void);
    bool load(const QString &fileName);
    bool save(void);
    bool saveAs(const QString &fileName);

    inline bool isModified() { return m_modified; }
    inline void setModified(void) { m_modified = true; }

    inline QString getFileName(void) { return m_fileName; }

    QString getLokoImageFileName(int lokoId);

    void setFBModuleStatus(int i, bool status);
    void setAddressStatus(int i, bool status);

    inline bool isAddressAvailable(int i) { return !m_address.value(i, true); }

    inline bool isFBModuleAvailable(int i) { return !m_FBModuleNumber.value(i, true); }

    QSize getDocumentSize(void);

    inline QString get74460Code(int address) { return m_74460Codes.value(address, ""); }
    inline QString getK83K84Code(int address) { return m_K83K84Codes.value(address, ""); }
    inline bool load74460Codes() { return loadCodes("74460.dat", &m_74460Codes); }
    inline bool loadK83K84Codes() { return loadCodes("k83k84.dat", &m_K83K84Codes); }
    bool loadCodes(QString fileName, QList<QString> *lines);

    void deleteInsertedText(int index);

    // System setup
    QString getDigitalSystem(void);
    QString getInterfaceCommPort(void);
    QString getInterfaceSpeed(void);
    int getReadingSpeed(void);

    QString getInterfaceIPAddress(void);
    QString getInterfaceTCPPort(void);
    QString getInterfaceIPProtocol(void);

    bool isUsingIntelliboxInterface(void);
    bool isTimeInMinutes(void);

    void setDigitalSystem(QString digitalSystem);
    void setInterfaceCommPort(QString port);
    void setInterfaceSpeed(QString speed);
    void setReadingSpeed(int speed);
    void setUsingIntelliboxInterface(bool status);

    void setInterfaceIPAddress(QString IPAddress);
    void setInterfaceTCPPort(QString TCPPort);
    void setInterfaceIPProtocol(QString protocol);

    void setTimeInMinutes(bool timeUnit);

    int getZoom(void);
    void setZoom(int zoom);

    // Sempre és del programa actiu 'm_currentProgram'
    int getNumRoutes(void);
    Route getRouteCopy(int route);
    Route *getRoute(int route);
    Route *getActiveRoute(void);
    Route *getRouteByLine(int line);
    void addRoute(Route r);
    void deleteRouteByLine(int line);
    void deleteRoute(Route *pRoute);
    void insertRoute(Route r);

    int getNumPrograms(void);
    Program * getCurrentProgram(void);
    Program * getProgram(int i);
    int getProgramByName(QString name);
    void addProgram(Program p, bool *pAdded=NULL);
    void setCurrentProgramByName(QString name);
    void setCurrentProgramByPos(int index);

    bool isElementNumberUsed(int elementNumber);

    Loko *getLoko(int lokoId);
    Loko *getLoko(QString name);
    QString getLokoName(int id);
    inline int getNumLokos(void);
    int getLokoId(QString name);
    void deleteLoko(int id);

    void deleteProgram(int index);
    void deleteProgramByName(QString name);

    void unzipFile(QString fileName);
    void zipFile(QString fileName);

    // For ecos and Märklín CS1
    void deleteAllInterfaceLokos(void);
    void deleteAllInterfaceSwitches(void);

    void saveThumbnailFile(QString fileName);

    void setDefaultInterfaceValues(void);

public:
    QMap<int,Loko *> m_lokos;
    QMap<int,bool> m_FBModuleNumber;
    QMap<int,bool> m_address;

    // Els decoders són de 8, 16, o 32 posicions (en mfc eren 3 booleans)
    int m_decoderPositions;

    // Will be true if a feedback module is signaled by one train
    QMap<int, bool> m_isFBModuleActive;

    QList<InsertTextInfo> m_insertedTextInfoList;

    // Guardem els punts de les línies (dos punts per línia ;))
    // TurnTableLines m_turnTableLines;

    QList<int> m_turnTablePositions; // list of encoded positions,sort by turntable, max 24 positions

    bool m_firstPositionEncoded;

    bool m_keyboard15;

    QList<QString> m_74460Codes;
    QList<QString> m_K83K84Codes;

    QList<Program> m_programs;

    QList<QString> m_programsOrder;

    int m_orderIndex;

    int m_currentProgram;

signals:

public slots:

private:
    bool m_modified;
    QString m_fileName;

 };

#endif // DOCUMENT_H
