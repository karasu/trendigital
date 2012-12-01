#ifndef SCANFEEDBACKMODULES_H
#define SCANFEEDBACKMODULES_H

#include <QObject>

#include "general.h"

class TrainInterface;

class ScanFeedbackModules : public QObject
{
    Q_OBJECT
public:
    explicit ScanFeedbackModules(QObject *parent = 0, TrainInterface *interface = 0);
    ~ScanFeedbackModules();
        
public slots:
    void start();
    void pause();

signals:
    void finished();
    void error(QString message);
    void fbModulesChanged(bool *fbModulesStatus);

private:

    bool m_paused;
    TrainInterface *m_interface;
    bool m_fbModulesStatus[MAX_FB_MODULE_NUMBER];
};

#endif // SCANFEEDBACKMODULES_H
