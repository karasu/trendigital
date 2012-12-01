#ifndef SCANFEEDBACKMODULES_H
#define SCANFEEDBACKMODULES_H

#include <QObject>
#include <QDebug>

#include "general.h"

class TrainInterface;

class ScanFeedbackModules : public QObject
{
    Q_OBJECT
public:
    explicit ScanFeedbackModules(QObject *parent = 0, TrainInterface *interface = 0, int readingDelay = DEFAULT_READING_DELAY);
    ~ScanFeedbackModules();

    void sleep(int ms);

public slots:
    void start();
    void pause();
    void stop();

    void setReadingDelay(int readingDelay);


signals:
    void finished();
    void error(QString message);
    void fbModulesChanged(bool *fbModulesStatus);

private:

    bool m_paused;
    bool m_stop;
    TrainInterface *m_interface;
    bool m_fbModulesStatus[MAX_FB_MODULE_NUMBER];
    int m_readingDelay;
};

#endif // SCANFEEDBACKMODULES_H
