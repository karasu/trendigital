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
#include "loko.h"
#include "element.h"
#include "cell.h"
#include "lokocommands.h"
#include "conditions.h"
#include "route.h"
#include "program.h"
#include "mainwindow.h"
#include "document.h"

Document::Document()
{
//    clear();

    if (getDigitalSystem().isEmpty())
    {
        setDefaultInterfaceValues();
    }

    load74460Codes();
    loadK83K84Codes();

    m_firstPositionEncoded = false;
    m_keyboard15 = true;
}

Document::~Document()
{
    clear();
}

void Document::setDefaultInterfaceValues()
{
    qDebug() << "Setting default values for interface configuration.";

    // Set some defaults...

    setDigitalSystem("Märklín");
    setInterfaceCommPort("COM1");
    setInterfaceSpeed("2400");
    setTimeInMinutes(false);
    setUsingIntelliboxInterface(false);

    // setInterfaceIPAddress("192.168.1.2");
    setInterfaceIPAddress("192.168.2.3");

    // these are good defaults for ECOS
    setInterfaceTCPPort("15471");
    setInterfaceIPProtocol("TCP");

    // these are good defaults for Märklín CS2
    // setInterfaceTCPPort("15731");
    // setInterfaceIPProtocol("UDP");
}

bool Document::load(const QString &fileName)
{
    qDebug() << ("Document::load()");

    m_fileName = fileName;

    QFile file(m_fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
       return false;
    }

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor)); // set wait cursor

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_6);

    QString str;

    in >> str;

    if (str != MAGIC_STRING)
    {
        QMessageBox::information(NULL, "Error", "Arxiu de dades incorrecte");
        return false;
    }

    qint32 fileVersion;
    in >> fileVersion;

    // clear previous info before loading a new file
    clear();

    // load lokos

    qint32 lokosSize;
    in >> lokosSize;

    for (int i=0; i<lokosSize; i++)
    {
        Loko *pLoko = new Loko();
        pLoko->load(in, fileVersion);
        m_lokos.insert(pLoko->m_id, pLoko);
    }

    // fixme: load other classes



    QApplication::restoreOverrideCursor();

    return true;
}

bool Document::save()
{
    QFile file(m_fileName);

    if (!file.open(QIODevice::WriteOnly))
    {
        /*
        cerr << "Cannot open file for writing: "
        << qPrintable(file.errorString()) << endl;
        */
        return false;
    }

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor)); // set wait cursor

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_6);

    out << MAGIC_STRING;
    out << CURRENT_FILE_VERSION;

    // Guardem les lokos 
    out << (qint32)m_lokos.size();

    foreach(Loko *pLoko, m_lokos)
    {
        pLoko->save(out); // saves loko
    }

    // fixme: save other classes




    QApplication::restoreOverrideCursor();

    return true;
}

bool Document::saveAs(const QString &fileName)
{
    m_fileName = fileName;

    save();

    return true;
}

void Document::clear()
{
    m_modified = false;

    // fixme: as document is no longer QObject derivated, tr doesn't work
    // m_fileName = tr("sensetitol");
    m_fileName.clear();

    foreach(Loko *pLoko, m_lokos)
    {
        delete pLoko;
    }
    m_lokos.clear();

    m_address.clear();
    m_FBModuleNumber.clear();
    m_isFBModuleActive.clear();

    m_currentProgram = 0;

    m_insertedTextInfoList.clear();

    m_lokos.clear();

    m_programs.clear();

    // m_grid.clear();

    // m_turnTableLines.clear();

    m_decoderPositions = 16;

    // For ecos and Märklín CS1
    deleteAllInterfaceLokos();
    deleteAllInterfaceSwitches();
}

QString Document::getLokoImageFileName(int id)
{
    Loko *pLoko = getLoko(id);

    if (pLoko != NULL)
    {
        return pLoko->getImageFileName();
    }

    // should return some default image?
    return QString("");
}

Loko * Document::getLoko(int id)
{
    return m_lokos.value(id, NULL);
}

QSize Document::getDocumentSize()
{
    // fixme
    return QSize(0,0);
    // return m_grid.getTotalSize();
}

int Document::getZoom()
{
    // fixme
    return 0;
    // return m_grid.getZoom();
}

