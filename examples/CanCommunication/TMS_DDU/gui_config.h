#ifndef GUI_CONFIG
#define GUI_CONFIG

#include <stdio.h>
#include <stdlib.h>
#include <QObject>
#include <QtDebug>
#include <QLabel>
#include <QTimer>
#include <QList>
#include <QFile>

#define NODEID_CCU1      0x01
#define NODEID_CCU2      0x02
#define NODEID_MDU1    0x03
#define NODEID_MDU2    0x04
#define NODEID_MDU3    0x05
#define NODEID_MDU4    0x06
#define NODEID_MDU5    0x07
#define NODEID_MDU6    0x08
#define NODEID_HMI1     0x09
#define NODEID_HMI2     0x0A
#define NODEID_ERM1     0x0B
#define NODEID_ERM2     0x0C

enum CAR_HEADER{
    CAR_NO_HEADER,
    CAR_1_HEADER,
    CAR_6_HEADER
};

enum CAN_STATE{
    CAN_PREOPERATIONAL,
    CAN_OPERATIONAL
};

enum CAN_WORKING{
    CAN_NONE,
    CAN_1,
    CAN_2
};

enum SIV_TX_DIRECTION{
    SIV_NONE,
    SIV_2_TO_4,
    SIV_4_TO_2
};

struct structFaultRecordData{
    int iCarId;                                //车号
    int iErrFlag;                             //故障发生或者消除标记  0：默认，1：发生，2：消除
    int iErrId;                                //故障序号
    QString strDate;                     //故障发生日期
    QString strTime;                    //故障发生时间
    int iErrStationCode;              //故障发生上一站ID
    int iErrDistance;                    //故障发生位置里上一站距离
    QString strErrDevName;      //故障设备编号
    QString strErrCode;              //故障代码
    int iTracBrakeLevel;              //牵引制动级位  //0:默认,1:通讯异常,2:紧急制动,3:制动位,4:惰性位,5:牵引位s;
    int iPWMValue;                     //牵引制动PWM值
    int iSpeedValue;                   //速度值
    int iVoltage;                           //网压
};

struct structSubDevErr{
    int iErrId;                       //故障序号
    QString strErrCode;     //故障代码
    QString strDate;           //故障发生日期
    QString strTime;          //故障发生时间
};

struct structSpeedCheck{
    QString strDate;          //日期
    QString strTime;         //时间
    int iCheckType;           //0：默认; 1：加速度1; 2：加速度2; 3：减速度;
    int iSpeedValue;         //加减速度值
    int iPWMValue;          //牵引制动级位PWM值
    int iRebornFlag;         //减速过程中任意VVVF电制动位出现1即为有，加速过程不记录再生 0：默认，1：无，2：有
    int iBrakeSpeed;        //制动初速度
    int iBrakeDistance;    //制动距离
};

struct structRunDataInfo{
    int iErmComnState;                              //ERM设备在当前CAN网络通讯状态，0：通讯正常，1：通讯异常；
    QString strRunDataDate;                     //运行数据开始日期
    QString strRunDataTime;                    //运行数据开始时间
    u_int16_t u16RunData;                        //运行数据;
    QString strWalkDataDate;                   //走行数据开始日期
    QString strWalkDataTime;                  //走行数据开始时间
    u_int16_t u16WalkData;                      //走行数据;
    QString strLoadRateDataDate;           //乘车率数据开始日期
    QString strLoadRateDataTime;          //乘车率数据开始时间
    u_int16_t u16LoadRateData;              //乘车率数据;
    QString strDistanceDataDate;           //行车距离数据开始日期
    QString strDistanceDataTime;          //行车距离数据开始时间
    u_int32_t u32DistanceData;              //行车距离数据;
    QString strWHMPDataDate;             //牵引电力数据开始日期
    QString strWHMPDataTime;             //牵引电力数据开始时间
    u_int32_t u32WHMPData;                 //牵引电力数据;
    QString strWHMBDataDate;             //再生电力数据开始日期
    QString strWHMBDataTime;            //再生电力数据开始时间
    u_int32_t u32WHMBData;                //再生电力数据;
    QString strACPWorkDataDate;         //空压机工作率数据开始日期
    QString strACPWorkDataTime;        //空压机工作率数据开始时间
    u_int32_t u32ACPWorkData;            //空压机工作率数据;
};

