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

#ifndef LOKODOCK_H
#define LOKODOCK_H

#include <QDialog>
#include <QLayout>
#include <QDebug>

namespace Ui {
    class LokoDock;
}

class LokoDock : public QDialog
{
    Q_OBJECT

public:
     explicit LokoDock(QWidget *parent = 0);
    ~LokoDock();

    int m_lokoID;

    void setLokoImage(QString lokoImageFileName);

private:
    Ui::LokoDock *ui;
};

#endif // LOKODOCK_H
