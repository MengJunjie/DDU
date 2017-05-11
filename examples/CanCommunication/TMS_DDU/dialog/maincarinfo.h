#ifndef MAINCARINFO_H
#define MAINCARINFO_H

#include <QDialog>
#include <QTextEdit>

#include "gui_config.h"

enum AIR_PUMP_RUN_STATE{
    AIR_PUMP_STOP_WORK,
    AIR_PUMP_WORKING
};

namespace Ui {
class MainCarInfo;
}

class MainCarInfo : public QDialog
{
    Q_OBJECT
    
public:
    explicit MainCarInfo(QWidget *parent = 0);
    ~MainCarInfo();
    
    void Init();
    void setVoltage();
    void setCurrent();
    void setBrakeLevel(int iState, int iValue);
    void setSpeed();
    void setElecBrake(DRAG_ELEC_BRAKE_TYPE type, CAR_ID carId, DRAG_ELEC_BRAKE_STATE level, int iValue);
    void setBC_KPA(CAR_ID carId, BC_KPA_STATE state, int value = 0);
    void setAirPumpRun(CAR_ID carId, AIR_PUMP_RUN_STATE workstate);
    void setSIVValue(CAR_ID carId, SIV_STATE workstate, int iValue1, int iValue2);
    void setBHB_BLB(CAR_ID carId, bool bBHBState,  bool bBLBState);
    void setUrgentShort(CAR_ID carId, bool bState);
    void setDoorBrake(CAR_ID carId, bool bState);

signals:
    void sig_LowPBtn_Clicked(int btnId);

public slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonmenu_clicked();
    void on_buttonaircondition_clicked();
    void on_buttonerr_clicked();
    void on_buttonrun_clicked();

    void slot_CarIcon_Update();
    void slot_CarCode_Update();
    void slot_Car_Direction_Update();
    void slot_AirPump_State_Update(CAR_ID carId, bool bState);
    void slot_SIV_Direction_Change(SIV_TX_DIRECTION sivDirection);
    void slot_BHB_BLB_State_Change(CAR_ID carId, bool bBHBFlag, bool bBLBFlag);
    void slot_UrgentShort_Change(CAR_ID carId, bool bState);
    void slot_BrakeIsolateDoor_Change(CAR_ID carId, bool bState);
    void slot_VVVF_BCU_Level_Update(int iState, int iValue);
    void slot_VVVF_Elec_BCU_Update(CAR_ID carId, int vvvfIndex, int iState, int iValue);
    void slot_BCU_BC_Kpa_Update(CAR_ID carId, int iState, int iValue);
    void slot_SIV_Voltage_Freq_Update(CAR_ID carId, int iState, int iValue1, int iValue2);

private:
    Ui::MainCarInfo *ui;

//    int iVVVFBCULevelState, iVVVFBCULevelValue;
    DRAG_ELEC_BRAKE_STATE VVVFElecBrakeState[6];
    int iVVVFElecBrakeStateValue[6];
};


#endif // MAINCARINFO_H
