#include <QApplication>
#include <QTextCodec>
#include <QFile>
#include <QMetaType>

#include "mainwidget.h"
#include "gui_config.h"
#include "../cansignalsslots.h"

int deviceCarId;
bool nowCtrlHMIFlag;
bool masterHMIFlag;
bool atoModelFlag;
int carTempValue, carVoltageValue[2] = {0}, carCurrentValue[2] = {0}, carSpeedValue[2] = {0};
int carCodeNum[6] = {0};
CAR_HEADER carHeaderFlag;
CAR_HEADER lastCarHeaderFlag;
bool can1OperationalFlag;
bool can2OperationalFlag;
CAN_WORKING canWorkFlag;
CanSignalsSlots * canSignalsSlots;
bool bCCUJudgeFlag;
//int mCCUWorkFlag;  //0：无CCU工作，1：CCU1工作，2：CCU2工作；
bool bCan1HMI1HeartFlag = true;   //Can1网络HMI2监听HMI1心跳
bool bCan2HMI1HeartFlag = true;   //Can2网络HMI2监听HMI1心跳
bool bPTUDownloading = false;       //PTU下载故障跟踪数据标志

int main(int argv,char  *argvs[] )
{
    QApplication m_application(argv,argvs);
    QTextCodec * codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    qRegisterMetaType<CAN_STATE>("CAN_STATE");
    qRegisterMetaType<CAR_ID>("CAR_ID");
    qRegisterMetaType<DOOR_ID>("DOOR_ID");
    qRegisterMetaType<RUN_DOOR_STATE>("RUN_DOOR_STATE");
    qRegisterMetaType<SIV_TX_DIRECTION>("SIV_TX_DIRECTION");

    canSignalsSlots = new CanSignalsSlots();

    QApplication::setOverrideCursor(Qt::BlankCursor);

    QFile file(":/new/prefix1/qss/common.qss");
    file.open(QFile::ReadOnly);
    m_application.setStyleSheet(file.readAll());
    file.close();

    MainWidget mainWidget;
    mainWidget.show();

    return  m_application.exec();
}
