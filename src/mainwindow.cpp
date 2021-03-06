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
#include "general.h"
#include "lokocommands.h"
#include "conditions.h"
#include "route.h"
#include "program.h"
#include "loko.h"
#include "element.h"
#include "cell.h"
#include "document.h"
#include "traininterface.h"

#include "plugindialog.h"

#include "debug.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialogs/lokotabbar.h"
#include "dialogs/lokodock.h"
#include "dialogs/editlokos.h"
#include "dialogs/systemsetup.h"

#include "scanfeedbackmodules.h"

TrainInterface *g_interface = NULL;

MainWindow * MainWindow::m_pInstance = NULL;

MainWindow * MainWindow::getInstance()
{
    if (!m_pInstance)
    {
        m_pInstance = new MainWindow();
    }

    return m_pInstance;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    debug("Entering MainWindow constructor", __FILE__, __LINE__);

    m_pInstance = this;

    QTextCodec *utf8Codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(utf8Codec);
    QTextCodec::setCodecForCStrings(utf8Codec);
    QTextCodec::setCodecForLocale(utf8Codec);

    ui->setupUi(this);

    // Check single instance

    // Show splash screen

    // Load program settings
    loadSettings();
    debug("Program settings loaded.", __FILE__, __LINE__);

    if (m_settingsInterfaceName.length() <= 0)
    {
        debug("Can't read settings. I'll try to use virtual interface", __FILE__, __LINE__);
        m_settingsInterfaceName = "virtual";
    }

    // Check serial key

    // Check for updates

    // Add menu and toolbar connections
    addConnections();

    // Create a new empty document by default
    // m_pDoc = new Document(this);
    m_pDoc = new Document();
    debug("New empty document created.", __FILE__, __LINE__);

    // Create lok tab bar (where all lokos are shown)

    m_pLokoTabBar = new LokoTabBar(this);
    debug("New Loko Tab Bar created.", __FILE__, __LINE__);

    // QMessageBox::information(this, "Error", "Could not load the plugin");

    // this->setCentralWidget(ui->graphicsView);

    m_pView = ui->graphicsView;

    m_printer = new QPrinter( QPrinter::HighResolution );

    // doc->setModified();

    showEditToolbars(false);

    // Use virtual plugin
    useInterface(m_settingsInterfaceName);

    if (g_interface == NULL)
    {
        // can't load m_settingsInterfaceName... try just loading virtual interface
        useInterface("virtual");
    }

    debug("Starting feedback modules thread", __FILE__, __LINE__);
    startReadingFeedbackModules();

    // create some testing lokos
    createTestLokos();
    debug("Test lokos created.", __FILE__, __LINE__);

    debug("MainWindow::MainWindow() finished", __FILE__, __LINE__);

}

void MainWindow::createTestLokos()
{
    // just for testing purposes ************************************************************************************

    Loko *pLoko;

    pLoko = new Loko();
    pLoko->m_name = QString("Loko1");
    pLoko->m_type = QString("MOTOROLA");
    pLoko->m_address = 1000;
    pLoko->m_imageFileName = QString("/home/karasu/TrenDigital/images/lokos/Roco/diesel/59460.png");
    pLoko->m_id = 0;
    m_pDoc->m_lokos.insert(0, pLoko);

    pLoko = new Loko();
    pLoko->m_name = QString("Loko2");
    pLoko->m_type = QString("MOTOROLA");
    pLoko->m_address = 1001;
    pLoko->m_imageFileName = QString("/home/karasu/TrenDigital/images/lokos/Roco/diesel/59461.png");
    pLoko->m_id = 1;
    m_pDoc->m_lokos.insert(1, pLoko);

    pLoko = new Loko();
    pLoko->m_name = QString("Loko3");
    pLoko->m_type = QString("MFX");
    pLoko->m_address = 1002;
    pLoko->m_imageFileName = QString("/home/karasu/TrenDigital/images/lokos/Roco/diesel/63458.png");
    pLoko->m_id = 2;
    m_pDoc->m_lokos.insert(2, pLoko);

    pLoko = new Loko();
    pLoko->m_name = QString("Loko4");
    pLoko->m_type = QString("MFX");
    pLoko->m_address = 1003;
    pLoko->m_imageFileName = QString("/home/karasu/TrenDigital/images/lokos/Roco/diesel/63425.png");
    pLoko->m_id = 3;
    m_pDoc->m_lokos.insert(3, pLoko);

    pLoko = new Loko();
    pLoko->m_name = QString("Loko5");
    pLoko->m_type = QString("DCC");
    pLoko->m_address = 1004;
    pLoko->m_imageFileName = QString("/home/karasu/TrenDigital/images/lokos/Roco/diesel/63970.png");
    pLoko->m_id = 4;
    m_pDoc->m_lokos.insert(4, pLoko);
}

