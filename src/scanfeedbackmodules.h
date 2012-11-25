#ifndef SCANFEEDBACKMODULES_H
#define SCANFEEDBACKMODULES_H

#include <QObject>

class ScanFeedbackModules : public QObject
{
    Q_OBJECT
public:
    explicit ScanFeedbackModules(QObject *parent = 0);
    ~ScanFeedbackModules();
    
signals:
    
public slots:
    void start();
    void pause();

signals:
    void finished();
    void error(QString message);

private:
    
};

#endif // SCANFEEDBACKMODULES_H
