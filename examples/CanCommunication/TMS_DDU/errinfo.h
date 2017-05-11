#ifndef ERRINFO_H
#define ERRINFO_H

#include <QDialog>
#include <QTimer>

#include "gui_config.h"

#define TIMER_ERROR_AUTO_BACK_5S 5000

namespace Ui {
class errinfo;
}

enum ERROR_ID{
    NONE_ERR,                   //无故障
    VVVF1_ERR,                  //VVVF1故障
    VVVF2_ERR,                  //VVVF2故障
    SIV_ERR,                      //SIV故障
    BRAKE_ERR,                //制动装置故障
    AIR_ERR,                     //空调装置故障
    RIGHT_DOOR_ERR,    //右侧门故障
    LEFT_DOOR_ERR        //左侧门故障
};

enum DOOR_ERR{
    ERR_DOOR_RUN_NORMAL,                 //正常运行
    ERR_DOOR_COMN_ERR,                      //通讯异常
    ERR_DOOR_URGENT_OPERATION,     //紧急装置操作
    ERR_DOOR_BREAKDOWN                   //发生故障
};

struct struct_VVVFErr{
    int carId;
    int iVVVFIndex;
    int iErrCode;
};

struct struct_SIVErr{
    int carId;
    int iErrCode;
};

struct struct_BCUErr{
    int carId;
    int iErrCode;
};

struct struct_ACCErr{
    int carId;
    int iErrCode;
};

struct struct_DOORErr{
    int carId;
    int doorId;
    int iErrCode;
};

class errinfo : public QDialog
{
    Q_OBJECT
    
public:
    explicit errinfo(QWidget *parent = 0);
    ~errinfo();

    void Init();
    void setDoorName();
    void setVoltage();
    void setCurrent();
    void setBrakeLevel();
    void setSpeed();
    void setErrorHappenInfo();
    void setErrorResetInfo();
    void setVVVFError(ERROR_ID type, CAR_ID carId, DRAG_ELEC_BRAKE_STATE level, int iValue);
    void setSIVError(CAR_ID carId, SIV_STATE iState, int iValue);
    void setBCUError(CAR_ID carId, BC_KPA_STATE iState, int iValue);
    void setACCError(CAR_ID carId, INSIDE_TEMP_STATE iState, int iValue);
    void setDoor_ERR(ERROR_ID errId, CAR_ID carId, DOOR_ID doorId, DOOR_ERR errType);

    void showErrorScreen();

signals:
    void sig_Error_Happened();
    void sig_Error_OKBtn_Clicked();

public slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonok_clicked();
    void slot_5S_Timer_Out();
    void slot_MyLabel_VVVF_clicked();
    void slot_MyLabel_SIV_clicked();
    void slot_MyLabel_BCU_clicked();
    void slot_MyLabel_ACC_clicked();
    void slot_MyLabel_DOOR_clicked();

    void slot_CarIcon_Update();
    void slot_CarCode_Update();
    void slot_Car_Direction_Update();
    void slot_VVVF_BCU_Level_Update(int iState, int iValue);
    void slot_VVVF_Elec_BCU_Update(CAR_ID carId, int vvvfIndex, int iState, int iValue);
    void slot_SIV_Voltage_Freq_Update(CAR_ID carId, int iState, int iValue1, int iValue2);
    void slot_BCU_BC_Kpa_Update(CAR_ID carId, int iState, int iValue);
    void slot_ACC_Temp_Update(CAR_ID carId, int iState, int iValue1, int iValue2);
    void slot_Door_State_Update(CAR_ID carId, DOOR_ID doorId, RUN_DOOR_STATE doorState);

private:
    Ui::errinfo *ui;
    QTimer *  timer_5s;

    bool bErrorHappenedFlag;

    QList<struct_VVVFErr>     m_VVVFErrList;
    QList<struct_SIVErr>        m_SIVErrList;
    QList<struct_BCUErr>      m_BCUErrList;
    QList<struct_ACCErr>       m_ACCErrList;
    QList<struct_DOORErr>   m_DOORErrList;

    int m_VVVF_BCU_State, m_VVVF_BCU_Value;
    int iCurrentFaltCount;
};

#endif // ERRINFO_H
