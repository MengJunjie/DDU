#ifndef AIRCONDITIONINFO_H
#define AIRCONDITIONINFO_H

#include <QDialog>
#include <QTimer>
#include <QTableWidgetItem>
#include <QHeaderView>
#include "gui_config.h"

enum AIRCONDITIONSTATE_ITEM{
    CAR_TEMP,                               //车内温度
    MACHINE_1_STATE,                 //机组1状态
    MACHINE_2_STATE,                 //机组2状态
    MACHINE_1_BREAKDOWN,    //机组1故障
    MACHINE_2_BREAKDOWN     //机组2故障
};

enum MACHINE_STATE{
    AIR_TEMP_COMO_ERR,     //通讯异常
    AIR_AUTO,                          //自动
    AIR_AERATION,                  //通风
    AIR_WORK_STOP,              //停机
    AIR_HALF_COLD,               //半冷，弱冷
    AIR_DEEP_COLD,               //强冷
    AIR_EMR_VENT,                 //紧急通风
    AIR_HALF_HOT,                 //半暖
    AIR_ALL_HOT                    //全暖
};

enum ACC_MODE_STATE{
    ACCMODE_COMO_ERR,                //通讯异常
    ACCMODE_WORK_STOP,              //停机
    ACCMODE_AERATION,                  //通风
    ACCMODE_AUTO,                          //自动
    ACCMODE_NET_CTRL,                  //网控
    ACCMODE_HALF_COLD,               //半冷，弱冷
    ACCMODE_DEEP_COLD,               //强冷
    ACCMODE_HALF_HOT,                 //半暖
    ACCMODE_ALL_HOT                    //全暖
};

namespace Ui {
class airconditioninfo;
}

class airconditioninfo : public QDialog
{
    Q_OBJECT
    
public:
    explicit airconditioninfo(QWidget *parent = 0);
    ~airconditioninfo();
    
    void Init();
    void setTEMP(CAR_ID carId, INSIDE_TEMP_STATE state, int tempValue = 0, int errCode = 0);
    void setAirconditionState(AIRCONDITIONSTATE_ITEM item, CAR_ID carId, int iState, MACHINE_STATE state);
    void setAirconditionError(AIRCONDITIONSTATE_ITEM item, CAR_ID carId, int iState,  int errCode);

signals:
    void sig_ACC_BackPBtn_Clicked();

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonTempUp_clicked();
    void on_buttonTempDown_clicked();
    void on_buttonmenu_clicked();

    void slot_CarIcon_Update();
    void slot_CarCode_Update();
    void slot_Car_Direction_Update();
    void slot_ACCData_Change(CAR_ID carId, int iState, int acc1Mode, int acc2Mode, int acc1Err, int acc2Err);
    void slot_ACC_Temp_Update(CAR_ID carId, int iState, int iValue1, int iValue2);
    void slot_ACCCtrlMode(int ctrlMode);
    void slot_ACC_SetTempValue_Update();

private:
    Ui::airconditioninfo *ui;

};

#endif // AIRCONDITIONINFO_H
