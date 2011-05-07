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

#include "plugindialog.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialogs/lokotabbar.h"
#include "dialogs/lokodock.h"
#include "dialogs/editlokos.h"

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
    qDebug() << ("MainWindow::MainWindow()");

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
    qDebug() << ("Program settings loaded.");

    // Load interface plugin
    // QPluginLoader

    // Check serial key

    // Check for updates

    // Add menu and toolbar connections
    addConnections();

    // Create a new empty document by default
    // m_pDoc = new Document(this);
    m_pDoc = new Document();
    // m_pDoc->clear();
    qDebug() << ("New empty document created.");

    // Create lok tab bar (where all lokos are shown)

    m_pLokoTabBar = new LokoTabBar(this);
    qDebug() << ("New Loko Tab Bar created.");

    // QMessageBox::information(this, "Error", "Could not load the plugin");

    // this->setCentralWidget(ui->graphicsView);

    m_pView = ui->graphicsView;

    m_printer = new QPrinter( QPrinter::HighResolution );

    // doc->setModified();

    showEditToolbars(false);

    qDebug() << ("Loading train interfaces");
    loadPlugins();

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

    qDebug() << ("Test lokos created.");
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
#elif defined(Q_OS_MAC)
    if (m_pluginsDir.dirName() == "MacOS")
    {
        m_pluginsDir.cdUp();
        m_pluginsDir.cdUp();
        m_pluginsDir.cdUp();
    }
#endif

    m_pluginsDir.cd("plugins");

    foreach (QString fileName, m_pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader loader(m_pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin)
        {
            qDebug() << QString("Loaded train interface: ") + fileName;
            // populateMenus(plugin);
            m_pluginFileNames += fileName;
        }
        else
        {
            qDebug() << loader.errorString();
        }
    }

    if (m_pluginFileNames.count() == 0)
    {
        qDebug() << ("No train interface plugin found!");
    }
    /*
    brushMenu->setEnabled(!brushActionGroup->actions().isEmpty());
    shapesMenu->setEnabled(!shapesMenu->actions().isEmpty());
    filterMenu->setEnabled(!filterMenu->actions().isEmpty());
    */
}

void MainWindow::closeEvent(QCloseEvent *event)
{
// fixme
//    if (askIfSaveBeforeCloseDocument())
//    {
        m_pLokoTabBar->close();

        // fixme
        // saveSettings();

        event->accept();
//    }
//    else
//    {
//        event->ignore();
//    }
}

void MainWindow::addConnections()
{
    connect(ui->actionNou, SIGNAL(triggered()), this, SLOT(onNew()));
    connect(ui->actionObrir, SIGNAL(triggered()), this, SLOT(onOpen()));
    connect(ui->actionSortir, SIGNAL(triggered()), this, SLOT(onExit()));
    connect(ui->actionGuardar, SIGNAL(triggered()), this, SLOT(onSave()));
    connect(ui->actionGuardar_com, SIGNAL(triggered()), this, SLOT(onSaveAs()));
    connect(ui->actionInsertar_text, SIGNAL(triggered()), this, SLOT(onInsertText()));
    connect(ui->actionBarra_locomotores, SIGNAL(triggered()), this, SLOT(onShowLokTabBar()));
    connect(ui->actionSobre_TrenDigital, SIGNAL(triggered()), this, SLOT(onAbout()));
    connect(ui->actionSobre_QT, SIGNAL(triggered()), this, SLOT(onAboutQT()));
    connect(ui->actionLocomotores, SIGNAL(triggered()), this, SLOT(onEditLokos()));
    connect(ui->actionConfigurar_impresora, SIGNAL(triggered()), this, SLOT(onPrinterSetup()));
    connect(ui->actionMode_edicio, SIGNAL(triggered(bool)), this, SLOT(onEditMode(bool)));
    connect(ui->actionSobre_Interficies, SIGNAL(triggered()), this, SLOT(onAboutInterfaces()));

    // EditToolbar actions
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

bool MainWindow::askIfSaveBeforeCloseDocument()
{
    if (m_pDoc->isModified())
    {
        int r = QMessageBox::warning(this, tr("Tren Digital"),
        tr("El circuit, les màquines o els programes s'han modificat.\n"
           "Desitges guardar els canvis?"),
        QMessageBox::Yes | QMessageBox::Default,
        QMessageBox::No,
        QMessageBox::Cancel | QMessageBox::Escape);

        if (r == QMessageBox::Yes)
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

void MainWindow::loadSettings()
{
    QSettings settings;
    settings.beginGroup("MainWindow");

    resize(settings.value("size", QSize(640, 480)).toSize());
    move(settings.value("position", QPoint(200, 200)).toPoint());

    settings.endGroup();
}

void MainWindow::saveSettings()
{ 
    QSettings settings;
    settings.beginGroup("MainWindow");

    settings.setValue("size", size());
    settings.setValue("position", pos());

    settings.endGroup();
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

void MainWindow::onShowLokTabBar()
{
    QAction *pSender = qobject_cast<QAction *>(sender());

    if (pSender->isChecked())
    {
        m_pLokoTabBar->show();
    }
    else
    {
        m_pLokoTabBar->hide();
    }
}

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

void MainWindow::onEditLokos()
{
    qDebug() << ("MainWindow::onEditLokos()");

    EditLokos *editLokos = new EditLokos(this);

    // editLokos->setDocumentLokos(&(getDocument()->m_lokos));
    editLokos->copyLokosFromDocumentLokos(&(getDocument()->m_lokos));

    editLokos->exec();
}

void MainWindow::onPrinterSetup()
{
}

QImage * MainWindow::getBitmapPtr(QString fileName)
{
    return m_dibs.value(fileName, NULL);
}

void MainWindow::onAboutInterfaces()
{
    PluginDialog dialog(m_pluginsDir.path(), m_pluginFileNames, this);
    dialog.exec();
}

void MainWindow::onInsertElement(QAction *pAction)
{
    qDebug() << ("MainWindow::onInsertElement");

    if (pAction != NULL && m_pView != NULL)
    {
        if (pAction->isChecked())
        {
            qDebug() << ("MainWindow::onInsertElement: Changing current element icon and id");
            m_pView->setCurrentElementIcon(pAction->icon());
            m_pView->setCurrentElementId(pAction->text());
        }
        else
        {
            qDebug() << ("MainWindow::onInsertElement: Changing current element to INVALID");
            m_pView->setCurrentElementId(INVALID);
        }
    }
}