MainWindow::~MainWindow()
{
    foreach(QActionGroup *pActionGroup, m_actionGroups)
    {
        delete pActionGroup;
    }

    m_actionGroups.clear();

    delete m_pDoc;

    delete m_pLokoTabBar;

    delete ui;

    delete m_printer;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
// fixme
//    if (askIfSaveBeforeCloseDocument())
//    {
        m_pLokoTabBar->close();

        saveSettings();

        event->accept();
//    }
//    else
//    {
//        event->ignore();
//    }
}

void MainWindow::addConnections()
{
    // file menu
    connect(ui->actionNou, SIGNAL(triggered()), this, SLOT(onNew()));
    connect(ui->actionObrir, SIGNAL(triggered()), this, SLOT(onOpen()));
    connect(ui->actionSortir, SIGNAL(triggered()), this, SLOT(onExit()));
    connect(ui->actionGuardar, SIGNAL(triggered()), this, SLOT(onSave()));
    connect(ui->actionGuardar_com, SIGNAL(triggered()), this, SLOT(onSaveAs()));
    connect(ui->actionConfigurar_impresora, SIGNAL(triggered()), this, SLOT(onPrinterSetup()));
    connect(ui->actionConfigurar_sistema, SIGNAL(triggered()), this, SLOT(onSystemSetup()));
    // onExit()

    // edit menu

    connect(ui->actionLocomotores, SIGNAL(triggered()), this, SLOT(onEditLokos()));
    connect(ui->actionProgrames_i_rutes, SIGNAL(triggered()), this, SLOT(onPrograms()));
    connect(ui->actionInsertar_text, SIGNAL(triggered()), this, SLOT(onInsertText()));

    // view menu
    connect(ui->actionBarra_locomotores, SIGNAL(triggered()), this, SLOT(onShowLokTabBar()));
    connect(ui->actionMode_edicio, SIGNAL(triggered(bool)), this, SLOT(onEditMode(bool)));

    // Actions menu
    connect(ui->actionPararTot, SIGNAL(triggered()), this, SLOT(onInterfaceStop()));
    connect(ui->actionEnjegarTot, SIGNAL(triggered()), this, SLOT(onInterfaceGo()));

    // help menu
    connect(ui->actionSobre_TrenDigital, SIGNAL(triggered()), this, SLOT(onAbout()));
    connect(ui->actionSobre_QT, SIGNAL(triggered()), this, SLOT(onAboutQT()));
    connect(ui->actionSobre_Interficies, SIGNAL(triggered()), this, SLOT(onAboutInterfaces()));

    // EditToolbar actions
    // fixme: falten icones!
    QActionGroup *pActionGroup = new QActionGroup(ui->editToolBar);
    pActionGroup->addAction(ui->actionCommon_33000_0);
    pActionGroup->addAction(ui->actionCommon_33001_0);
    pActionGroup->addAction(ui->actionCommon_33002_0);
    pActionGroup->addAction(ui->actionCommon_33003_0);
    pActionGroup->addAction(ui->actionCommon_33008_0);
    pActionGroup->addAction(ui->actionCommon_33009_0);
    pActionGroup->addAction(ui->actionCommon_33010_0);
    pActionGroup->addAction(ui->actionCommon_33011_0);
    pActionGroup->addAction(ui->actionCommon_33012_0);
    pActionGroup->addAction(ui->actionCommon_33013_0);
    pActionGroup->addAction(ui->actionCommon_33014_0);
    pActionGroup->addAction(ui->actionCommon_33015_0);
    pActionGroup->addAction(ui->actionCommon_33016_0);
    pActionGroup->addAction(ui->actionCommon_33017_0);
    pActionGroup->addAction(ui->actionCommon_33018_0);
    pActionGroup->addAction(ui->actionCommon_33019_0);
    pActionGroup->addAction(ui->actionCommon_33020_0);
    pActionGroup->addAction(ui->actionCommon_33021_0);
    pActionGroup->addAction(ui->actionCommon_33022_0);
    pActionGroup->addAction(ui->actionCommon_33023_0);
    pActionGroup->addAction(ui->actionCommon_33024_0);
    pActionGroup->addAction(ui->actionCommon_33025_0);
    pActionGroup->addAction(ui->actionCommon_33026_0);
    pActionGroup->addAction(ui->actionCommon_33027_0);
    connect(pActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(onInsertElement(QAction*)));
    m_actionGroups.insert("Common", pActionGroup);
}

