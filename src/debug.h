#ifndef DEBUG_H
#define DEBUG_H

#include <QString>
#include <QDebug>

// QMessageBox::warning(this, tr("Tren Digital"), selectedImage);

class Debug {
public:
    Debug();

    static void showMsg(QString msg, QString file, int line)
    {
        QString strline;

        strline.setNum(line);

        qDebug() << file + (":") + strline + (": ") + msg;
    }
};

#define debug(x,y,z) Debug::showMsg(x,y,z)

#endif // DEBUG_H
