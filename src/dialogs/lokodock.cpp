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
#include "lokodock.h"
#include "ui_lokodock.h"

LokoDock::LokoDock(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LokoDock)
{
    ui->setupUi(this);

    // layout()->setSizeConstraint(QLayout::SetFixedSize);

    ui->functionGroupBox->hide();

    m_lokoID = -1;

    // setLokoImage("/home/karasu/projects/TrenDigital/images/loks/Roco/steam/63291.png");
}

LokoDock::~LokoDock()
{
    delete ui;
}

void LokoDock::setLokoImage(QString lokoImageFileName)
{
    // qApp->getLokoImageFileName();

    ui->pushButton->setIcon(QIcon(lokoImageFileName));
}
