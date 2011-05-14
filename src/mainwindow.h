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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QSettings>
#include <QTextCodec>
#include <QDebug>
#include <QPageSetupDialog>
#include <QPrinter>
#include <QMap>
#include <QImage>
#include <QPluginLoader>
#include <QActionGroup>

class LokoTabBar;
class LokoDock;
class Document;
class View;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *event);

    QString getLokoImageFileName(int index);
    LokoDock *getLokoDock(int id);

    Document *getDocument(void);

    QImage * getBitmapPtr(QString fileName);

    void loadPlugins(void);

    static MainWindow *getInstance();

    QPrinter *m_printer;

    QMap<QString,QImage *> m_dibs;

    QMap<QString,QActionGroup *> m_actionGroups;

    QDir m_pluginsDir;
    QStringList m_pluginFileNames;

    bool m_programsPaused;

private:
    Ui::MainWindow *ui;

    static MainWindow *m_pInstance;

    View *m_pView;
    Document *m_pDoc;
    LokoTabBar *m_pLokoTabBar;

    void showEditToolbars(bool show);

    void addConnections(void);
    bool askIfSaveBeforeCloseDocument(void);
    bool isCircuitModified(void);

    void loadSettings(void);
    void saveSettings(void);

    QActionGroup *m_pActionCommonGroup;

private slots:
    void onExit(void);
    void onNew(void);
    void onOpen(void);
    void onSave(void);
    void onSaveAs(void);
    void onInsertText(void);
    void onShowLokTabBar(void);
    void onAbout(void);
    void onAboutQT(void);
    void onAboutInterfaces(void);
    void onEditLokos(void);
    void onPrinterSetup(void);
    void onEditMode(bool checked=false);
    void onInsertElement(QAction *pAction);
    void onInterfaceGo(void);
    void onInterfaceStop(void);
};

#endif // MAINWINDOW_H