extern int deviceCarId;
extern bool nowCtrlHMIFlag;
extern bool masterHMIFlag;
extern bool atoModelFlag;
extern int carTempValue;
extern int carVoltageValue[2];
extern int carCurrentValue[2];
extern int carSpeedValue[2];
extern int carCodeNum[6];
extern CAR_HEADER carHeaderFlag;
extern CAR_HEADER lastCarHeaderFlag;
extern bool can1OperationalFlag;
extern bool can2OperationalFlag;
extern CAN_WORKING canWorkFlag;
extern bool bCCUJudgeFlag;
//extern int mCCUWorkFlag;  //0：无CCU工作，1：CCU1工作，2：CCU2工作；
extern bool bCan1HMI1HeartFlag;   //Can1网络HMI2监听HMI1心跳
extern bool bCan2HMI1HeartFlag;   //Can2网络HMI2监听HMI1心跳
extern bool bPTUDownloading;       //PTU下载故障跟踪数据标志

extern QString updateDateTime();
extern void buttonBeep();

enum CAR_ID{   //列车编号
    CAR_NULL,
    CAR_1,
    CAR_2,
    CAR_3,
    CAR_4,
    CAR_5,
    CAR_6
};

enum DOOR_ID{   //列车车门编号
    DOOR_NULL,
    DOOR_1,
    DOOR_2,
    DOOR_3,
    DOOR_4,
    DOOR_5,
    DOOR_6,
    DOOR_7,
    DOOR_8
};

enum ATO_MODEL{
    ATO_POWER_OFF,  //ATO电源异常
    ATC_CUT_OFF,        //ATC切除
    ATO_COMN_ERR,   //ATO通信异常
    ATO_INVALID,        //非ATO模式
    ATO_VALID             //ATO模式
};

enum INSIDE_TEMP_STATE{
    TEMP_COMO_ERR,              //通讯异常
    TEMP_BREAKDOWN,          //机组故障
    TEMP_RUN_NORMAL         //正常运行
};

enum TRAIN_VOLTAGE{         //列车网压
    VOLTAGE_COMN_ERR,
    VOLTAGE_LESS_THAN_500,
    VOLTAGE_BETWEEN_500_900,
    VOLTAGE_MORE_THAN_900
};

enum TRAIN_CURRENT{       //列车网流
    CURRENT_COMN_ERR,
    CURRENT_NORMAL
};

enum BRAKE_LEVEL_STATE{     //牵引制动级位
    BRAKE_COMN_ERR,              //通讯异常
    BRAKE_URGENT_BRAKE,      //紧急制动位
    BRAKE_BRAKE,                      //制动位
    BRAKE_LAZY,                         //惰性位
    BRAKE_DRAG                        //牵引位
};

enum SPEED_STATE{
    SPEED_COMN_ERR_POWER_OFF,
    SPEED_NORMAL
};

enum DRAG_ELEC_BRAKE_TYPE{
    DRAG_ELEC_BRAKE_1,
    DRAG_ELEC_BRAKE_2
};

enum SIV_STATE{
    SIV_COMN_ERR,
    SIV_POWER_OFF,
    SIV_BREAKDOWN,
    SIV_NORMAL
};

enum DRAG_ELEC_BRAKE_STATE{     //牵引电制动
    ELEC_BRAKE_COMN_ERR,              //通讯异常
    ELEC_BRAKE_POWER_OFF,            //电源OFF
    ELEC_BRAKE_BREAKDOWN,          //故障
    ELEC_BRAKE_LAZY,                         //惰性
    ELEC_BRAKE_DRAG,                       //牵引
    ELEC_BRAKE_BRAKE,                     //制动
    ELEC_BRAKE_CUT_OFF,                 //切除
    ELEC_BRAKE_URGENT_BRAKE      //紧急制动状态
};

enum RUN_DOOR_STATE{                   //门状态
    RUN_DOOR_COMN_ERR,                 //通讯异常
    RUN_DOOR_ISOLATE,                      //隔离
    RUN_DOOR_URGENT_ACTION,       //紧急装置动作
    RUN_DOOR_BREAKDOWN,             //故障
    RUN_DOOR_PREVENT_SQUEEZE,   //门开关防挤压
    RUN_DOOR_OPENED,                     //门开状态
    RUN_DOOR_OPENING_CLOSING,  //开门或关门动作中
    RUN_DOOR_CLOSED                       //门关闭状态
};

