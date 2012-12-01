#include "scanfeedbackmodules.h"
#include "traininterface.h"

ScanFeedbackModules::ScanFeedbackModules(QObject *parent, TrainInterface *interface, int readingDelay) :
    QObject(parent)
{
    // you could copy data from constructor arguments to internal variables here.

    m_interface = interface;

    m_stop = false;

    m_paused = false;

    m_readingDelay = readingDelay;

    memset(m_fbModulesStatus, 0, MAX_FB_MODULE_NUMBER * sizeof(bool));
}

ScanFeedbackModules::~ScanFeedbackModules()
{
    // free resources here
}

void ScanFeedbackModules::start()
{
    // allocate resources using new here

    if (m_interface == NULL)
    {
        emit finished();
        return;
    }

    bool moduleStatus[S88_POSITIONS];

    memset(moduleStatus, 0, S88_POSITIONS * sizeof(bool));

    int moduleSize = S88_POSITIONS * sizeof(bool);

    int values[MAX_S88];

    for (int i=0; i<MAX_S88; i++)
    {
        values[i] = i * S88_POSITIONS;
    }

    while (!m_stop)
    {
        for (int i=0; i<MAX_S88; i++)
        {
            if (!m_paused)
            {
                qDebug() << "Polling interface with a delay of " + QString::number(m_readingDelay) + "ms...";

                if (m_interface->readFeedBackModule(i + 1, moduleStatus))
                {
                    bool *pAddr = m_fbModulesStatus + 1 + values[i];

                    if (memcmp(pAddr, moduleStatus, moduleSize) != 0)
                    {
                        memcpy(pAddr, moduleStatus, moduleSize);

                        emit fbModulesChanged(m_fbModulesStatus);
                    }
                }

                sleep(m_readingDelay);
            }

            if (m_stop)
            {
                break;
            }
        }
    }

    emit finished();
}

void ScanFeedbackModules::sleep(int ms)
{
    if (ms > 0)
    {
#ifdef Q_OS_WIN
        Sleep(uint(ms));
#else
        struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
        nanosleep(&ts, NULL);
#endif
    }
}

void ScanFeedbackModules::setReadingDelay(int readingDelay)
{
    m_readingDelay = readingDelay;
}

void ScanFeedbackModules::pause()
{
    m_paused = !m_paused;
}

void ScanFeedbackModules::stop()
{
    m_stop = true;
}

/*
UINT ScanFBModulesCS2Thread(LPVOID pParam)
{
    ThreadInfo *pThreadInfo = (ThreadInfo *)pParam;

    BOOL *pbIsFBModuleActive = pThreadInfo->pbIsFBModuleActive;

    if (pbIsFBModuleActive == NULL)
    {
        CLog::Log(LOG_ERROR, "Invalid parameter to the cs2 listener thread", __FILE__, __LINE__);

        return 1; // error
    }

    HWND hWnd = pThreadInfo->hWnd;

    if (!::IsWindow(hWnd))
    {
        CLog::Log(LOG_ERROR, "Invalid parameter to the cs2 listener thread (hWnd is not a real window!)", __FILE__, __LINE__);

        return 1; // error
    }

    if (!g_interface.IsOpen())
    {
        CLog::Log(LOG_ERROR, "The interface comunications are not yet established. Can't read feedback modules.", __FILE__, __LINE__);

        return 1; // error
    }

    g_interface.SetMutex(pThreadInfo->hMutex);

    pThreadInfo->bRunning = TRUE;

    CLog::Log(LOG_INFO, "Special CS2 listener thread for reading feedback modules started.", __FILE__, __LINE__);

    while (!pThreadInfo->bStop)
    {
        if (!pThreadInfo->bPause)
        {
            // La CS2 necessita separar les funcions de pregunta/resposta de l'estat dels contactes
            // Així la funció ReadFeedBackModule només pregunta (pel modul que toca)
            // Es la funció ReadFeedBackModules que obté l'estat

            if (g_interface.ReadFeedBackModules(pbIsFBModuleActive))
            {
                ::PostMessage(hWnd, UM_FBMODULE_CHANGED, 0, 0);
            }
        }
    }

    pThreadInfo->bRunning = FALSE;

    CLog::Log(LOG_INFO, "CS2 Listener thread stopped.", __FILE__, __LINE__);

    return 0; // all ok.
}

UINT ScanFBModulesThread(LPVOID pParam)
{
    ThreadInfo *pThreadInfo = (ThreadInfo *)pParam;

    BOOL *pbIsFBModuleActive = pThreadInfo->pbIsFBModuleActive;

    if (pbIsFBModuleActive == NULL)
    {
        CLog::Log(LOG_ERROR, "Invalid parameter to the listener thread", __FILE__, __LINE__);

        return 1; // error
    }

    HWND hWnd = pThreadInfo->hWnd;

    if (!::IsWindow(hWnd))
    {
        CLog::Log(LOG_ERROR, "Invalid parameter to the listener thread (hWnd is not a real window!)", __FILE__, __LINE__);

        return 1; // error
    }

    BOOL bModuleStatus[S88_POSITIONS];

    memset(bModuleStatus, 0, S88_POSITIONS);

    int iModuleSize = S88_POSITIONS * sizeof(BOOL);

    int iValues[MAX_S88];

    for (int i=0; i<MAX_S88; i++)
    {
        iValues[i] = i * S88_POSITIONS;
    }

    if (!g_interface.IsOpen())
    {
        CLog::Log(LOG_ERROR, "The interface comunications are not yet established. Can't read feedback modules.", __FILE__, __LINE__);

        return 1; // error
    }

    g_interface.SetMutex(pThreadInfo->hMutex);

    pThreadInfo->bRunning = TRUE;

    i = 0;

    CLog::Log(LOG_INFO, "Listener thread for reading feedback modules started.", __FILE__, __LINE__);

    while (!pThreadInfo->bStop)
    {
        for (int i=0; i<MAX_S88; i++)
        {
            if (!pThreadInfo->bPause)
            {
                if (g_interface.ReadFeedBackModule(i+1, bModuleStatus))
                {
                    BOOL *pAddr = pbIsFBModuleActive + 1 + iValues[i];

                    if (memcmp(pAddr, bModuleStatus, iModuleSize) != 0)
                    {
                        memcpy(pAddr, bModuleStatus, iModuleSize);

                        ::PostMessage(hWnd, UM_FBMODULE_CHANGED, 0, 0);
                    }
                }

                Sleep(pThreadInfo->iReadingSpeed);
            }

            if (pThreadInfo->bStop)
            {
                break;
            }
        }
    }

    pThreadInfo->bRunning = FALSE;

    CLog::Log(LOG_INFO, "Listener thread stopped.", __FILE__, __LINE__);

    return 0; // all ok.
}
*/
