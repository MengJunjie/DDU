#include <QDateTime>
#include "gui_config.h"
#include "stdio.h"
#include <unistd.h>

QString updateDateTime(){
    QString strDateTime = QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss");

    return strDateTime;
}

void buttonBeep(){
    QFile file(QString("/sys/class/leds/gpio4_18/brightness"));

    if(file.open(QIODevice::WriteOnly)){
        file.write("1");
        file.close();
    }
    while (1) {
        usleep(20000);
        break;
    }
    if(file.open(QIODevice::WriteOnly)){
        file.write("0");
        file.close();
    }
}
