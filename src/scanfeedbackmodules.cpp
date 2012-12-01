#include "scanfeedbackmodules.h"
#include "traininterface.h"

ScanFeedbackModules::ScanFeedbackModules(QObject *parent, TrainInterface *interface) :
    QObject(parent)
{
    // you could copy data from constructor arguments to internal variables here.

    m_interface = interface;

    memset(m_fbModulesStatus, 0, MAX_FB_MODULE_NUMBER * sizeof(bool));
}

ScanFeedbackModules::~ScanFeedbackModules()
{
    // free resources here
}

void ScanFeedbackModules::start()
{
    // allocate resources using new here

    m_paused = false;

    if (m_interface == NULL)
    {
        return;
    }

    // m_fbModulesStatus

    emit fbModulesChanged(m_fbModulesStatus);

    // start processing
}

void ScanFeedbackModules::pause()
{
    m_paused = !m_paused;
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