// file menu --------------------------------------------------------

bool MainWindow::askIfSaveBeforeCloseDocument()
{
    if (m_pDoc->isModified())
    {
        int r = QMessageBox::warning(this, tr("Tren Digital"),
        tr("El circuit, les màquines o els programes s'han modificat.\n"
           "Estàs segur que vols sortir sense desar els canvis?"),
        QMessageBox::Yes | QMessageBox::Default,
        QMessageBox::No,
        QMessageBox::Cancel | QMessageBox::Escape);

        if (r == QMessageBox::No)
        {
            return m_pDoc->save();
        }
        else if (r == QMessageBox::Cancel)
        {
            return false;
        }
    }

    return true;
}


void MainWindow::onOpen()
{
    if (askIfSaveBeforeCloseDocument())
    {
        QString fileName = QFileDialog::getOpenFileName(this,
        tr("Obre un arxiu"), ".",
        tr("Arxius Tren Digital (*.tdz)"));

        if (!fileName.isEmpty())
        {
            if  (m_pDoc->load(fileName))
            {
                statusBar()->showMessage(tr("Càrrega de l'arxiu cancel·lada"), 2000);
            }
            else
            {
                statusBar()->showMessage(tr("Arxiu carregat"), 2000);
            }
        }
    }
}

void MainWindow::onNew()
{
    if (askIfSaveBeforeCloseDocument())
    {
        m_pDoc->clear();
    }

    setWindowTitle(m_pDoc->getFileName());
}

void MainWindow::onSave()
{
    if (m_pDoc->getFileName() == tr("sensetitol"))
    {
        onSaveAs();
    }
    else
    {
        m_pDoc->save();
    }

    setWindowTitle(m_pDoc->getFileName());
}

void MainWindow::onSaveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
    tr("Guarda un arxiu"), ".",
    tr("Arxius Tren Digital (*.tdz)"));

    if (!fileName.isEmpty())
    {
        m_pDoc->saveAs(fileName);
    }

    setWindowTitle(m_pDoc->getFileName());
}

void MainWindow::onExit()
{
    if (askIfSaveBeforeCloseDocument())
    {
        this->close();
    }
}

void MainWindow::onPrinterSetup()
{
}

void MainWindow::onSystemSetup()
{
    SystemSetup *systemSetup = new SystemSetup(this);

    if (g_interface != NULL)
    {
        debug(g_interface->commPort(), __FILE__, __LINE__);

        systemSetup->setCommPort(g_interface->commPort());
        systemSetup->setSpeed(g_interface->baudRate());
        systemSetup->setPluginNames(m_pluginNames, g_interface->name());
    }
    else
    {
        systemSetup->setPluginNames(m_pluginNames, "");
    }

    systemSetup->setReadingDelay(m_readingDelay);

    if (systemSetup->exec() == QDialog::Accepted)
    {
        if (g_interface == NULL || g_interface->name() != systemSetup->pluginName())
        {
            // user has selected a new interface, load it!
            useInterface(systemSetup->pluginName());
        }

        // save dialog settings (general an plugin ones) to our interface
        systemSetup->saveSettings(g_interface);

        m_readingDelay = systemSetup->readingDelay();

        if (m_pScanFeedbackModulesWorker != NULL)
        {
            m_pScanFeedbackModulesWorker->setReadingDelay(m_readingDelay);
        }

        // save our new plugin settings to disk
        saveInterfaceSettings();
    }
}

