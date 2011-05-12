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
#include "conditions.h"

DetourSignalCondition::DetourSignalCondition()
{
    qDebug() << ("Entering DetourSignalCondition::DetourSignalCondition()");

    qDebug() << ("DetourSignalCondition::DetourSignalCondition() finished.");
}

FBModuleCondition::FBModuleCondition()
{
    m_number = 0;
    m_status = 0;
}

TransferTableCondition::TransferTableCondition()
{
    m_elementNumber = 0;
    m_FBModuleNumber = 0;
    m_position = 0;
}

bool TransferTableCondition::load(QDataStream & ar, int fileVersion)
{
    ar >> m_elementNumber;
    ar >> m_FBModuleNumber;
    ar >> m_position;

    return true;
}

bool TransferTableCondition::save(QDataStream & ar)
{
    ar << m_elementNumber;
    ar << m_FBModuleNumber;
    ar << m_position;

    return true;
}

TurnTableCondition::TurnTableCondition()
{
    m_position = 1;
    m_FBModuleNumber = 0;
}

bool TurnTableCondition::load(QDataStream & ar, int fileVersion)
{
    ar << m_FBModuleNumber;
    ar << m_position;

    return true;
}

bool TurnTableCondition::save(QDataStream & ar)
{
    ar >> m_FBModuleNumber;
    ar >> m_position;

    return true;
}
