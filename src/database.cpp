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

#include "database.h"

Database::Database(QObject *parent) :
    QObject(parent)
{
}

Database::~Database()
{
}


bool Database::openDB()
{
    // Find QSLite driver
    m_db = QSqlDatabase::addDatabase("QSQLITE");

    #ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append(DBFILENAME);
    path = QDir::toNativeSeparators(path);
    m_db.setDatabaseName(path);
    #else
    // NOTE: File exists in the application private folder, in Symbian Qt implementation
    m_db.setDatabaseName(DBFILENAME);
    #endif

    // Open databasee
    return m_db.open();
}

QSqlError Database::lastError()
{
    // If opening database has failed user can ask
    // error description by QSqlError::text()
    return m_db.lastError();
}

bool Database::deleteDB()
{
    // Close database
    m_db.close();

    #ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append(DBFILENAME);
    path = QDir::toNativeSeparators(path);
    return QFile::remove(path);
    #else

    // Remove created database binary file
    return QFile::remove(DBFILENAME);
    #endif
}

