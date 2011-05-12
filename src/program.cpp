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

#include "general.h"
#include "lokocommands.h"
#include "conditions.h"
#include "route.h"
#include "program.h"

Program::Program()
{
    qDebug() << ("Entering Program::Program()");

    m_name.clear();
    m_description.clear();

    m_activeLine = 0;
    m_lastLine = 0;
    m_startLine = 0;

    m_routes.clear();

    qDebug() << ("Program::Program() finished.");
}

bool Program::load(QDataStream & ar, int fileVersion)
{
    // Loading...

    int size;

    ar >> m_name;
    ar >> m_description;

    ar >> m_activeLine;
    ar >> m_lastLine;
    ar >> m_startLine;

    m_routes.clear();

    ar >> size;

    for (int i=0; i<size; i++)
    {
        Route r;
        r.load(ar, fileVersion);
        m_routes.push_back(r);
    }

    if (fileVersion >= 4)
    {
        // fixme
        // m_globalConditions.Serialize(ar, fileVersion);
    }

    return true;
}

bool Program::save(QDataStream & ar)
{
    ar << m_name;
    ar << m_description;

    ar << m_activeLine;
    ar << m_lastLine;
    ar << m_startLine;

    int size = m_routes.size();

    ar << size;

    foreach(Route r, m_routes)
    {
        r.save(ar);
    }

    // fixme
    // m_globalConditions.Serialize(ar, iVersion);

    return true;
}

void Program::deleteRoute(Route *pRoute)
{
    if (pRoute == NULL)
    {
        return;
    }

    int deleteIndex = INVALID;

    for (int i=0; i<m_routes.size(); i++)
    {
        if (pRoute != &m_routes.at(i))
        {
            // Quan s'inserta una línia de programa s'ha de renumerar els números
            // de les línies (de forma que no s'hagin de canviar manualment una per
            // una després de la insersió).

            if (m_routes[i].getRelativeLine() > pRoute->getLine())
            {
                m_routes[i].setRelativeLine(m_routes[i].getRelativeLine() - 1);
            }
        }
        else
        {
            deleteIndex = i;
        }
    }

    ASSERT(deleteIndex != INVALID);

    if (deleteIndex != INVALID)
    {
        m_routes.removeAt(deleteIndex);
    }
}

void Program::insertRoute(Route r)
{
    int insertIndex = INVALID;

    for (int i=0; i<m_routes.size(); i++)
    {
        if (m_routes[i].getLine() < r.getLine())
        {
            // Quan s'inserta una línia de programa s'ha de renumerar els números
            // de les línies (de forma que no s'hagin de canviar manualment una per
            // una després de la insersió).

            if (m_routes[i].getRelativeLine() >= r.getLine())
            {
                m_routes[i].setRelativeLine(m_routes[i].getRelativeLine() + 1);
            }
        }
        else if (insertIndex == INVALID)
        {
            insertIndex = i;
        }
    }

    // Al ser una llista, l'hem inserir allà on toca

    m_routes.insert(insertIndex, r);
}

void Program::copyTo(Program *pProgram)
{
    ASSERT(pProgram != NULL);

    if (pProgram != NULL)
    {
        pProgram->setName(m_name);
        pProgram->setDescription(m_description);

        copyRoutesAndConditionsTo(pProgram);
    }
}

void Program::copyRoutesAndConditionsTo(Program *pProgram)
{
    ASSERT(pProgram != NULL);

    // fixme
    // m_globalConditions.copyTo(pProgram->getGlobalConditions());

    pProgram->clearRoutes();

    foreach(Route r, m_routes)
    {
        pProgram->addRoute(r);
    }

    pProgram->setLastLine(m_lastLine);
    pProgram->setActiveLine(m_activeLine);
}

bool Program::isDone()
{
    for (int i=0; i<(int)m_routes.size(); i++)
    {
        if (!m_routes[i].isDone())
        {
            return false;
        }
    }

    return true;
}

void Program::setDone(bool status)
{
    for (int i=0; i<m_routes.size(); i++)
    {
        m_routes[i].setDone(status);
    }
}

void Program::setRunningStatus(bool status)
{
    for (int i=0; i<m_routes.size(); i++)
    {
        m_routes[i].setRunningStatus(status);
    }
}

bool Program::isRunning()
{
    for (int i=0; i<m_routes.size(); i++)
    {
        if (m_routes[i].isRunning())
        {
            return true;
        }
    }

    return false;
}

Route * Program::getRoute(int i)
{
    return &m_routes[i];
}


void Program::addRoute(Route r)
{
    ASSERT(r.getLine() == m_routes.size() + 1);

    m_routes.push_back(r);
}

