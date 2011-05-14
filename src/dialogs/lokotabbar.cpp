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

#include "../general.h"

#include "lokodock.h"
#include "../loko.h"
#include "../element.h"
#include "../cell.h"
#include "../document.h"
#include "../mainwindow.h"

#include "lokotabbar.h"
#include "ui_lokotabbar.h"

LokoTabBar::LokoTabBar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LokoTabBar)
{
    ui->setupUi(this);

    setupButtons();

    ui->LokoTab->setCurrentIndex(0);
}

void LokoTabBar::closeEvent(QCloseEvent *event)
{
    event->accept();
}

LokoTabBar::~LokoTabBar()
{
    m_buttons.clear();

    QMap<int, LokoDock *>::const_iterator i = m_docks.constBegin();
    while (i != m_docks.constEnd())
    {
        delete i.value();
        i++;
    }

    delete ui;
}

void LokoTabBar::setupButtons()
{
    m_buttons.append(ui->pushButton_1);
    m_buttons.append(ui->pushButton_2);
    m_buttons.append(ui->pushButton_3);
    m_buttons.append(ui->pushButton_4);
    m_buttons.append(ui->pushButton_5);
    m_buttons.append(ui->pushButton_6);
    m_buttons.append(ui->pushButton_7);
    m_buttons.append(ui->pushButton_8);
    m_buttons.append(ui->pushButton_9);
    m_buttons.append(ui->pushButton_10);
    m_buttons.append(ui->pushButton_11);
    m_buttons.append(ui->pushButton_12);
    m_buttons.append(ui->pushButton_13);
    m_buttons.append(ui->pushButton_14);
    m_buttons.append(ui->pushButton_15);
    m_buttons.append(ui->pushButton_16);
    m_buttons.append(ui->pushButton_17);
    m_buttons.append(ui->pushButton_18);
    m_buttons.append(ui->pushButton_19);
    m_buttons.append(ui->pushButton_20);
    m_buttons.append(ui->pushButton_21);
    m_buttons.append(ui->pushButton_22);
    m_buttons.append(ui->pushButton_23);
    m_buttons.append(ui->pushButton_24);
    m_buttons.append(ui->pushButton_25);
    m_buttons.append(ui->pushButton_26);
    m_buttons.append(ui->pushButton_27);
    m_buttons.append(ui->pushButton_28);
    m_buttons.append(ui->pushButton_29);
    m_buttons.append(ui->pushButton_30);
    m_buttons.append(ui->pushButton_31);
    m_buttons.append(ui->pushButton_32);
    m_buttons.append(ui->pushButton_33);
    m_buttons.append(ui->pushButton_34);
    m_buttons.append(ui->pushButton_35);
    m_buttons.append(ui->pushButton_36);
    m_buttons.append(ui->pushButton_37);
    m_buttons.append(ui->pushButton_38);
    m_buttons.append(ui->pushButton_39);
    m_buttons.append(ui->pushButton_40);
    m_buttons.append(ui->pushButton_41);
    m_buttons.append(ui->pushButton_42);
    m_buttons.append(ui->pushButton_43);
    m_buttons.append(ui->pushButton_44);
    m_buttons.append(ui->pushButton_45);
    m_buttons.append(ui->pushButton_46);
    m_buttons.append(ui->pushButton_47);
    m_buttons.append(ui->pushButton_48);
    m_buttons.append(ui->pushButton_49);
    m_buttons.append(ui->pushButton_50);
    m_buttons.append(ui->pushButton_51);
    m_buttons.append(ui->pushButton_52);
    m_buttons.append(ui->pushButton_53);
    m_buttons.append(ui->pushButton_54);
    m_buttons.append(ui->pushButton_55);
    m_buttons.append(ui->pushButton_56);
    m_buttons.append(ui->pushButton_57);
    m_buttons.append(ui->pushButton_58);
    m_buttons.append(ui->pushButton_59);
    m_buttons.append(ui->pushButton_60);
    m_buttons.append(ui->pushButton_61);
    m_buttons.append(ui->pushButton_62);
    m_buttons.append(ui->pushButton_63);
    m_buttons.append(ui->pushButton_64);
    m_buttons.append(ui->pushButton_65);
    m_buttons.append(ui->pushButton_66);
    m_buttons.append(ui->pushButton_67);
    m_buttons.append(ui->pushButton_68);
    m_buttons.append(ui->pushButton_69);
    m_buttons.append(ui->pushButton_70);
    m_buttons.append(ui->pushButton_71);
    m_buttons.append(ui->pushButton_72);
    m_buttons.append(ui->pushButton_73);
    m_buttons.append(ui->pushButton_74);
    m_buttons.append(ui->pushButton_75);
    m_buttons.append(ui->pushButton_76);
    m_buttons.append(ui->pushButton_77);
    m_buttons.append(ui->pushButton_78);
    m_buttons.append(ui->pushButton_79);
    m_buttons.append(ui->pushButton_80);
    m_buttons.append(ui->pushButton_81);
    m_buttons.append(ui->pushButton_82);
    m_buttons.append(ui->pushButton_83);
    m_buttons.append(ui->pushButton_84);
    m_buttons.append(ui->pushButton_85);
    m_buttons.append(ui->pushButton_86);
    m_buttons.append(ui->pushButton_87);
    m_buttons.append(ui->pushButton_88);
    m_buttons.append(ui->pushButton_89);
    m_buttons.append(ui->pushButton_90);
    m_buttons.append(ui->pushButton_91);
    m_buttons.append(ui->pushButton_92);
    m_buttons.append(ui->pushButton_93);
    m_buttons.append(ui->pushButton_94);
    m_buttons.append(ui->pushButton_95);
    m_buttons.append(ui->pushButton_96);
    m_buttons.append(ui->pushButton_97);
    m_buttons.append(ui->pushButton_98);
    m_buttons.append(ui->pushButton_99);
    m_buttons.append(ui->pushButton_100);

    for (int i = 0; i < m_buttons.size(); ++i)
    {
        connect(m_buttons[i], SIGNAL(clicked()), this, SLOT(onButtonClicked()));
    }
}

