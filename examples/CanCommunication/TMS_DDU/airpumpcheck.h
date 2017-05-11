#ifndef AIRPUMPCHECK_H
#define AIRPUMPCHECK_H

#include <QDialog>

#include "gui_config.h"

#define ACP_CHECK_TOME_OUT 1000

enum AIRPUMP_ACTION_TYPE{
    ACTION_NOSET,
    ACTION_STOPED,
    ACTION_RUNING
};

enum AIRPUMP_RUNTIME_TYPE{
    RUNTIME_STANDBY,
    RUNTIME_JUDGING,
    RUNTIME_50SEC
};

namespace Ui {
class airpumpcheck;
}

class airpumpcheck : public QDialog
{
    Q_OBJECT
    
public:
    explicit airpumpcheck(QWidget *parent = 0);
    ~airpumpcheck();

    void Init();
    void setVoltage();
    void setCurrent();
    void setAirpumpAction(CAR_ID carId, AIRPUMP_ACTION_TYPE type);
    void setActionTime(CAR_ID carId, AIRPUMP_RUNTIME_TYPE type, int iValue=0);

signals:
    void sig_ACP_Check_Start();

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonstrart_clicked();
    void on_buttonstop_clicked();
    void on_buttonrecord_clicked();
    void on_buttonquit_clicked();

    void slot_CarIcon_Update();
    void slot_CarCode_Update();
    void slot_Car_Direction_Update();
    void slot_AirPump_State_Change(CAR_ID carId, bool bState);

    void slot_ACP1_Check_TimeOut();
    void slot_ACP2_Check_TimeOut();

private:
    Ui::airpumpcheck *ui;
    QTimer * timerACP1;
    QTimer * timerACP2;
    bool bCheckStartFlag, bACP1CheckFlag, bACP2CheckFlag;
    int iACP1WorkTime, iACP2WorkTime;

    AIRPUMP_ACTION_TYPE iACP1ActionFlag, iACP2ActionFlag;
    AIRPUMP_RUNTIME_TYPE iACP1ResultFlag, iACP2ResultFlag;
};

#endif // AIRPUMPCHECK_H
