#ifndef SYSTEMSETUP_H
#define SYSTEMSETUP_H

#include <QDialog>
#include <QStringList>

#include "../traininterface.h"

namespace Ui {
class SystemSetup;
}

class SystemSetup : public QDialog
{
    Q_OBJECT
    
public:
    explicit SystemSetup(QWidget *parent = 0);
    ~SystemSetup();

    void setPluginNames(QStringList pluginNameList, QString pluginName);
    QString pluginName(void);

    void setCommPort(QString port);

    void setSpeed(int baudRate);

    QStringList m_pluginNames;

    void saveSettings(TrainInterface *interface);

private:
    Ui::SystemSetup *ui;
};

#endif // SYSTEMSETUP_H