Document *LokoTabBar::getDocument()
{
    MainWindow *pMainWindow = MainWindow::getInstance();

    if (pMainWindow != NULL)
    {
        return pMainWindow->getDocument();
    }

    return NULL;
}

void LokoTabBar::onButtonClicked()
{
    QPushButton *pSender = qobject_cast<QPushButton *>(sender());

    // pSender->setDisabled(true);

    // QString buttonName(nom, pSender->name());

    int id=-1;

    for (int i=0; i<m_buttons.size(); ++i)
    {
        if (m_buttons.at(i) == pSender)
        {
            id = i;
        }
    }

    if (id >= 0)
    {
        showLokoDock(id);
    }
}

void LokoTabBar::showLokoDock(int id)
{
    Document *pDoc = getDocument();

    if (pDoc != NULL)
    {
        LokoDock *pLokoDock = m_docks.value(id, NULL);

        if (!m_docks.contains(id))
        {
            // First check there is a loko in this position
            Loko *pLoko = getLoko(id);

            if (pLoko != NULL)
            {
                // Ok, there is a loko. First time we show the dock, we must create it
                pLokoDock = m_docks[id] = new LokoDock(this->parentWidget());
                pLokoDock->setWindowTitle(pLoko->m_name);

                setLokoImage(id, pLoko->getImageFileName());
            }
        }

        if (pLokoDock != NULL)
        {
            pLokoDock->setVisible(!pLokoDock->isVisible());
        }
    }
}

void LokoTabBar::setLokoImage(int id, QString fileName)
{
    if (m_docks.contains(id))
    {
        m_docks.value(id)->setLokoImage(fileName);
    }

    if (m_buttons[id] != NULL)
    {
        m_buttons[id]->setIcon(QIcon(fileName));
    }
}

Loko * LokoTabBar::getLoko(int id)
{
    Document *pDoc = getDocument();

    if (pDoc != NULL)
    {
        return pDoc->getLoko(id);
    }

    return NULL;
}

LokoDock * LokoTabBar::getLokoDock(int id)
{
//    if (id >= 0 && id < m_docks.size())
//    {
        return m_docks.value(id, NULL);
//    }
}

void LokoTabBar::hideAllDocks()
{
    QMap<int, LokoDock *>::const_iterator i = m_docks.constBegin();
    while (i != m_docks.constEnd())
    {
        LokoDock *pLokoDock = i.value();

        if (pLokoDock != NULL)
        {
            pLokoDock->setVisible(false);
        }

        i++;
    }
}