// edit menu --------------------------------------------------------

void MainWindow::onEditLokos()
{
    debug("onEditLokos()", __FILE__, __LINE__);

    EditLokos *editLokos = new EditLokos(this);

    // editLokos->setDocumentLokos(&(getDocument()->m_lokos));
    editLokos->copyLokosFromDocumentLokos(&(getDocument()->m_lokos));

    editLokos->exec();
}

void MainWindow::onPrograms()
{
    debug("Fix me!", __FILE__, __LINE__);
}

void MainWindow::onInsertText()
{
    bool ok;
    QString text = QInputDialog::getText(this,
                "Tren Digital", tr("Enteri el text a inserir:"), QLineEdit::Normal,
                QString::null, &ok);

    if (ok && !text.isEmpty())
    {
        // user entered something and pressed OK
    }
    else
    {
        // user entered nothing or pressed Cancel
    }
}

// view menu --------------------------------------------------------

void MainWindow::onEditMode(bool checked)
{
    if (m_pView != NULL)
    {
        m_pView->setEditMode(checked);
    }

    showEditToolbars(checked);
}

void MainWindow::showEditToolbars(bool checked)
{
    if (checked)
    {
        ui->editToolBar->show();
    }
    else
    {
        ui->editToolBar->hide();
    }
}

void MainWindow::onShowLokTabBar()
{
    QAction *pSender = qobject_cast<QAction *>(sender());

    if (pSender != NULL && m_pLokoTabBar != NULL)
    {
        if (pSender->isChecked())
        {
            m_pLokoTabBar->show();
        }
        else
        {
            m_pLokoTabBar->hide();
        }
    }
}

// actions menu --------------------------------------------------------

void MainWindow::onInterfaceGo()
{
    if (m_pDoc != NULL)
    {
        Loko *pLoko = NULL;
        int addr = 0;

        for (int i=0; i<m_pDoc->m_lokos.count(); i++)
        {
            pLoko = m_pDoc->m_lokos[i];

            if (pLoko != NULL)
            {
                addr = pLoko->m_address;

                if (addr > 0)
                {
                    int speed = m_pDoc->m_lokos[i]->m_speed;

                    if (g_interface != NULL)
                    {
                        if (!g_interface->setLokoSpeed(addr, speed))
                        {
                            debug("Can't comunicate with the interface! ", __FILE__, __LINE__);
                        }
                    }
                }
            }
        }
    }

    m_programsPaused = FALSE;

    debug("A general GO command has been issued.", __FILE__, __LINE__);

}

void MainWindow::onInterfaceStop()
{
    if (m_pDoc != NULL)
    {
        Loko *pLoko = NULL;
        int addr = 0;

        for (int i=0; i<m_pDoc->m_lokos.count(); i++)
        {
            pLoko = m_pDoc->m_lokos[i];

            if (pLoko != NULL)
            {
                addr = pLoko->m_address;

                if (addr > 0)
                {
                    if (g_interface != NULL)
                    {
                        if (!g_interface->setLokoSpeed(addr, 0))
                        {
                            debug("Can't comunicate with the interface!", __FILE__, __LINE__);
                        }
                    }
                }
            }
        }
    }

    m_programsPaused = TRUE;

    debug("Emergency stop!", __FILE__, __LINE__);
}

// help menu -----------------------------------------------------------

void MainWindow::onAboutQT()
{
    qApp->aboutQt();
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, tr("Sobre Tren Digital"),
    tr("<h2>Tren Digital 0.1.12</h2>"
    "<p>Copyright &copy; 2010 Egara SYG</p>"
    "<p>Tren Digital &eacute;s una apliaci&oacute; que permet "
    "controlar un circuit de tren digitalment des de "
    "l'ordinador.</p>"));
}