enum BC_KPA_STATE{          //BC压力
    BC_COMN_ERR,                //通讯异常
    BC_POWER_OFF,              //电源OFF
    BC_BREAKDOWN,            //故障
    BC_PARK_BRAKE,             //停车制动
    BC_NORMAL                    //正常运行
};

enum ERRRECOED_ITEM{
    ERR_RECOED,
    ERR_VVVF1,
    ERR_VVVF2,
    ERR_SIV,
    ERR_BRAKE
};

#define MAINMENU_BTN_CLICKED   1
#define ACC_BTN_CLICKED                 2
#define CARINFO_BTN_CLICKED        3
#define ERROR_BTN_CLICKED            4
#define KEEPMOVE_BTN_CLICKED     5

#define NO_QPIXMAP    QPixmap("")

#define ARROW_LEFT     QPixmap(":/new/prefix1/png/Arrow_left.png")
#define MC1_CAR_BMP  QPixmap(":/new/prefix1/png/MC1_CAR.png")
#define MC2_CAR_BMP  QPixmap(":/new/prefix1/png/MC2_CAR.png")
#define T_CAR_BMP        QPixmap(":/new/prefix1/png/T_CAR.png")
#define M_CAR_BMP       QPixmap(":/new/prefix1/png/M_CAR.png")

#define ARROW_LEFT_1   QPixmap(":/new/prefix1/png/Arrow_left1.png")

#define SIV_LEFT_BMP        QPixmap(":/new/prefix1/bmp/sivLeft.bmp")
#define SIV_RIGHT_BMP     QPixmap(":/new/prefix1/bmp/sivRight.bmp")

#define CAR_TYPE  "TQ"

#define CAR_1_ID    "1"
#define CAR_2_ID    "2"
#define CAR_3_ID    "3"
#define CAR_4_ID    "4"
#define CAR_5_ID    "5"
#define CAR_6_ID    "6"

#define CALLBACK_DELAY_TIME                 200*1000
#define SEND_PDO_CYCLE_200MS             0xC8
#define SEND_PDO_CYCLE_1S                     0x3E8
#define SEND_PDO_CYCLE_30S                   0x7530
#define TIME_UPDATE                                 1000
#define CCU_JUDGE_TIME_OUT                  1000
#define HMI_REFRESH_TIME_OUT              500
#define SYS_TIME_UPDATE                          2*60*1000
#define GUI_SHOW_TIMEOUT                    60
#define HMI_MASTER_TIMER_OUT             2500

#define KMH_TO_MS_UNIT_VALUE             0.28        //1Km/h = 0.28m/s

#define SPEED_UNIT_VALUE                           0.5      //速度最小单位  1Bit = 0.5Km/h
#define VOLTAGE_UNIT_VALUE                       4.9     //电压最小单位  1Bit = 4.9V
#define CURRENT_UNIT_VALUE_GT                15.6      //电流最小单位  1Bit = 15.6A
//#define CURRENT_UNIT_VALUE_LT                 -16     //电流最小单位  1Bit = 16A
#define VVVF_ELE_CURRENT_UNIT_VALUE     2.5    //牵引电制动电流最小单位值   1Bit = 2.5A
#define SIV_VOLTAGE_UNIT_VALUE                2.3    //SIV输出电压最小单位值   1Bit = 2.3V
#define SIV_FREQ_UNIT_VALUE                       0.5    //SIV输出频率最小单位值   1Bit = 0.5Hz
#define BCU_BC_KPA_UNIT_VALUE                 4       //BCU协议中BC压力最小单位值   1Bit = 4Kpa
#define BCU_AS_KPA_UNIT_VALUE                 4       //BCU协议中AS压力最小单位值   1Bit = 4Kpa
#define BCU_PWM_UNIT_VALUE                     0.4    //BCU协议中PWM最小单位值   1Bit = 0.4
#define VVVF_PWM_UNIT_VALUE                    0.5    //VVVF协议中PWM最小单位值   1Bit = 0.5
#define TOTAL_WEIGHT_UNIT_VALUE             0.2    //BCU协议中列车重量最小单位值   1Bit = 0.2

// 线条样式：棕色，紫色，白色
#define QSS_LINE_COLOR_BROWN  "border:2px solid; border-color:rgb(119, 47, 40); background-color:rgb(119, 47, 40);"
#define QSS_LINE_COLOR_PURPLE  "border:2px solid; border-color:rgb(204, 0, 153); background-color:rgb(204, 0, 153);"
#define QSS_LINE_COLOR_WHITE    "border:2px solid; border-color:rgb(245, 245, 245); background-color:rgb(245, 245, 245);"

