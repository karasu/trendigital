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

LokoCommands::LokoCommands()
{
    m_FBModuleNumber = 0;

    m_name.clear();

    m_speed = 0;
    m_acceleration = 0;
    m_desacceleration = 0;

    m_stop = false;
    m_light = false;
    m_reverse = false;
    m_sound = false;

    m_soundFileName.clear();
    m_imageFileName.clear();

    m_functions.clear();
    m_aditionalFunctions.clear();
}

bool LokoCommands::load(QDataStream & ar, int fileVersion)
{
    ar >> m_FBModuleNumber;

    if (m_FBModuleNumber > 0)
    {
        ar >> m_light;
        ar >> m_reverse;
        ar >> m_sound;
        ar >> m_stop;
        ar >> m_acceleration;
        ar >> m_desacceleration;
        ar >> m_speed;
        ar >> m_imageFileName;
        ar >> m_name;
        ar >> m_soundFileName;

        int numFunctions = NUM_FUNCTIONS;
        int numAditionalFunctions = NUM_ADITIONAL_FUNCTIONS;

        if (fileVersion <= 6)
        {
            numFunctions = 8;
            numAditionalFunctions = 8;
        }

        for (int i=0; i<numFunctions; i++)
        {
            ar >> m_functions[i];
        }

        for (int i=0; i<numAditionalFunctions; i++)
        {
            ar >> m_aditionalFunctions[i];
        }
    }

    return true;
}

bool LokoCommands::save(QDataStream & ar)
{
    ar << m_FBModuleNumber;

    if (m_FBModuleNumber > 0)
    {
        ar << m_light;
        ar << m_reverse;
        ar << m_sound;
        ar << m_stop;
        ar << m_acceleration;
        ar << m_desacceleration;
        ar << m_speed;
        ar << m_imageFileName;
        ar << m_name;
        ar << m_soundFileName;

        for (int i=0; i<NUM_FUNCTIONS; i++)
        {
            ar << m_functions[i];
        }

        for (int i=0; i<NUM_ADITIONAL_FUNCTIONS; i++)
        {
            ar << m_aditionalFunctions[i];
        }
    }

    return true;
}
