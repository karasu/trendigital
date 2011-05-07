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
#ifndef CONDITIONS_H
#define CONDITIONS_H

#include <QDataStream>

class DetourSignalCondition
{
public:
    DetourSignalCondition();

    inline void setFBModuleNumber(int FBModuleNumber)
        { m_FBModuleNumber = FBModuleNumber; }
    inline void setDetourSignalElementNumber(int elementNumber)
        { m_elementNumber = elementNumber; }
    inline void setDetourSignalStatus(int status)
        { m_status = status; }

    inline int getFBModuleNumber(void) { return m_FBModuleNumber; }
    inline int getDetourSignalElementNumber(void) { return m_elementNumber; }
    inline int getDetourSignalStatus(void) { return m_status; }

protected:
        int m_FBModuleNumber;
        int m_elementNumber;
        int m_status;
};

class FBModuleCondition
{
public:
    FBModuleCondition();

    inline int getFBModuleNumber() { return m_number; }
    inline int getFBModuleStatus() { return m_status; }

    inline void setFBModuleNumber(int number) { m_number = number; }
    inline void setFBModuleStatus(int status ) { m_status = status; }


protected:
    int m_number;
    int m_status; // 0 means not used, 1 free, 2 occupied
};

class TransferTableCondition
{
public:
    TransferTableCondition();

    bool load(QDataStream & ar, int fileVersion);
    bool save(QDataStream & ar);

    inline int getElementNumber() { return m_elementNumber; }
    inline int getFBModuleNumber() { return m_FBModuleNumber; }
    inline int getPosition() { return m_position; }

    inline void setElementNumber(int elementNumber) { m_elementNumber = elementNumber; }
    inline void setFBModuleNumber(int FBModuleNumber) { m_FBModuleNumber = FBModuleNumber; }
    inline void setPosition(int position) { m_position = position; }

protected:
    int m_elementNumber;
    int m_FBModuleNumber;
    int m_position;
};

class TurnTableCondition
{
public:
    TurnTableCondition();

    bool load(QDataStream & ar, int fileVersion);
    bool save(QDataStream & ar);

    inline int getFBModuleNumber() { return m_FBModuleNumber; }
    inline int getPosition() { return m_position; }

    inline void setFBModuleNumber(int FBModuleNumber) { m_FBModuleNumber = FBModuleNumber; }
    inline void setPosition(int position) { m_position = position; }

protected:
    int m_FBModuleNumber;
    int m_position;
};

#endif // CONDITIONS_H
