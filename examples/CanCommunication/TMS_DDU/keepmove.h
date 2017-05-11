#ifndef KEEPMOVE_H
#define KEEPMOVE_H

#include <QDialog>
#include "gui_config.h"

enum KEEPMOVE_ITEM{         //运行界面显示内容
    RUN_ELEC_BRAKE_1,       //牵引电制动1
    RUN_ELEC_BRAKE_2,       //牵引电制动2
    RUN_BC_KPA,             //BC压力
    RUN_RIGHT_DOOR,         //右侧门状态
    RUN_LEFT_DOOR,          //左侧门状态
    RUN_RIDE_RATE,          //乘车率
    RUN_INSIDE_TEMP,        //车内温度
    RUN_URGENT_ALERT,       //紧急报警
    RUN_URGENT_SHORT,       //紧急短路
    RUN_BRAKE_PLUG_DOOR     //制动塞门
};

enum RUN_TYPE{              //运行类别
    TYPE_NORMAL,            //普通
    TYPE_GOOUT_COMEBACK,    //出入库
    TYPE_RUN_TEST           //试运行
};

enum RUN_RIDE_RATE_STATE{   //乘车率显示
    RIDE_RATE_COMN_ERR,     //通讯异常
    RIDE_RATE_POWER_OFF,    //电源OFF
    RIDE_RATE_BREAKDOWN,    //故障
    RIDE_RATE_NORMAL        //正常显示
};

namespace Ui {
class keepmove;
}

class keepmove : public QDialog
{
    Q_OBJECT
    
public:
    explicit keepmove(QWidget *parent = 0);
    ~keepmove();
    
    void Init();
    void setDoorName();
    void setVoltage();
    void setCurrent();
//    void setEndStation(int endIndex);
//    void setNextStation(int nextIndex);
//    void setCurrentStation(int curIndex);
    void setElecBrake(DRAG_ELEC_BRAKE_TYPE type, CAR_ID carId, DRAG_ELEC_BRAKE_STATE level, int iValue);
    void setBC_KPA(CAR_ID carId, BC_KPA_STATE state, int value = 0);
    void setDoorState(KEEPMOVE_ITEM item, CAR_ID carId, DOOR_ID doorId, RUN_DOOR_STATE doorState);
    void setLoadRate(CAR_ID carId, RUN_RIDE_RATE_STATE state, int value);
    void setInsideTEMP(CAR_ID carId, INSIDE_TEMP_STATE state, int tempValue = 0, int errCode = 0);
    void setUrgent_Alert(CAR_ID carId , bool bState);
    void setUrgent_Short(CAR_ID carId , bool bState);
    void setBrakeIsolateDoor(CAR_ID carId , bool bState);

signals:
    void sig_LowPBtn_Clicked(int btnId);
    void sig_buttonruntable_clicked();
    void sig_buttonforward_clicked();
    void sig_buttonback_clicked();

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonmenu_clicked();
    void on_buttoncarinfo_clicked();
    void on_buttonaccset_clicked();
    void on_buttonerr_clicked();

    void slot_CarIcon_Update();
    void slot_CarCode_Update();
    void slot_Car_Direction_Update();
    void slot_ATOModel_Update(int model);
    void slot_Door_State_Update(CAR_ID carId, DOOR_ID doorId, RUN_DOOR_STATE doorState);
    void slot_VVVF_Elec_BCU_Update(CAR_ID carId, int vvvfIndex, int iState, int iValue);
    void slot_BCU_BC_Kpa_Update(CAR_ID carId, int iState, int iValue);
    void slot_Load_Rate_Update(CAR_ID carId, int iState, int iValue);
    void slot_ACC_Temp_Update(CAR_ID carId, int iState, int iValue1, int iValue2);
    void slot_Urgent_Alert_Update(CAR_ID carId, bool bState);
    void slot_UrgentShort_Update(CAR_ID carId, bool bState);
    void slot_BrakeIsolateDoor_Update(CAR_ID carId, bool bState);
    void slot_Car_Arrive_Station_Update(int iFlag, int stationId);

private:
    Ui::keepmove *ui;
};

#endif // KEEPMOVE_H