void Document::setZoom(int zoom)
{
    // fixme
    // m_grid.setZoom(zoom);
}

bool Document::loadCodes(QString fileName, QList<QString> *lines)
{
    QFile file(fileName);

    if (!file.open(IO_ReadOnly))
    {
        qDebug() << ("Can't open " + fileName);
        return false;
    }

    QTextStream stream(&file);

    while (!stream.atEnd())
    {
        (*lines).append(stream.readLine());
    }

    file.close(); // when your done.

    return true;
}

void Document::deleteLoko(int id)
{
    Loko *pLoko = m_lokos.value(id, NULL);

    if (pLoko != NULL)
    {
        pLoko->clear();
        delete pLoko;
        m_lokos.remove(id);
    }
}

/////////////////////////////////////////////////////////////////////////////
// CDigitalTrainDoc serialization
/*
void Document::Serialize(CArchive& ar)
{
    CWaitCursor waitCursor;

    int iVersion;

    if (!ar.IsStoring())
    {
        QString strID;

        ar >> strID;

        if (strID != DIGITAL_TRAIN_FILE_ID_STRING)
        {
            CLog::Log(LOG_ERROR, IDS_INVALID_FILE, __FILE__, __LINE__);
            _AfxMessageBox(IDS_INVALID_FILE);
            return;
        }

        ar >> iVersion;

        if (iVersion > CURRENT_DIGITAL_TRAIN_FILE_VERSION)
        {
            CLog::Log(LOG_ERROR, IDS_WRONG_VERSION, __FILE__, __LINE__);
            _AfxMessageBox(IDS_WRONG_VERSION);
            return;
        }
        else if (iVersion < CURRENT_DIGITAL_TRAIN_FILE_VERSION)
        {
            _AfxMessageBox(IDS_DGZ_VERSION_WARNING);
        }

        for (int i=0; i<MAX_ADDRESSES; i++)
        {
            ar >> m_bAddress[i];
        }

        for (i=0; i<MAX_FB_MODULE_NUMBER; i++)
        {
            ar >> m_bFBModuleNumber[i];
            m_bIsFBModuleActive[i] = 0;
        }

        ar >> m_currentProgram;

        // Load Turn Table line points

        m_turnTableLines.Serialize(ar, iVersion);

        // Load all text annotations

        int iSize = 0;

        ar >> iSize;

        m_insertedTextInfoList.clear();

        InsertTextInfo info;

        info.bSelected = FALSE;

        for (i=0; i<iSize; i++)
        {
            ar >> info.fRect.left;
            ar >> info.fRect.right;
            ar >> info.fRect.top;
            ar >> info.fRect.bottom;
            ar >> info.text;

            m_insertedTextInfoList.push_back(info);
        }

        // ------------------------------------------------------------
        // Locomotives

        DeleteAllInterfaceLocomotives(); // Special for new TCP/IP stations

        if (iVersion < 2)
        {
            int iMaxLocomotives = 101;

            ar >> iMaxLocomotives;

            for (i=0; i<MAX_LOCOMOTIVES; i++)
            {
                m_pLocomotives[i] = new CLocomotive();
                m_pLocomotives[i]->Serialize(ar, iVersion);

                if (m_pLocomotives[i]->GetAddress() == 0 && m_pLocomotives[i]->GetID() == 0)
                {
                    // empty locomotive!

                    delete m_pLocomotives[i];
                    m_pLocomotives[i] = NULL;
                }
            }

            for(; i<iMaxLocomotives; i++)  //  Per a la versió DEMO. Només deixem una loco. Les demés es "perden".
            {
                CLocomotive l;
                l.Serialize(ar, iVersion);
            }
        }
        else
        {
            int iNumLocomotives = 0;

            ar >> iNumLocomotives;

            if (iNumLocomotives >= MAX_LOCOMOTIVES)
            {
                iNumLocomotives = MAX_LOCOMOTIVES - 1;
            }

            int iAdded = 0;

            for (int i=0; i<iNumLocomotives; i++)
            {
                int iIndex;

                ar >> iIndex;

                if (m_pLocomotives[iIndex] == NULL)
                {
                    m_pLocomotives[iIndex] = new CLocomotive();
                }

                m_pLocomotives[iIndex]->Serialize(ar, iVersion);
            }
        }

        // ---------------------------------------------------------------
        // Programs

        int iNumPrograms = 0;
        ar >> iNumPrograms;

        m_programs.clear();

        CProgram p;

        for (i=0; i<iNumPrograms; i++)
        {
            m_programs.push_back(p);

            m_programs[i].Serialize(ar, iVersion);
        }

        ar >> m_bFirstPositionEncoded;
        ar >> m_bKeyboard15;

        int iPositionsSize = 0;

        ar >> iPositionsSize;

        m_iTurnTablePositionsArray.clear();

        int iPosition;

        for (i=0; i<iPositionsSize; i++)
        {
            ar >> iPosition;

            m_iTurnTablePositionsArray.push_back(iPosition);
        }

        ar >> m_bDecoderPositions8;
        ar >> m_bDecoderPositions16;
        ar >> m_bDecoderPositions32;
    }
    else
    {
        m_grid.UnmarkAllRoutes();

        ar << QString(DIGITAL_TRAIN_FILE_ID_STRING);

        iVersion = CURRENT_DIGITAL_TRAIN_FILE_VERSION;

        ar << iVersion;

        for (int i=0; i<MAX_ADDRESSES; i++)
        {
            ar << m_bAddress[i];
        }

        for (i=0; i<MAX_FB_MODULE_NUMBER; i++)
        {
            ar << m_bFBModuleNumber[i];
        }

        ar << m_currentProgram;

        // Save Turn Table line points

        m_turnTableLines.Serialize(ar, iVersion);

        // Save all text annotations

        int iSize = (int)m_insertedTextInfoList.size();

        ar << iSize;

        InsertTextInfo info;

        for (i=0; i<iSize; i++)
        {
            info = m_insertedTextInfoList[i];

            ar << info.fRect.left;
            ar << info.fRect.right;
            ar << info.fRect.top;
            ar << info.fRect.bottom;

            ar << info.text;
        }

        // ------------------------------------------------------------

        int iNumLocomotives = GetNumLocomotives();

        if (iNumLocomotives >= MAX_LOCOMOTIVES)
        {
            iNumLocomotives = MAX_LOCOMOTIVES - 1;
        }

        ar << iNumLocomotives;

        int iAdded = 0;

        for (int iIndex=0; iIndex<MAX_LOCOMOTIVES; iIndex++)
        {
            if (m_pLocomotives[iIndex] != NULL)
            {
                ar << iIndex;

                m_pLocomotives[iIndex]->Serialize(ar, iVersion);

                iAdded++;

                if (iAdded >= MAX_LOCOMOTIVES)
                {
                    i = MAX_LOCOMOTIVES;
                    break;
                }
            }
        }

        // ---------------------------------------------------------------

        int iNumPrograms = 0;
        iNumPrograms = m_programs.size();

        ar << iNumPrograms;

        for (i=0; i<iNumPrograms; i++)
        {
            m_programs[i].Serialize(ar, iVersion);
        }

        ar << m_bFirstPositionEncoded;
        ar << m_bKeyboard15;

        int iPositionsSize = m_iTurnTablePositionsArray.size();

        ar << iPositionsSize;

        int iPosition;

        for (i=0; i<iPositionsSize; i++)
        {
            iPosition = m_iTurnTablePositionsArray[i];

            ar << iPosition;
        }

        ar << m_bDecoderPositions8;
        ar << m_bDecoderPositions16;
        ar << m_bDecoderPositions32;
    }

    m_grid.Serialize(ar, iVersion);

    if (!ar.IsStoring())
    {
        // Això ho hem de fer sempre DESPRES d'haver llegit les locomotores!
        CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();

        ASSERT(pMainFrame != NULL);

        if (pMainFrame != NULL)
        {
            pMainFrame->RestoreLocomotiveDialogBarAndControls();

            ::SetFocus(pMainFrame->GetSafeHwnd());
        }
    }

    // -----------------------------------------------------------------------

    CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();

    ASSERT(pMainFrame != NULL);

    if (pMainFrame != NULL)
    {
        char szAux[256];

        ZeroMemory(szAux, 256);

        CFile *pFile = ar.GetFile();

        QString strMsg = _T("");

        QString strFileTitle = pFile->GetFileTitle();

        int iID = 0;

        if (!ar.IsStoring())
        {
            iID = IDS_CIRCUIT_LOADED;
        }
        else
        {
            iID = IDS_CIRCUIT_SAVED;
        }

        _AfxFormatString1(strMsg, iID, strFileTitle);

//		pMainFrame->ShowPopupMsg((char *)LPCSTR(strMsg));
    }
}

// ----------------------------------------------------------------------------

void ZipFile(LPCTSTR lpszPathName)
{
    CDigitalTrainDoc *pDoc = (CDigitalTrainDoc *)GetCurrentDocument();

    if (pDoc != NULL)
    {
        pDoc->ZipFile(lpszPathName);
    }
}

void Document::ZipFile(LPCTSTR lpszPathName)
{
    zlib::CGZip gzip;

    // Load it to a buffer...

    CFile file(lpszPathName, CFile::modeRead|CFile::shareDenyWrite);

    long lFileSize = file.SeekToEnd();

    file.SeekToBegin();

    ASSERT(lFileSize > 0);

    if (lFileSize <= 0)
    {
        return;
    }

    void *pBuffer = malloc(lFileSize + 1);

    ASSERT(pBuffer != NULL);

    if (pBuffer == NULL)
    {
        return;
    }

    ZeroMemory(pBuffer, lFileSize + 1);

    CArchive ar(&file, CArchive::load | CArchive::bNoFlushOnDelete);

    ar.Read(pBuffer, lFileSize);

    ar.Close();

    file.Close();

    // Now zip it...

    if (!gzip.Open(lpszPathName, zlib::CGZip::ArchiveModeWrite))
    {
        return;
    }

    gzip.WriteBuffer(pBuffer, lFileSize);

    gzip.Close();

    free(pBuffer);

    pBuffer = NULL;
}

void Document::UnzipFile(LPCTSTR lpszPathName)
{
    zlib::CGZip gzip;

    // Open file for unzipping....

    if (!gzip.Open(lpszPathName, zlib::CGZip::ArchiveModeRead))
    {
        return;
    }

    // Read the whole file...

    unsigned int uiLength = 0;

    void * pBuffer = NULL;

    // reads and unzip the hole file, iLength contains the resulting size of pBuffer

    gzip.ReadBuffer(&pBuffer, uiLength);

    gzip.Close();

    // Now store uncompressed data

    CFile file(lpszPathName, CFile::modeCreate | CFile::modeWrite |CFile::shareDenyWrite);

    CArchive ar(&file, CArchive::store | CArchive::bNoFlushOnDelete);

    ar.Write(pBuffer, uiLength);

    ar.Close();

    file.Close();

    delete [] pBuffer;

    pBuffer = NULL;
}

void UnzipFile(LPCTSTR lpszPathName)
{
    CDigitalTrainDoc *pDoc = (CDigitalTrainDoc *)GetCurrentDocument();

    if (pDoc != NULL)
    {
        pDoc->UnzipFile(lpszPathName);
    }
}
*/