//  文本样式：白底白边红字，白底无边红字
#define QSS_BG_WHITE_BR_WHITE_WD_RED   "border:1px solid; border-color:rgb(245, 245, 245); background-color:rgb(245, 245, 245); color:rgb(255, 0, 0);"
#define QSS_BG_WHITE_BR_NONE_WD_RED    "border:0px solid; background-color:rgb(245, 245, 245); color:rgb(255, 0, 0);"

//  文本样式：黑底白边白字，黑底绿边白字，黑底黑边白字，黑底无边白字
#define QSS_BG_BLACK_BR_WHITE_WD_WHITE   "border:1px solid; border-color:rgb(245, 245, 245); background-color:rgb(0, 0, 0); color:rgb(245, 245, 245);"
#define QSS_BG_BLACK_BR_GREEN_WD_WHITE   "border:1px solid; border-color:rgb(0, 255, 0); background-color:rgb(0, 0, 0); color:rgb(245, 245, 245);"
#define QSS_BG_BLACK_BR_BLACK_WD_WHITE    "border:1px solid; border-color:rgb(0, 0, 0); background-color:rgb(0, 0, 0); color:rgb(245, 245, 245);"
#define QSS_BG_BLACK_BR_NONE_WD_WHITE     "border:1px solid; border-color:rgb(0, 0, 0); background-color:rgb(0, 0, 0); color:rgb(245, 245, 245);"

//  文本样式：绿底白边黑字，绿底白边白字，绿底绿边黑字，绿底无边黑字
#define QSS_BG_GREEN_BR_WHITE_WD_BLACK   "border:1px solid; border-color:rgb(245, 245, 245); background-color:rgb(0, 255, 0); color:rgb(0, 0, 0);"
#define QSS_BG_GREEN_BR_WHITE_WD_WHITE   "border:1px solid; border-color:rgb(245, 245, 245); background-color:rgb(0, 255, 0); color:rgb(245, 245, 245);"
#define QSS_BG_GREEN_BR_GREEN_WD_BLACK   "border:1px solid; border-color:rgb(0, 255, 0); background-color:rgb(0, 255, 0); color:rgb(0, 0, 0);"
#define QSS_BG_GREEN_BR_NONE_WD_BLACK    "border:0px solid; background-color:rgb(0, 255, 0); color:rgb(0, 0, 0);"

//  文本样式：深绿底白边白字，深绿底深绿边黑字
#define QSS_BG_DARK_GREEN_BR_WHITE_WD_WHITE   "border:1px solid; border-color:rgb(245, 245, 245); background-color:rgb(0, 176, 80); color:rgb(245, 245, 245);"
#define QSS_BG_BR_DARK_GREEN_WD_BLACK                "border:1px solid; border-color:rgb(0, 176, 80); background-color:rgb(0, 176, 80); color:rgb(0, 0, 0);"

//  文本样式：红底白边白字，红底红边白字，红底无边白字
#define QSS_BG_RED_BR_WHITE_WD_WHITE        "border:1px solid; border-color:rgb(245, 245, 245); background-color:rgb(255, 0, 0); color:rgb(245, 245, 245);"
#define QSS_BG_RED_BR_RED_WD_WHITE            "border:1px solid; border-color:rgb(255, 0, 0); background-color:rgb(255, 0, 0); color:rgb(245, 245, 245);"
#define QSS_BG_RED_BR_NONE_WD_WHITE        "border:0px solid; background-color:rgb(255, 0, 0); color:rgb(245, 245, 245);"

//  文本样式：深红底白边白字
#define QSS_BG_DARK_RED_BR_WHITE_WD_WHITE   "border:1px solid; border-color:rgb(245, 245, 245); background-color:rgb(128, 0, 0); color:rgb(245, 245, 245);"

//  文本样式：蓝底白边白字，蓝底白边黑字，蓝底蓝边黑字，蓝底无边白字
#define QSS_BG_BLUE_BR_WHITE_WD_WHITE       "border:1px solid; border-color:rgb(245, 245, 245); background-color:rgb(0, 112, 192); color:rgb(245, 245, 245);"
#define QSS_BG_BLUE_BR_WHITE_WD_BLACK       "border:1px solid; border-color:rgb(245, 245, 245); background-color:rgb(0, 112, 192); color:rgb(0, 0, 0);"
#define QSS_BG_BLUE_BR_BLUE_WD_BLACK         "border:1px solid; border-color:rgb(0, 112, 192); background-color:rgb(0, 112, 192); color:rgb(0, 0, 0);"
#define QSS_BG_BLUE_BR_NONE_WD_WHITE       "border:0px solid; background-color:rgb(0, 112, 192); color:rgb(245, 245, 245);"

