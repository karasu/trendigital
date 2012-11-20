#ifndef SYSTEMSETUP_H
#define SYSTEMSETUP_H

#include <QDialog>

namespace Ui {
class SystemSetup;
}

class SystemSetup : public QDialog
{
    Q_OBJECT
    
public:
    explicit SystemSetup(QWidget *parent = 0);
    ~SystemSetup();
    
private:
    Ui::SystemSetup *ui;
};

#endif // SYSTEMSETUP_H