/*
BOOL Document::OnOpenDocument(LPCTSTR lpszPathName)
{
    CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();

    if (pMainFrame != NULL)
    {
        pMainFrame->ClearLocomotivesDialogBar();
    }

    Clear();

    UnzipFile(lpszPathName);

    if (!CDocument::OnOpenDocument(lpszPathName))
    {
        return FALSE;
    }

    ZipFile(lpszPathName);

    return TRUE;
}

BOOL Document::OnSaveDocument(LPCTSTR lpszPathName)
{
    BOOL bResult = CDocument::OnSaveDocument(lpszPathName);

    if (bResult)
    {
        ZipFile(lpszPathName);
    }

    // ------------------------------------------------------

    SaveThumbnailFile(QString(lpszPathName));

    return bResult;
}

void Document::OnCloseDocument()
{
    CDocument::OnCloseDocument(); // may delete this
}
*/

bool Document::isElementNumberUsed(int elementNumber)
{
    // fixme
    return false;
    // return m_grid.isElementNumberUsed(elementNumber);
}

// --------------------------------------------------------------------------------------------

// Sempre és del programa actiu 'm_currentProgram'

int Document::getNumRoutes()
{
    if (m_currentProgram >= 0 && m_currentProgram < m_programs.size())
    {
        Program p(m_programs.at(m_currentProgram));
        return p.getNumRoutes();
    }

    return 0;
}

