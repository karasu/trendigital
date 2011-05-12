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
#ifndef PROGRAM_H
#define PROGRAM_H

#include <QList>
#include <QString>
#include <QDataStream>
#include <QDebug>

class Route;
// class GlobalConditions;

class Program
{
public:
    Program();

// protected:
    QList<Route> m_routes;

    QString m_name;
    QString m_description;

    int m_activeLine;
    int m_lastLine;
    int m_startLine;

    //fixme:
    // GlobalConditions m_globalConditions;

public:
    //fixme:
    // inline GlobalConditions * getGlobalConditions() { return &m_globalConditions; }

    inline int getActiveLine() { return m_activeLine; }
    inline int getLastLine() { return m_lastLine; }
    inline QString getName() { return m_name; }
    inline QString getDescription() { return m_description; }

    inline int getStartLine() { return m_startLine; }
    inline void setActiveLine(int activeLine) { m_activeLine = activeLine; }
    inline void setLastLine(int lastLine) { m_lastLine = lastLine; }

    inline void setName(QString name) { m_name = name; }
    inline void setDescription(QString s) { m_description = s; }
    inline void setStartLine(int startLine) { m_startLine = startLine; }

    inline int getNumRoutes() { return m_routes.size(); }
    inline Route getRouteCopy(int i) { return m_routes.at(i); }

    Route * getRoute(int i);
    void addRoute(Route r);

    void deleteRoute(Route *pRoute);
    void insertRoute(Route r);

    bool load(QDataStream & ar, int fileVersion);
    bool save(QDataStream & ar);

    void copyTo(Program * pProgram);
    void copyRoutesAndConditionsTo(Program *pProgram);

    bool isDone(void);
    void setRunningStatus(bool bStatus);
    void setDone(bool bStatus);

    bool isRunning(void);

    inline void clearRoutes() {	m_routes.clear(); }

    // fixme
    // inline void checkGlobalConditions() { m_globalConditions.checkGlobalConditions(); }

};

#endif // PROGRAM_H