void MainWindow::onAboutInterfaces()
{
    PluginDialog dialog(m_pluginsDir.path(), m_pluginFileNames, this);
    dialog.exec();
}

// misc functions ------------------------------------------------------

void MainWindow::loadSettings()
{
    QSettings settings("EgaraSYG", "TrenDigital");

    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(640, 480)).toSize());
    move(settings.value("position", QPoint(200, 200)).toPoint());
    settings.endGroup();  

    settings.beginGroup("Interface");
    // at this point we haven't loaded the interface, so we only retrieve the interface name we want to load
    m_settingsInterfaceName = settings.value("name").toString();
    settings.endGroup();
}

void MainWindow::loadInterfaceSettings()
{
    // this is loaded AFTER loading interface plugin

    if (g_interface != NULL)
    {
        debug("Loading interface settings", __FILE__, __LINE__);

        QSettings settings("EgaraSYG", "TrenDigital");

        // Is our interface stored? If not, plugin defaults will be used

        QStringList groups = settings.childGroups();

        if (groups.indexOf(g_interface->name()) >= 0) // found
        {
            settings.beginGroup(g_interface->name());
            g_interface->setCommPort(settings.value("COMMPort").toString());
            g_interface->setBaudRate(settings.value("baudRate").toInt());

            /*
            settings.value("dataBits", g_interface->m_dataBits);
            settings.value("handshake", g_interface->m_handshake);
            settings.value("parityCheck", g_interface->m_parityCheck);
            settings.value("stopBits", g_interface->m_stopBits);
            */

            g_interface->setIp(settings.value("ip").toString());

            /*
            settings.value("ipProtocol", g_interface->m_ipProtocol);
            settings.value("ipPort", g_interface->m_ipPort);
            */

            m_readingDelay = settings.value("reading_delay", DEFAULT_READING_DELAY).toInt();

            settings.endGroup();
        }
    }
}

void MainWindow::saveSettings()
{ 
    QSettings settings("EgaraSYG", "TrenDigital");

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("position", pos());
    settings.endGroup();

    saveInterfaceSettings();
}

void MainWindow::saveInterfaceSettings()
{
    if (g_interface != NULL)
    {
        debug("Saving interface settings", __FILE__, __LINE__);

        QSettings settings("EgaraSYG", "TrenDigital");

        // store default interface name
        settings.beginGroup("Interface");
        settings.setValue("name", g_interface->name());
        settings.endGroup();

        // store our interface settings
        settings.beginGroup(g_interface->name());
        settings.setValue("COMMPort", g_interface->commPort());
        settings.setValue("baudRate", g_interface->baudRate());

        /*
        settings.setValue("dataBits", g_interface->m_dataBits);
        settings.setValue("handshake", g_interface->m_handshake);
        settings.setValue("parityCheck", g_interface->m_parityCheck);
        settings.setValue("stopBits", g_interface->m_stopBits);
        */

        settings.setValue("ip", g_interface->ip());

        /*
        settings.setValue("ipProtocol", g_interface->m_ipProtocol);
        settings.setValue("ipPort", g_interface->m_ipPort);
        */

        settings.setValue("reading_delay", m_readingDelay);

        settings.endGroup();
    }
}