Route Document::getRouteCopy(int route)
{
    ASSERT(m_currentProgram >= 0 && m_currentProgram < m_programs.size());

    if (m_currentProgram >= 0 && m_currentProgram < m_programs.size())
    {
        return m_programs[m_currentProgram].getRouteCopy(route);
    }

    qDebug() << ("Document::getRouteCopy: Index m_currentProgram out of bounds!");

    return (Route());
}

Route * Document::getRoute(int route)
{
    ASSERT(m_currentProgram >= 0 && m_currentProgram < m_programs.size());

    if (m_currentProgram >= 0 && m_currentProgram < m_programs.size())
    {
        m_programs[m_currentProgram].getRoute(route);
    }

    return NULL;
}

void Document::addRoute(Route r)
{
    ASSERT(m_currentProgram >= 0 && m_currentProgram < m_programs.size());

    if (m_currentProgram >= 0 && m_currentProgram < m_programs.size())
    {
        m_programs[m_currentProgram].addRoute(r);
    }
}

Route * Document::getActiveRoute()
{
    ASSERT(m_currentProgram >= 0 && m_currentProgram < m_programs.size());

    if (m_currentProgram >= 0 && m_currentProgram < m_programs.size())
    {
        Program *pProgram = getProgram(m_currentProgram);

        if (pProgram != NULL)
        {
            return getRouteByLine(pProgram->getActiveLine());
        }
    }

    return NULL;
}

