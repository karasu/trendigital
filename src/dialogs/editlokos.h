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

#ifndef EDITLOKOS_H
#define EDITLOKOS_H

#include <QDialog>
#include <QMap>
#include <QSettings>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QLabel>
#include <QUrl>
#include <QInputDialog>
#include <QPrintDialog>
#include <QPrinter>
#include <QTextDocument>

class Loko;

namespace Ui {
    class EditLokos;
    class ImagePreview;
}

class EditLokos : public QDialog
{
    Q_OBJECT

public:
    explicit EditLokos(QWidget *parent = 0);
    ~EditLokos();

    void fillTreeWidget(void);
    void refreshTreeWidget(void);

    void saveSettings(void);

    void accept(void);
    void helpRequest(void);
    void reject(void);

    void clearLokos(void);
    void saveChanges(void);
    void copyLokosFromDocumentLokos(QMap<int,Loko *> *pDocLokos);

private:
    Ui::EditLokos *ui;

    // Pointer to lokos in Document class
    QMap<int,Loko *> *m_pDocLokos;

    // Our copy of lokos for editing
    QMap<int,Loko *> m_lokos;

    // List of our ui pushbuttonsFunctions
    QList<QPushButton *> m_uiFunctionButtons;
    QList<QPushButton *> m_uiExtraFunctionButtons;

    void getLokoInfo(int id);
    void setLokoInfo(int id=-1); // -1 means selected loko
    void clearLokoInfo(); // it simply clears all the ui widgets to be able to enter a new loko

    Loko * getLoko(int id);
    Loko * getLokoByAddress(int address);
    Loko * getLokoByName(QString name);
    Loko * getSelectedLoko(void);
    void enableAllControls(bool status=true);

    int getFirstFreeId(void);
    void setSelectedLoko(int id);

    int askLokoAddress(bool *ok);
    QString askLokoName(bool *ok);

    bool m_modifiedLokos;

private slots:
    void onPrint();
    void onNewLoko(void);
    void onTreeWidgetItemClicked(QTreeWidgetItem* item, int column);
    void onImageButton(void);
    void apply(void);

};

#endif // EDITLOKOS_H