//  文本样式：黄底白边黑字，黄底白边白字，黄底黄边黑字，黄底无边黑字
#define QSS_BG_YELLOW_BR_WHITE_WD_BLACK     "border:1px solid; border-color:rgb(245, 245, 245); background-color:rgb(255, 255, 0); color:rgb(0, 0, 0);"
#define QSS_BG_YELLOW_BR_WHITE_WD_WHITE     "border:1px solid; border-color:rgb(245, 245, 245); background-color:rgb(255, 255, 0); color:rgb(245, 245, 245);"
#define QSS_BG_YELLOW_BR_YELLOW_WD_BLACK   "border:1px solid; border-color:rgb(255, 255, 0); background-color:rgb(255, 255, 0); color:rgb(0, 0, 0);"
#define QSS_BG_YELLOW_BR_NONE_WD_BLACK      "border:0px solid; background-color:rgb(255, 255, 0); color:rgb(0, 0, 0);"

//  文本样式：紫底白边白字，紫底紫边白字，紫底紫边黑字，紫底无边白字
#define QSS_BG_PURPLE_BR_WHITE_WD_WHITE     "border:1px solid; border-color:rgb(245, 245, 245); background-color:rgb(204, 0, 153); color:rgb(245, 245, 245);"
#define QSS_BG_PURPLE_BR_PURPLE_WD_WHITE   "border:1px solid; border-color:rgb(204, 0, 153); background-color:rgb(204, 0, 153); color:rgb(245, 245, 245);"
#define QSS_BG_PURPLE_BR_PURPLE_WD_BLACK   "border:1px solid; border-color:rgb(204, 0, 153); background-color:rgb(204, 0, 153); color:rgb(0, 0, 0);"
#define QSS_BG_PURPLE_BR_NONE_WD_WHITE     "border:0px solid; background-color:rgb(204, 0, 153); color:rgb(245, 245, 245);"

//  文本样式：棕底白边黑字，棕底白边白字
#define QSS_BG_BROWN_BR_WHITE_WD_BLACK     "border:1px solid; border-color:rgb(245, 245, 245); background-color:rgb(217, 104, 13); color:rgb(0, 0, 0);"
#define QSS_BG_BROWN_BR_WHITE_WD_WHITE     "border:1px solid; border-color:rgb(245, 245, 245); background-color:rgb(217, 104, 13); color:rgb(245, 245, 245);"

//  文本样式：深棕底深棕边黑字
#define QSS_BG_BR_DARK_BROWN_WD_BLACK       "border:1px solid; border-color:rgb(119, 47, 40); background-color:rgb(119, 47, 40); color:rgb(0, 0, 0);"

//  文本样式：灰底白边黑字，灰底无边黑字
#define QSS_BG_GREY_BR_WHITE_WD_BLACK         "border:1px solid; border-color:rgb(245, 245, 245); background-color:rgb(166, 166, 166); color:rgb(0, 0, 0);"
#define QSS_BG_GREY_BR_NONE_WD_BLACK         "border:0px solid; background-color:rgb(166, 166, 166); color:rgb(0, 0, 0);"

//  文本样式：橙底白边白字，橙底白无边白字
#define QSS_BG_ORANGE_BR_WHITE_WD_WHITE   "border:1px solid; border-color:rgb(245, 245, 245); background-color:rgb(242, 130, 38); color:rgb(245, 245, 245);"
#define QSS_BG_ORANGE_BR_NONE_WD_WHITE   "border:0px solid; background-color:rgb(242, 130, 38); color:rgb(245, 245, 245);"

//  文本样式：橙底白边白字，橙底白无边白字
#define QSS_PBTN_SELECTED_GREEN      "background-color:rgb(0, 255, 0);"
#define QSS_PBTN_SELECTED_GREY         "background-color:rgb(140, 140, 140);"
#define QSS_PBTN_NO_SELECTED            "background-color:rgb(217, 217, 217);"

#endif // GUI_CONFIG

