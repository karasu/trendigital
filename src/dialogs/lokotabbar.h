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

#ifndef LOKOTABBAR_H
#define LOKOTABBAR_H

#include <QTabWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLayout>
#include <QMap>

class Loko;
class LokoDock;
class Document;
class MainWindow;

namespace Ui {
    class LokoTabBar;
}

class LokoTabBar : public QDialog
{
    Q_OBJECT

public:
    explicit LokoTabBar(QWidget *parent = 0);
    ~LokoTabBar();

    void closeEvent(QCloseEvent *event);
    LokoDock * getLokoDock(int id);

private:
    Ui::LokoTabBar *ui;

    QList<QPushButton*> m_buttons;
    QMap<int, LokoDock*> m_docks;
    
    void setupButtons(void);
    Document *getDocument(void);
    void showLokoDock(int id);
    Loko *getLoko(int id);
    void hideAllDocks(void);

    void setLokoImage(int id, QString fileName);

private slots:
    void onButtonClicked(void);

};

#endif // LOKOTABBAR_H
