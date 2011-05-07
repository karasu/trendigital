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
#ifndef ROUTE_H
#define ROUTE_H

#include <QString>
#include <QList>
#include <QTime>

class LokoCommands;

class DetourSignalCondition;
class FBModuleCondition;
class TransferTableCondition;
class TurnTableCondition;

class Document;

class Route
{
public:
    Route();
    virtual ~Route();

    inline int getLine() { return m_line; }
    inline int getLokoId() { return m_lokoId; }
    inline float getTime() { return m_time; }
    // Obté el temps que ha trigat a fer la ruta
    inline float getEndTime() { return m_endTime; }
    inline int getRelativeLine() { return m_relativeLine; }

    inline LokoCommands getLokoCommands(int i) { return m_lokoCommands.at(i); }

    inline bool isDone() { return m_done; }
    inline bool isRunning() { return m_running; }

    QString getLocomotiveName(void);

    inline void setDone(bool done) { m_done = done; }
    inline void setLine(int line) { m_line = line; }
    inline void setLocomotiveId(int lokoId) { m_lokoId = lokoId; }
    inline void setTime(float time) { m_time = time; }
    inline void setEndTime(float endTime) { m_endTime = endTime; }

    inline void setRelativeLine(int relativeLine) { m_relativeLine = relativeLine; }

    inline void setLokoCommands(int index, LokoCommands lokoCommands)
        { m_lokoCommands.insert(index, lokoCommands); }

    inline void setRunningStatus(bool status) { m_running = status; }

    void insertMarkedCellByIndex(int column, int row, int initialPosition);

    void clear(void);

    bool load(QDataStream & ar, int fileVersion);
    bool save(QDataStream & ar);

    typedef struct CellIndex
    {
        int column;
        int row;
        int initialPosition;
    } CellIndex;

    typedef struct FBModulesInfo
    {
        FBModulesInfo()
        {
            FBModule = 0;
            counter = 0;
            seconds = DEFAULT_CT_COUNTER_SECONDS;
        }
        int FBModule;
        int counter;
        int seconds;  // seconds program must wait to unblock counter
        QTime time;   // time that starts counting at
    } FBModulesInfo;

    QList<CellIndex> m_routeCells;

    QList<DetourSignalCondition> m_detourSignalConditions;
    QList<FBModuleCondition> m_FBModulesConditions;
    QList<TransferTableCondition> m_transferTableConditions;
    QList<TurnTableCondition> m_turnTableConditions;

    QList<FBModulesInfo> m_FBModules;
    QList<FBModulesInfo> m_FBModulesPending;

    // El número d'index és el número del mòdul
    // fixme
    QList<LokoCommands> m_lokoCommands;

    inline bool isSomeCTCounterActive(void);

    Document * getDocument(void);

protected:

    int		m_line;			// Identificar unic de la ruta
    int		m_lokoId;	// Identificador a l'estructura de locomotores.
    float	m_time;
    float	m_endTime;

    int		m_relativeLine;
    bool	m_done;
    bool	m_running;
};

#endif // ROUTE_H
