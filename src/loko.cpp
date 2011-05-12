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
#include "dialogs/lokodock.h"
#include "mainwindow.h"
#include "loko.h"

Loko::Loko()
{
    qDebug() << ("Entering Loko::Loko()");

    clear();

    qDebug() << ("Loko::Loko() finished");

}

void Loko::clear()
{
    m_name.clear();
    m_type.clear();
    m_description.clear();
    m_brand.clear();
    m_engine.clear();
    m_id = -1;
    m_address = -1;
    m_direction = false; // forwards

    // set all functions to false
    for (int i=0; i<NUM_FUNCTIONS; i++)
    {
        m_functions[i] = false;
    }

    for (int i=0; i<NUM_EXTRA_FUNCTIONS; i++)
    {
        m_extraFunctions[i] = false;
    }

    m_imageFileName = "";

    m_dockVisible = false;

    m_extraDecoder = 0;

    m_stop = true;
    m_speed = 0;

    m_light = false;

    m_multitrack = false;
    m_multitracked = false;
}

void Loko::copy(Loko *pLoko)
{
    pLoko->clear();

    pLoko->m_name = m_name;
    pLoko->m_type = m_type;
    pLoko->m_description = m_description;
    pLoko->m_brand = m_brand;
    pLoko->m_engine = m_engine;
    pLoko->m_id = m_id;
    pLoko->m_address = m_address;
    pLoko->m_direction = m_direction;

    for (int i=0; i<NUM_FUNCTIONS; i++)
    {
        pLoko->m_functions[i] = m_functions[i];
    }

    for (int i=0; i<NUM_EXTRA_FUNCTIONS; i++)
    {
        pLoko->m_extraFunctions[i] = m_extraFunctions[i];
    }

    pLoko->m_imageFileName = m_imageFileName;

    pLoko->m_dockVisible = m_dockVisible;

    pLoko->m_extraDecoder = m_extraDecoder;

    pLoko->m_stop = m_stop;
    pLoko->m_speed = m_speed;

    pLoko->m_light = m_light;

    pLoko->m_multitrack = m_multitrack;
    pLoko->m_multitracked = m_multitracked;
}

bool Loko::load(QDataStream & ar, int fileVersion)
{
    ar >> m_direction;

    int numFunctions = NUM_FUNCTIONS;
    int numExtraFunctions = NUM_EXTRA_FUNCTIONS;

    if (fileVersion <= 6)
    {
        // Previous versions only allowed 8 functions and 8 extra functions
        numFunctions = 8;
    }

    for (int i=0; i<numFunctions; i++)
    {
        ar >> m_functions[i];
    }

    for (int i=0; i<numExtraFunctions; i++)
    {
        ar >> m_extraFunctions[i];
    }

    ar >> m_address;
    ar >> m_extraDecoder;
    ar >> m_id;
    ar >> m_description;
    ar >> m_imageFileName;
    ar >> m_brand;
    ar >> m_engine;
    ar >> m_name;
    ar >> m_type;
    m_type = m_type.upper();
    // ar >> m_controlDlgRect;
    ar >> m_dockVisible;
    ar >> m_light;
    ar >> m_speed;

    // From version 2 we also store multitrack lokos
    if (fileVersion >= 2)
    {
        ar >> m_multitracked;

        ar >> m_multitrack;

        if (m_multitrack)
        {
            int size = 0;

            ar >> size;

            if (size > 0)
            {
                int mid = 0;

                for (int i=0; i<size; i++)
                {
                    ar >> mid;
                    m_multitrack_ids.append(mid);
                }
            }
        }
    }

    storeLocomotive(); // Special for new TCP/IP stations

    return true;
}

bool Loko::save(QDataStream & ar)
{
    ar << m_direction;

    for (int i=0; i<NUM_FUNCTIONS; i++)
    {
        ar << m_functions[i];
    }

    for (int i=0; i<NUM_EXTRA_FUNCTIONS; i++)
    {
        ar << m_extraFunctions[i];
    }

    ar << m_address;
    ar << m_extraDecoder;
    ar << m_id;
    ar << m_description;
    ar << m_imageFileName;
    ar << m_brand;
    ar << m_engine;
    ar << m_name;
    ar << m_type.upper();
    // ar << m_dockRect;
    ar << m_dockVisible;
    ar << m_light;
    ar << m_speed;

    ar << m_multitracked;
    ar << m_multitrack;

    if (m_multitrack)
    {
        int size = m_multitrack_ids.size();
        ar << size;

        for (int i=0; i<size; i++)
        {

            ar << m_multitrack_ids[i];
        }
    }

    storeLocomotive(); // Special for new TCP/IP stations

    return true;
}

LokoDock * Loko::getLokoDock()
{
    MainWindow *pMainWindow = MainWindow::getInstance();

    if (pMainWindow != NULL)
    {
        return NULL;
    }

    return pMainWindow->getLokoDock(m_id);
}

void Loko::storeLocomotive()
{
}

void Loko::setLight(bool lightOn)
{
    // Algunes interfaces obliguen a canviar la velocitat si volem canviar la llum (märklín, p.e.)

    int speednow = m_speed;

    if (m_stop)
    {
        // Quan posem la màquina a stop a speed continua havent-hi la última velocitat coneguda
        speednow = 0;
    }
/*
    if (g_interface.setLokoLight(address, speednow, lightOn))
    {
        updateLokoDock(LIGHT_DOCK, lightOn);
    }
 */
    m_light = lightOn;
}
