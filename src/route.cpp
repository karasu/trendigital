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
#include "element.h"
#include "cell.h"
#include "grid.h"
#include "document.h"
#include "loko.h"
#include "conditions.h"
#include "route.h"
#include "mainwindow.h"

Route::Route()
{
    m_line = 0;
    m_lokoId = 0;

    m_time = 0;
    m_endTime = 0;

    m_relativeLine = 0;

    m_done = false;

    m_running = false;
}

Route::~Route()
{
    clear();
}

bool Route::load(QDataStream & ar, int fileVersion)
{
    int size;
    int status, elementNumber, FBModuleNumber;

    DetourSignalCondition detourSignalCondition;

    ar >> size;

    m_detourSignalConditions.clear();

    for (int i=0; i<size; i++)
    {
        ar >> status;
        ar >> elementNumber;
        ar >> FBModuleNumber;

        detourSignalCondition.setDetourSignalStatus(status);
        detourSignalCondition.setDetourSignalElementNumber(elementNumber);
        detourSignalCondition.setFBModuleNumber(FBModuleNumber);

        m_detourSignalConditions.push_back(detourSignalCondition);
    }

    ar >> size;

    FBModuleCondition FBModuleCondition;
    int number;

    m_FBModulesConditions.clear();

    for (int i=0; i<size; i++)
    {
        ar >> number;
        ar >> status;

        FBModuleCondition.setFBModuleNumber(number);
        FBModuleCondition.setFBModuleStatus(status);

        m_FBModulesConditions.push_back(FBModuleCondition);
    }

    int FBModule;

    ar >> size;

    m_FBModules.clear();

    if (fileVersion <= 5) // Fins a la versió 5 de fitxers dgz
    {
        FBModulesInfo fb;

        fb.counter = 0;
        fb.seconds = DEFAULT_CT_COUNTER_SECONDS;

        for (int i=0; i<size; i++)
        {
            ar >> FBModule;

            fb.FBModule = FBModule;
            m_FBModules.push_back(fb);
        }

        if (fileVersion >= 3)
        {
            // A partir de la versio 3 ja es va afegir el tema dels comptadors
            // per cada CT, però no encara el tema del bloqueig per segons

            int counter;

            for (int i=0; i<size; i++)
            {
                ar >> counter;
                m_FBModules[i].counter = counter;
            }
        }
    }
    else // Versió 6 i superiors del format de fitxer dgz
    {
        // A partir de la versió 6 ja es va guardar el tema de bloqueig de CT
        // per segons. També es va modificar l'ordre en que es guarda la informació.

        int FBModule;
        int counter;
        int seconds;

        FBModulesInfo fb;

        for (int i=0; i<size; i++)
        {
            ar >> FBModule;
            ar >> counter;
            ar >> seconds;

            fb.FBModule = FBModule;
            fb.counter = counter;
            fb.seconds = seconds;

            m_FBModules.push_back(fb);
        }
    }

    ar >> m_line;
    ar >> m_lokoId;
    ar >> m_relativeLine;
    ar >> m_time;

    ar >> size;

    m_lokoCommands.clear();

    for (int i=0; i<size; i++)
    {
        LokoCommands command;

        command.load(ar, fileVersion);

        m_lokoCommands.push_back(command);
    }

    ar >> size;

    m_routeCells.clear();

    for (int i=0; i<size; i++)
    {
        CellIndex cellIndex;

        ar >> cellIndex.column;
        ar >> cellIndex.row;
        ar >> cellIndex.initialPosition;

        m_routeCells.push_back(cellIndex);
    }

    ar >> size;

    TransferTableCondition transferTableCondition;

    m_transferTableConditions.clear();

    for (int i=0; i<size; i++)
    {
        int FBModuleNumber, position;

        ar >> FBModuleNumber;
        ar >> position;

        transferTableCondition.setFBModuleNumber(FBModuleNumber);
        transferTableCondition.setPosition(position);

        m_transferTableConditions.push_back(transferTableCondition);
    }

    ar >> size;

    TurnTableCondition turnTableCondition;

    m_turnTableConditions.clear();

    for (int i=0; i<size; i++)
    {
        int iFBModuleNumber, iPosition;

        ar >> iFBModuleNumber;
        ar >> iPosition;

        turnTableCondition.setFBModuleNumber(iFBModuleNumber);
        turnTableCondition.setPosition(iPosition);

        m_turnTableConditions.push_back(turnTableCondition);
    }

    return true;
}