void Document::deleteRoute(Route *pRoute)
{
    ASSERT(m_currentProgram >= 0 && m_currentProgram < m_programs.size());

    if (m_currentProgram >= 0 && m_currentProgram < m_programs.size())
    {
        m_programs[m_currentProgram].deleteRoute(pRoute);
    }
}

void Document::insertRoute(Route r)
{
    ASSERT(m_currentProgram >= 0 && m_currentProgram < m_programs.size());

    if (m_currentProgram >= 0 && m_currentProgram < m_programs.size())
    {
        m_programs[m_currentProgram].insertRoute(r);
    }
}

Route * Document::getRouteByLine(int line)
{
    int numRoutes = getNumRoutes();

    for (int i=0; i<numRoutes; i++)
    {
        if (getRoute(i)->getLine() == line)
        {
            return getRoute(i);
        }
    }

    ASSERT(false); // always throws an assert here

    return NULL; // Not found.
}

void Document::deleteRouteByLine(int line)
{
    Route *pRoute = getRouteByLine(line);

    if (pRoute != NULL)
    {
        deleteRoute(pRoute);
    }
}

// --------------------------------------------------------------------------------------------

void Document::addProgram(Program p, bool *pAdded)
{
    int index = getProgramByName(p.getName());

    bool added = false;

    if (index != -1)
    {
        p.copyTo(&m_programs[index]);

        added = false;
    }
    else
    {
        m_programs.push_back(p);

        added = true;
    }

    if (pAdded != NULL)
    {
        *pAdded = added;
    }
}

int Document::getProgramByName(QString name)
{
    int size = m_programs.size();

    for (int i=0; i<size; i++)
    {
        Program p(m_programs.at(i));

        if (p.getName().lower() == name.lower())
        {
            return i;
        }
    }

    return INVALID;
}

int Document::getNumPrograms()
{
    return m_programs.size();
}

Program * Document::getCurrentProgram()
{
    if (m_currentProgram >= 0 && m_currentProgram < m_programs.size())
    {
        return &m_programs[m_currentProgram];
    }

    return NULL;
}

Program * Document::getProgram(int i)
{
    ASSERT(i >= 0 && i < m_programs.size());

    if (i >= 0 && i < m_programs.size())
    {
        return &m_programs[i];
    }

    return NULL;
}

void Document::setCurrentProgramByName(QString name)
{
    for (int i=0; i<(int)m_programs.size(); i++)
    {
        if (name.lower() == m_programs[i].getName().lower())
        {
            m_currentProgram = i;
            return;
        }
    }

    qDebug() << ("Document::setCurrentProgramByName: It was not a valid program name");
    ASSERT(false);
}

