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
#ifndef LOKOCOMMANDS_H
#define LOKOCOMMANDS_H

#include <QString>
#include <QList>
#include <QDataStream>
#include <QDebug>

class LokoCommands
{
public:
    LokoCommands();

    inline int getFBModuleNumber() { return m_FBModuleNumber; }
    inline int getSpeed() {return m_speed; }
    inline int getAcceleration() { return m_acceleration; }
    inline int getDesacceleration() {return m_desacceleration; }
    inline bool getFunction(int i)
        { return m_functions.value(i, false); }
    inline bool getAditionalFunction(int i)
        { return m_aditionalFunctions.value(i, false); }
    inline bool getLight() { return m_light; }
    inline bool getReverse() { return m_reverse; }
    inline bool getSound() { return m_sound; }
    inline bool getStop() { return m_stop; }
    inline QString getName() { return m_name; }
    inline QString getImageFileName() { return m_imageFileName; }
    inline QString getSoundFileName() { return m_soundFileName; }

    inline void setFBModuleNumber(int value) { m_FBModuleNumber = value; }
    inline void setSpeed(int value) { m_speed = value; }
    inline void setAcceleration(int value) { m_acceleration = value; }
    inline void setDesacceleration(int value) { m_desacceleration = value; }
    inline void setFunction(int function, bool status)
        { m_functions[function] = status; }
    inline void setAditionalFunction(int function, bool status)
        { m_aditionalFunctions[function] = status; }
    inline void setLight(bool status) { m_light = status; }
    inline void setReverse(bool reverse) { m_reverse = reverse; }
    inline void setSound(bool sound) { m_sound = sound; }
    inline void setStop(bool stop) { m_stop = stop; }
    inline void setName(QString name) { m_name = name; }
    inline void setImageFileName(QString imageFileName)
        { m_imageFileName = imageFileName; }
    inline void setSoundFileName(QString soundFileName)
        { m_soundFileName = soundFileName; }

    inline void setUsed(bool used) { m_used = used; }

    bool load(QDataStream & ar, int fileVersion);
    bool save(QDataStream & ar);

protected:

    int m_FBModuleNumber;

    QString m_name;

    int m_speed;
    int m_acceleration;
    int m_desacceleration;

    bool m_stop;
    bool m_light;
    bool m_reverse;
    bool m_sound;

    QList<bool> m_functions;
    QList<bool> m_aditionalFunctions;

    QString m_soundFileName;
    QString m_imageFileName;

    bool m_used;
};

#endif // LOKOCOMMANDS_H
