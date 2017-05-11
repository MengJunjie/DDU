#ifndef SPEEDCHECK_H
#define SPEEDCHECK_H

#include <QDialog>
#include <QDateTime>
#include "gui_config.h"

#define SPEED_CHECK_1S          1000
#define SPEED_CHECK_500MS   500

namespace Ui {
class speedcheck;
}

class speedcheck : public QDialog
{
    Q_OBJECT
    
public:
    explicit speedcheck(QWidget *parent = 0);
    ~speedcheck();
    
    void Init();
    void setVoltage();
    void setCurrent();
    void setBrakeLevel(BRAKE_LEVEL_STATE level);
    void setSpeed();
    void setElecBrake(DRAG_ELEC_BRAKE_TYPE type, CAR_ID carId, DRAG_ELEC_BRAKE_STATE level, int iValue);
    void setBC_KPA(CAR_ID carId, BC_KPA_STATE state, int value = 0);
    void setBHB_BLB(CAR_ID carId, bool bBHBState,  bool bBLBState);
    void setUrgentShort(CAR_ID carId, bool bState);
    void speedChecking();
    void getSpeedCheckResult();
    void showNextScreen(int iShowFlag);

signals:
    void sig_Speed_Check_Start();
    void sig_Speed_CheckData_Update();

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonstart_clicked();
    void on_buttonstop_clicked();
    void on_buttonchange_clicked();
    void on_buttonquit_clicked();

    void slot_CarIcon_Update();
    void slot_CarCode_Update();
    void slot_Car_Direction_Update();

    void slot_VVVF_BCU_Level_Update(int iState, int iValue);
    void slot_VVVF_Elec_BCU_Update(CAR_ID carId, int vvvfIndex, int iState, int iValue);
    void slot_BCU_BC_Kpa_Update(CAR_ID carId, int iState, int iValue);
    void slot_BHB_BLB_State_Update(CAR_ID carId, bool bBHBFlag, bool bBLBFlag);
    void slot_UrgentShort_Update(CAR_ID carId, bool bState);
    void slot_VVVF_Elec_Brake_State_Update(bool bState);

    void slot_TimerOut_1S();
    void slot_TimerOut_Distance_1S();
    void slot_TimerOut_500MS();

public:
    bool bStartCheckFlag, b500MSFlag;
//    int iNowTestFalg;  //0：默认，1：加速度1测试，2：加速度2测试，3：减速度测试；
    int iPlus1Flag, iPlus2Flag, iMinusFlag;  //0：非测试状态，1：测试中，2：测试完成；
    int iVVVFBCULevelState, iVVVFBCULevelValue, iP1PWMValue, iP2PWMValue, iMPWMValue;
    int iCheckTimes;
    int iPlusS1, iPlusS2, iPlusS3, iMinusP1, iMinusP2, iRunDistance;
    float fRunDistance;
    QDateTime mQTPlus1, mQTPlus2, mQTPlus3;
    QString strStartPlus, strStartMinus;
    bool bPlus1OkFlag, bPlus2OkFlag, bMinusOkFlag;
    int iRebornFlag;  //0：默认，1：无再生，2：有再生

    QList<structSpeedCheck*> speedDataList;
    bool bNextScreenFlag;

private:
    Ui::speedcheck *ui;
    QTimer * timer1S;
    QTimer * timer1S_Distance;
    QTimer * timer500ms;
};

#endif // SPEEDCHECK_H