bool Route::save(QDataStream & ar)
{
    int size = m_detourSignalConditions.size();

    ar << size;

    for (int i=0; i<size; i++)
    {
        ar << m_detourSignalConditions[i].getDetourSignalStatus();
        ar << m_detourSignalConditions[i].getDetourSignalElementNumber();
        ar << m_detourSignalConditions[i].getFBModuleNumber();
    }

    size = m_FBModulesConditions.size();

    ar << size;

    for (int i=0; i<size; i++)
    {
        ar << m_FBModulesConditions[i].getFBModuleNumber();
        ar << m_FBModulesConditions[i].getFBModuleStatus();
    }

    size = m_FBModules.size();

    ar << size;

    for (int i=0; i<size; i++)
    {
        ar << m_FBModules.at(i).FBModule;
        ar << m_FBModules.at(i).counter;
        ar << m_FBModules.at(i).seconds;
    }

    ar << m_line;
    ar << m_lokoId;
    ar << m_relativeLine;
    ar << m_time;

    size = m_lokoCommands.size();

    ar << size;

    for (int i=0; i<size; i++)
    {
        m_lokoCommands[i].save(ar);
    }

    size = m_routeCells.size();

    ar << size;

    for (int i=0; i<size; i++)
    {
        ar << m_routeCells.at(i).column;
        ar << m_routeCells.at(i).row;
        ar << m_routeCells.at(i).initialPosition;
    }

    size = m_transferTableConditions.size();

    ar << size;

    for (int i=0; i<size; i++)
    {
        ar << m_transferTableConditions[i].getFBModuleNumber();
        ar << m_transferTableConditions[i].getPosition();
    }

    size = m_turnTableConditions.size();

    ar << size;

    for (int i=0; i<size; i++)
    {
        ar << m_turnTableConditions[i].getFBModuleNumber();
        ar << m_turnTableConditions[i].getPosition();
    }

    return true;
}

bool Route::isSomeCTCounterActive()
{
    foreach(FBModulesInfo module, m_FBModules)
    {
        if (module.counter > 0)
        {
            return true;
        }
    }

    return false;
}

void Route::insertMarkedCellByIndex(int column, int row, int initialPosition)
{
    // Busquem si ja està insertada

    int size = (int)m_routeCells.size();

    int index = INVALID;

    for (int i=0; i<size; i++)
    {
        if (m_routeCells.at(i).column == column &&
            m_routeCells.at(i).row == row)
        {
            index = i;
            break;
        }
    }

    if (index != INVALID)
    {
        m_routeCells[index].initialPosition = initialPosition;
    }
    else
    {
        // No l'hem trobada. L'insertem de nou.

        CellIndex cellIndex;

        cellIndex.column =  column;
        cellIndex.row = row;
        cellIndex.initialPosition = initialPosition;

        m_routeCells.push_back(cellIndex);
    }
}

void Route::clear()
{
    m_routeCells.clear();
    m_detourSignalConditions.clear();
    m_FBModulesConditions.clear();
    m_FBModules.clear();
}

Document * Route::getDocument()
{
    MainWindow *pMainWindow = MainWindow::getInstance();

    if (pMainWindow != NULL)
    {
        return pMainWindow->getDocument();
    }

    return NULL;
}

QString Route::getLocomotiveName()
{
    Document *pDoc = getDocument();

    ASSERT(pDoc != NULL);

    if (pDoc != NULL)
    {
        Loko *pLoko = pDoc->getLoko(m_lokoId);

        ASSERT(pLoko != NULL);

        if (pLoko != NULL)
        {
            return pLoko->m_name;
        }
    }

    return "";
}
