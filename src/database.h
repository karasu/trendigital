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
#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QFile>
#include <QDir>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>

#define DBFILENAME "trendigital.sqlite"

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = 0);
    ~Database();

    bool openDB();
    bool deleteDB();

    QSqlError lastError();

private:
    QSqlDatabase m_db;

signals:

public slots:

};

#endif // DATABASE_H