/*
// ----------------------------------------------------------------------------------------------
// Interface Setup

QString Document::getDigitalSystem()
{
    QSettings settings("EgaraSYG", "TrenDigital");

    return settings.value("Settings/InterfaceModel", "").toString();
}

QString Document::getInterfaceCommPort()
{
    QSettings settings("EgaraSYG", "TrenDigital");

    return settings.value("Settings/InterfaceCommPort", "").toString();
}

QString Document::getInterfaceIPAddress()
{
    QSettings settings("EgaraSYG", "TrenDigital");

    return settings.value("Settings/InterfaceIPAddress", "").toString();
}

QString Document::getInterfaceTCPPort()
{
    QSettings settings("EgaraSYG", "TrenDigital");

    return settings.value("Settings/InterfaceTCPPort", "").toString();
}

QString Document::getInterfaceIPProtocol()
{
    QSettings settings("EgaraSYG", "TrenDigital");

    return settings.value("Settings/InterfaceIPProtocol", "").toString();
}

QString Document::getInterfaceSpeed()
{
    QSettings settings("EgaraSYG", "TrenDigital");

    return settings.value("Settings/InterfaceSpeed", "").toString();
}

int Document::getReadingSpeed()
{
    QSettings settings("EgaraSYG", "TrenDigital");

    return settings.value("Settings/ReadingSpeed", 100).toInt();
}

bool Document::isTimeInMinutes()
{
    QSettings settings("EgaraSYG", "TrenDigital");

    return settings.value("Settings/TimeInMinutes", false).toBool();
}

bool Document::isUsingIntelliboxInterface()
{
    QSettings settings("EgaraSYG", "TrenDigital");

    return settings.value("Settings/UsingIntelliboxInterface", false).toBool();
}

void Document::setDigitalSystem(QString digitalSystem)
{
    QSettings settings("EgaraSYG", "TrenDigital");

    settings.setValue("Settings/DigitalSystem", digitalSystem);
}

void Document::setInterfaceCommPort(QString port)
{
    QSettings settings("EgaraSYG", "TrenDigital");

    settings.setValue("Settings/InterfaceCommPort", port);
}

void Document::setInterfaceIPAddress(QString IPAddress)
{
    QSettings settings("EgaraSYG", "TrenDigital");

    settings.setValue("Settings/InterfaceIPAddress", IPAddress);
}

void Document::setInterfaceTCPPort(QString TCPPort)
{
    QSettings settings("EgaraSYG", "TrenDigital");

    settings.setValue("Settings/InterfaceTCPPort", TCPPort);
}

void Document::setInterfaceIPProtocol(QString IPProtocol)
{
    QSettings settings("EgaraSYG", "TrenDigital");

    settings.setValue("Settings/InterfaceIPProtocol", IPProtocol);
}

void Document::setInterfaceSpeed(QString speed)
{
    QSettings settings("EgaraSYG", "TrenDigital");

    settings.setValue("Settings/InterfaceSpeed", speed);
}

void Document::setReadingSpeed(int speed)
{
    QSettings settings("EgaraSYG", "TrenDigital");

    settings.setValue("Settings/ReadingSpeed", speed);
}

void Document::setTimeInMinutes(bool timeInMinutes)
{
    QSettings settings("EgaraSYG", "TrenDigital");

    settings.setValue("Settings/TimeInMinutes", timeInMinutes);
}

void Document::setUsingIntelliboxInterface(bool status)
{
    QSettings settings("EgaraSYG", "TrenDigital");

    settings.setValue("Settings/UsingIntelliboxInterface", status);
}
*/


QString MainWindow::getLokoImageFileName(int index)
{
    return m_pDoc->getLokoImageFileName(index);
}

LokoDock * MainWindow::getLokoDock(int id)
{
    return m_pLokoTabBar->getLokoDock(id);
}

Document * MainWindow::getDocument()
{
    return m_pDoc;
}

void MainWindow::onInsertElement(QAction *pAction)
{
    debug("MainWindow::onInsertElement", __FILE__, __LINE__);

    if (pAction != NULL && m_pView != NULL)
    {
        if (pAction->isChecked())
        {
            debug("MainWindow::onInsertElement: Changing current element icon and id to " + pAction->text(), __FILE__, __LINE__);
            m_pView->setCurrentElementIcon(pAction->icon());
            m_pView->setCurrentElementId(pAction->text());
        }
        else
        {
            debug("MainWindow::onInsertElement: Changing current element to INVALID", __FILE__, __LINE__);
            m_pView->setCurrentElementId(INVALID);
        }
    }
}

