#ifndef SYSTEMSETUP_H
#define SYSTEMSETUP_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class SystemSetup;
}

class SystemSetup : public QDialog
{
    Q_OBJECT
    
public:
    explicit SystemSetup(QWidget *parent = 0);
    ~SystemSetup();

    inline void setPluginNames(QStringList l) { m_pluginNames = l; }

    QStringList m_pluginNames;
    
private:
    Ui::SystemSetup *ui;
};

#endif // SYSTEMSETUP_H