void Document::setCurrentProgramByPos(int index)
{
    ASSERT(index >= 0 && index < m_programs.size());

    if (index >= 0 && index < m_programs.size())
    {
        m_currentProgram = index;
    }
    else
    {
        qDebug() << ("Document::setCurrentProgramByIndex: It was not a valid program index");
        ASSERT(false);
    }
}

void Document::deleteProgram(int index)
{
    ASSERT(index >= 0 && index < m_programs.size());

    if (index >= 0 && index < m_programs.size())
    {
        m_programs.removeAt(index);
    }
}

void Document::deleteProgramByName(QString name)
{
    for (int i=0; i<m_programs.size(); i++)
    {
        if (name.lower() == m_programs[i].getName().lower())
        {
            deleteProgram(i);
            return;
        }
    }
}

// --------------------------------------------------------------------------------------------

void Document::deleteInsertedText(int index)
{
    if (index >= 0 && index < m_insertedTextInfoList.size())
    {
        m_insertedTextInfoList.removeAt(index);
    }
}

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

// ------------------------------------------------------------------

int Document::getNumLokos()
{
    return m_lokos.size();
}

/*
void Document::SaveThumbnailFile(QString strFileName)
{
    strFileName = strFileName.Left(strFileName.ReverseFind('.')) + QString(".bmz");

    CFrameWnd *pFrameWnd = (CFrameWnd *)AfxGetApp()->m_pMainWnd;

    if (pFrameWnd == NULL)
    {
        return;
    }

    CView *pView = (CView *)pFrameWnd->GetActiveView();

    if (pView == NULL)
    {
        return;
    }

    CWindowDC dc(pView);

    CDC thumbnailDC;

    thumbnailDC.CreateCompatibleDC(&dc);

    CRect r(0, 0, THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT);

    CBitmap bitmap;

    bitmap.CreateCompatibleBitmap(&dc, r.Width(), r.Height());

    CBitmap *pOldBitmap = thumbnailDC.SelectObject(&bitmap);

    thumbnailDC.FillSolidRect(r, RGB(0, 0, 0));

    m_grid.DrawThumbnail(&thumbnailDC);

    thumbnailDC.SelectObject(pOldBitmap);

    CDIBSectionLite dibSectionLite;

    dibSectionLite.SetBitmap(bitmap);

    dibSectionLite.Save(strFileName);

    // ZipFile(LPCSTR(strFileName));
}
*/

Loko * Document::getLoko(QString name)
{
    foreach(Loko *pLoko, m_lokos)
    {
        if (pLoko->m_name == name)
        {
            return pLoko;
        }
    }

    return NULL;
}

int Document::getLokoId(QString name)
{
    foreach(Loko *pLoko, m_lokos)
    {
        if (pLoko->m_name == name)
        {
            return pLoko->m_id;
        }
    }

    return INVALID;
}

QString Document::getLokoName(int id)
{
    Loko *pLoko = m_lokos.value(id, NULL);

    if (pLoko != NULL)
    {
        return pLoko->m_name;
    }

    return "";
}

// Special for new TCP/IP stations

void Document::deleteAllInterfaceLokos()
{
    // fixme
    /*
    if (!g_interface.DeleteAllLocomotives())
    {
        CLog::Log(LOG_ERROR, IDS_INTERFACE_NOT_CONNECTED, __FILE__, __LINE__);
        _AfxMessageBox(IDS_INTERFACE_NOT_CONNECTED);
    }
    */
}

void Document::deleteAllInterfaceSwitches()
{
    /*
    if (!g_interface.DeleteAllSwitches())
    {
        CLog::Log(LOG_ERROR, IDS_INTERFACE_NOT_CONNECTED, __FILE__, __LINE__);
        _AfxMessageBox(IDS_INTERFACE_NOT_CONNECTED);
    }
    */
}

void Document::setFBModuleStatus(int i, bool status)
{
    if (i >= 0)
    {
        m_FBModuleNumber[i] = status;
    }
}

void Document::setAddressStatus(int i, bool status)
{
    if (i >= 0)
    {
        m_address[i] = status;
    }
}