void MainWindow::loadPlugins()
{
    // foreach (QObject *plugin, QPluginLoader::staticInstances())
    //    populateMenus(plugin);

    m_pluginsDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
    if (m_pluginsDir.dirName().toLower() == "debug" || m_pluginsDir.dirName().toLower() == "release")
    {
        m_pluginsDir.cdUp();
    }
#elif defined(Q_OS_LINUX)
    if (m_pluginsDir.dirName().toLower() == "debug" || m_pluginsDir.dirName().toLower() == "release")
    {
        m_pluginsDir.cdUp();
    }
#elif defined(Q_OS_MAC)
    if (m_pluginsDir.dirName() == "MacOS")
    {
        m_pluginsDir.cdUp();
        m_pluginsDir.cdUp();
        m_pluginsDir.cdUp();
    }
#endif

    debug(m_pluginsDir.absolutePath(), __FILE__, __LINE__);

    m_pluginsDir.cd("plugins");

    foreach (QString fileName, m_pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader loader(m_pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin)
        {
            debug("Loaded train interface: " + fileName, __FILE__, __LINE__);
            // populateMenus(plugin);
            m_pluginFileNames += fileName;
        }
        else
        {
            debug(loader.errorString(), __FILE__, __LINE__);
        }
    }

    if (m_pluginFileNames.count() == 0)
    {
        debug("No train interface plugin found!", __FILE__, __LINE__);
    }

    /*
    brushMenu->setEnabled(!brushActionGroup->actions().isEmpty());
    shapesMenu->setEnabled(!shapesMenu->actions().isEmpty());
    filterMenu->setEnabled(!filterMenu->actions().isEmpty());
    */
}

void MainWindow::useInterface(QString interfaceName)
{
    if (m_pluginFileNames.length() <= 0)
    {
        // maybe we haven't load our plugins yet. Try to load them
        loadPlugins();
    }

    if (m_pluginFileNames.length() <= 0)
    {
        debug("Error loading plugins!", __FILE__, __LINE__);
    }
    else
    {
        m_pluginNames.clear();

        g_interface = NULL;

        foreach (QString fileName, m_pluginsDir.entryList(QDir::Files))
        {
            QPluginLoader loader(m_pluginsDir.absoluteFilePath(fileName));
            QObject *plugin = loader.instance();
            if (plugin)
            {
                TrainInterface *pTrain = qobject_cast<TrainInterface *>(plugin);

                if (pTrain != NULL)
                {
                    m_pluginNames.append(pTrain->name());
                    if (pTrain->name().toLower() == interfaceName.toLower())
                    {
                        debug("Using \"" + interfaceName + "\" interface.", __FILE__, __LINE__);
                        g_interface = pTrain;

                        // first, load default setup
                        g_interface->setDefaultSetup();

                        // now check for user setup in our config file
                        loadInterfaceSettings();
                    }
                }
            }
        }
    }
}

// Listening thread

void MainWindow::startReadingFeedbackModules()
{
    if (g_interface != NULL)
    {
        m_pScanFeedbackModulesThread = new QThread;
        m_pScanFeedbackModulesWorker = new ScanFeedbackModules(0, g_interface, m_readingDelay);

        if (m_pScanFeedbackModulesThread != NULL &&
            m_pScanFeedbackModulesWorker != NULL)
        {
            m_pScanFeedbackModulesWorker->moveToThread(m_pScanFeedbackModulesThread);

            // connect(m_pScanFeedbackModulesWorker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));

            connect(m_pScanFeedbackModulesThread, SIGNAL(started()), m_pScanFeedbackModulesWorker, SLOT(start()));
            connect(m_pScanFeedbackModulesWorker, SIGNAL(finished()), m_pScanFeedbackModulesThread, SLOT(quit()));

            connect(m_pScanFeedbackModulesWorker, SIGNAL(finished()), m_pScanFeedbackModulesWorker, SLOT(deleteLater()));
            connect(m_pScanFeedbackModulesThread, SIGNAL(finished()), m_pScanFeedbackModulesThread, SLOT(deleteLater()));

            connect(m_pScanFeedbackModulesWorker, SIGNAL(fbModulesChanged(bool *)), this, SLOT(fbModulesChanged(bool *)));

            m_pScanFeedbackModulesThread->start();
        }
    }
    else
    {
        debug("Can't start feedback modules listening thread. Interface not loaded yet!", __FILE__, __LINE__);
    }
}

void MainWindow::fbModulesChanged(bool *fbModulesStatus)
{
    // update our gui

    debug("GUI must be updated!", __FILE__, __LINE__);
}
