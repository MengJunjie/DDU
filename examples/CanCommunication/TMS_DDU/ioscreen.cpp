#include "ioscreen.h"
#include "ui_ioscreen.h"

#include "../objectdictdata.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

IOScreen::IOScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IOScreen)
{
    ui->setupUi(this);

    mCurCarShow = 1;
    mMC1PageIndex = 0;
    mMC2PageIndex = 10;
    mCurShowIndex = 0;
    mCardNum = 9;
    bMultiPageFlag = true;

    mLabelList1.clear();
    mLabelList2.clear();
    mLabelList3.clear();

    Init();

    timerRefresh = new QTimer();
    connect(timerRefresh, SIGNAL(timeout()), this, SLOT(slot_TimerRefresh_TimeOut()));

    connect(canSignalsSlots, SIGNAL(sig_Car_DIDoorOperation_Update(CAR_ID)), this, SLOT(slot_Car_DIDoorOperation_Update(CAR_ID)));

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));

    timerRefresh->start(400);
}

IOScreen::~IOScreen(){
    delete ui;
}

void IOScreen::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

    mLabelList1<<ui->label_1_1<<ui->label_2_1<<ui->label_3_1<<ui->label_4_1<<ui->label_5_1<<ui->label_6_1<<ui->label_7_1<<ui->label_8_1<<ui->label_9_1<<ui->label_10_1;
    mLabelList1<<ui->label_1_4<<ui->label_2_4<<ui->label_3_4<<ui->label_4_4<<ui->label_5_4<<ui->label_6_4<<ui->label_7_4<<ui->label_8_4<<ui->label_9_4<<ui->label_10_4;
    mLabelList2<<ui->label_1_2<<ui->label_2_2<<ui->label_3_2<<ui->label_4_2<<ui->label_5_2<<ui->label_6_2<<ui->label_7_2<<ui->label_8_2<<ui->label_9_2<<ui->label_10_2;
    mLabelList2<<ui->label_1_5<<ui->label_2_5<<ui->label_3_5<<ui->label_4_5<<ui->label_5_5<<ui->label_6_5<<ui->label_7_5<<ui->label_8_5<<ui->label_9_5<<ui->label_10_5;
    mLabelList3<<ui->label_1_3<<ui->label_2_3<<ui->label_3_3<<ui->label_4_3<<ui->label_5_3<<ui->label_6_3<<ui->label_7_3<<ui->label_8_3<<ui->label_9_3<<ui->label_10_3;
    mLabelList3<<ui->label_1_6<<ui->label_2_6<<ui->label_3_6<<ui->label_4_6<<ui->label_5_6<<ui->label_6_6<<ui->label_7_6<<ui->label_8_6<<ui->label_9_6<<ui->label_10_6;

    mDINameList[0]<<"左侧开门"<<"左侧关门"<<"地线"<<"右侧开门"<<"右侧关门"<<"左侧门再开闭"<<"右侧门再开闭"<<"地线"<<"ATP紧急制动"<<"ATP常用制动";
    mDINameList[0]<<"制动单元故障"<<"停放制动"<<"地线"<<"制动隔离塞门"<<"紧急制动短路"<<"空压机启动"<<"VVVF电源有效"<<"地线"<<"预留"<<"预留";
    mDINameList[1]<<"头继电器"<<"尾继电器"<<"地线"<<"制动指令"<<"紧急制动继电器"<<"ATC工作状态"<<"ATC切除开关"<<"地线"<<"预留"<<"预留";
    mDINameList[1]<<"牵引指令"<<"预留"<<"地线"<<"预留"<<"预留"<<"预留"<<"预留"<<"地线"<<"预留"<<"预留";
    mDINameList[2]<<"预留"<<"预留"<<"地线"<<"门关好继电器"<<"预留"<<"司机室空调制冷故障"<<"司机室空调制冷运行"<<"地线"<<"预留"<<"预留";
    mDINameList[2]<<"预留"<<"紧急疏散门状态"<<"地线"<<"预留"<<"预留"<<"预留"<<"预留"<<"地线"<<"预留"<<"预留";
    mDINameList[3]<<"5km/h"<<"复位"<<"司机钥匙"<<"牵引向前"<<"牵引向后"<<"电制动投入开关"<<"牵引已切除"<<"紧急制动按钮"<<"地线"<<"地线";
    mDINameList[3]<<"紧急制动指令(负)"<<"列车制动已施加"<<"列车缓解不良"<<"强迫缓解按钮"<<"停放制动施加按钮"<<"停放制动缓解按钮"<<"制动缸压力不足"<<"制动缸压力检测"<<"地线"<<"地线";
    mDINameList[4]<<"ATO模式"<<"ATB模式"<<"预留"<<"预留"<<"新旧系统倒接开关"<<"预留"<<"RM模式"<<"零速输出"<<"地线"<<"地线";
    mDINameList[4]<<"紧急制动指令(正)"<<"预留"<<"ATB模式发车按钮"<<"ATO模式发车按钮"<<"ATC重启按钮"<<"预留"<<"警惕按钮"<<"预留"<<"地线"<<"地线";
    mDINameList[5]<<"方向向前指令"<<"方向向后指令"<<"总风欠压继电器"<<"保持制动缓解信号"<<"列车门关好"<<"门关好旁路"<<"门自动模式"<<"门半自动模式"<<"地线"<<"地线";
    mDINameList[5]<<"预留"<<"预留"<<"预留"<<"预留"<<"预留"<<"预留"<<"预留"<<"预留"<<"地线"<<"地线";
    mDINameList[6]<<"BHB"<<"BLB"<<"地线"<<"预留"<<"SIV输出接触器"<<"SIV电源建立"<<"预留"<<"地线"<<"预留"<<"预留";
    mDINameList[6]<<"制动单元故障"<<"停放制动"<<"地线"<<"制动隔离塞门"<<"预留"<<"预留"<<"预留"<<"地线"<<"预留"<<"预留";
    mDINameList[7]<<"BHB"<<"BLB"<<"地线"<<"制动单元故障"<<"停放制动"<<"制动隔离塞门"<<"预留"<<"地线"<<"预留"<<"预留";
    mDINameList[7]<<"预留"<<"预留"<<"地线"<<"预留"<<"预留"<<"VVVF电源有效"<<"预留"<<"地线"<<"预留"<<"预留";
    mDINameList[8]<<"预留"<<"扩展供电"<<"地线"<<"预留"<<"预留"<<"SIV输出接触器"<<"SIV电源建立"<<"地线"<<"预留"<<"预留";
    mDINameList[8]<<"制动单元故障"<<"停放制动"<<"地线"<<"制动隔离塞门"<<"预留"<<"预留"<<"预留"<<"地线"<<"预留"<<"预留";
    mDINameList[9]<<"预留"<<"预留"<<"地线"<<"预留"<<"预留"<<"预留"<<"制动单元故障"<<"地线"<<"预留"<<"预留";
    mDINameList[9]<<"停放制动"<<"制动隔离塞门"<<"地线"<<"预留"<<"预留"<<"预留"<<"预留"<<"地线"<<"预留"<<"预留";

    ui->buttonMC1->setStyleSheet(QSS_PBTN_SELECTED_GREEN);

    setPNPBtnState();
    setLabelHeader();
    setLabelList1();
    setLabelList2();
    setLabelList3();
    setLabelPWM();

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void IOScreen::clearBtnFocus(){
    switch (mCurCarShow) {
    case 1:
        ui->buttonMC1->setStyleSheet(QSS_PBTN_NO_SELECTED);
        break;
    case 2:
        ui->buttonT->setStyleSheet(QSS_PBTN_NO_SELECTED);
        break;
    case 3:
        ui->buttonM->setStyleSheet(QSS_PBTN_NO_SELECTED);
        break;
    case 4:
        ui->buttonT1->setStyleSheet(QSS_PBTN_NO_SELECTED);
        break;
    case 5:
        ui->buttonT2->setStyleSheet(QSS_PBTN_NO_SELECTED);
        break;
    case 6:
        ui->buttonMC2->setStyleSheet(QSS_PBTN_NO_SELECTED);
        break;
    default:
        break;
    }
}

void IOScreen::setLabelHeader(){
    QString strText = "";
    if(1 == mCurCarShow && (mMC1PageIndex >= 3 && mMC1PageIndex <= 5)){
        strText = QString("事件记录仪1-数字量输入信号");
    }
    else if(6 == mCurCarShow && (mMC2PageIndex >= 13 && mMC2PageIndex <= 15)){
        strText = QString("事件记录仪2-数字量输入信号");
    }
    else{
        strText = QString("监控终端%1-数字量输入信号").arg(mCurCarShow);
    }
    ui->label_0_0->setText(strText);
}

void IOScreen::setLabelList1(){
    if(1 == mCurShowIndex || 11 == mCurShowIndex){
        mCardNum = 12;
    }
    else if(2 == mCurShowIndex || 12 == mCurShowIndex){
        mCardNum = 15;
    }
    else if(3 == mCurShowIndex || 13 == mCurShowIndex){
        mCardNum = 7;
    }
    else if(5 == mCurShowIndex || 15 == mCurShowIndex){
        mCardNum = 11;
    }
    else{
        mCardNum = 9;
    }

    for(int i = 0; i < mLabelList1.length(); i++){
        if(i < 10){
            mLabelList1.at(i)->setText(QString("X%1-A%2").arg(mCardNum).arg(i+1));
        }
        else{
            mLabelList1.at(i)->setText(QString("X%1-B%2").arg(mCardNum).arg(i+1-10));
        }
    }
}

void IOScreen::setLabelList2(){
    if(mCurShowIndex < 10){
        for(int i = 0; i < mLabelList2.length(); i++){
            mLabelList2.at(i)->setText(mDINameList[mCurShowIndex].at(i));
        }
    }
    else{
        for(int i = 0; i < mLabelList2.length(); i++){
            mLabelList2.at(i)->setText(mDINameList[mCurShowIndex-10].at(i));
        }
    }
}

void IOScreen::setLabelList3(){
    QString strDIValue[20] = {""};
    int iPWMValue = 0;
    switch (mCurShowIndex) {
    case 0:  // MC1  X9
        strDIValue[0] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[0], 0) ? "1" : "0";
        strDIValue[1] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[0], 1) ? "1" : "0";
        strDIValue[2] = "-";
        strDIValue[3] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[0], 2) ? "1" : "0";
        strDIValue[4] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[0], 3) ? "1" : "0";
        strDIValue[5] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[0], 4) ? "1" : "0";
        strDIValue[6] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[0], 5) ? "1" : "0";
        strDIValue[7] = "-";
        strDIValue[8] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[0], 6) ? "1" : "0";
        strDIValue[9] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[0], 7) ? "1" : "0";
        strDIValue[10] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[1], 0) ? "1" : "0";
        strDIValue[11] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[1], 1) ? "1" : "0";
        strDIValue[12] = "-";
        strDIValue[13] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[1], 2) ? "1" : "0";
        strDIValue[14] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[1], 3) ? "1" : "0";
        strDIValue[15] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[1], 4) ? "1" : "0";
        strDIValue[16] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[1], 5) ? "1" : "0";
        strDIValue[17] = "-";
        strDIValue[18] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[1], 6) ? "1" : "0";
        strDIValue[19] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[1], 7) ? "1" : "0";
        iPWMValue = 0;
        break;
    case 1:  // MC1 X12
        strDIValue[0] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[2], 0) ? "1" : "0";
        strDIValue[1] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[2], 1) ? "1" : "0";
        strDIValue[2] = "-";
        strDIValue[3] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[2], 2) ? "1" : "0";
        strDIValue[4] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[2], 3) ? "1" : "0";
        strDIValue[5] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[2], 4) ? "1" : "0";
        strDIValue[6] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[2], 5) ? "1" : "0";
        strDIValue[7] = "-";
        strDIValue[8] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[2], 6) ? "1" : "0";
        strDIValue[9] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[2], 7) ? "1" : "0";
        strDIValue[10] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[3], 0) ? "1" : "0";
        strDIValue[11] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[3], 1) ? "1" : "0";
        strDIValue[12] = "-";
        strDIValue[13] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[3], 2) ? "1" : "0";
        strDIValue[14] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[3], 3) ? "1" : "0";
        strDIValue[15] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[3], 4) ? "1" : "0";
        strDIValue[16] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[3], 5) ? "1" : "0";
        strDIValue[17] = "-";
        strDIValue[18] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[3], 6) ? "1" : "0";
        strDIValue[19] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[3], 7) ? "1" : "0";
        iPWMValue = 0;
        break;
    case 2:  // MC1 X15
        strDIValue[0] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[4], 0) ? "1" : "0";
        strDIValue[1] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[4], 1) ? "1" : "0";
        strDIValue[2] = "-";
        strDIValue[3] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[4], 2) ? "1" : "0";
        strDIValue[4] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[4], 3) ? "1" : "0";
        strDIValue[5] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[4], 4) ? "1" : "0";
        strDIValue[6] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[4], 5) ? "1" : "0";
        strDIValue[7] = "-";
        strDIValue[8] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[4], 6) ? "1" : "0";
        strDIValue[9] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[4], 7) ? "1" : "0";
        strDIValue[10] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[5], 0) ? "1" : "0";
        strDIValue[11] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[5], 1) ? "1" : "0";
        strDIValue[12] = "-";
        strDIValue[13] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[5], 2) ? "1" : "0";
        strDIValue[14] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[5], 3) ? "1" : "0";
        strDIValue[15] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[5], 4) ? "1" : "0";
        strDIValue[16] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[5], 5) ? "1" : "0";
        strDIValue[17] = "-";
        strDIValue[18] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[5], 6) ? "1" : "0";
        strDIValue[19] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[5], 7) ? "1" : "0";
        iPWMValue = DI_Data_MC1[6];
        break;
    case 3:  // ERM1 X7
        strDIValue[0] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[0], 0) ? "1" : "0";
        strDIValue[1] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[0], 1) ? "1" : "0";
        strDIValue[2] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[0], 2) ? "1" : "0";
        strDIValue[3] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[0], 3) ? "1" : "0";
        strDIValue[4] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[0], 4) ? "1" : "0";
        strDIValue[5] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[0], 5) ? "1" : "0";
        strDIValue[6] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[0], 6) ? "1" : "0";
        strDIValue[7] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[0], 7) ? "1" : "0";
        strDIValue[8] = "-";
        strDIValue[9] = "-";
        strDIValue[10] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[1], 0) ? "1" : "0";
        strDIValue[11] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[1], 1) ? "1" : "0";
        strDIValue[12] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[1], 2) ? "1" : "0";
        strDIValue[13] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[1], 3) ? "1" : "0";
        strDIValue[14] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[1], 4) ? "1" : "0";
        strDIValue[15] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[1], 5) ? "1" : "0";
        strDIValue[16] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[1], 6) ? "1" : "0";
        strDIValue[17] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[1], 7) ? "1" : "0";
        strDIValue[18] = "-";
        strDIValue[19] = "-";
        iPWMValue = 0;
        break;
    case 4:  // ERM1 X9
        strDIValue[0] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[3], 0) ? "1" : "0";
        strDIValue[1] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[3], 1) ? "1" : "0";
        strDIValue[2] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[3], 2) ? "1" : "0";
        strDIValue[3] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[3], 3) ? "1" : "0";
        strDIValue[4] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[3], 4) ? "1" : "0";
        strDIValue[5] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[3], 5) ? "1" : "0";
        strDIValue[6] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[3], 6) ? "1" : "0";
        strDIValue[7] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[3], 7) ? "1" : "0";
        strDIValue[8] = "-";
        strDIValue[9] = "-";
        strDIValue[10] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[4], 0) ? "1" : "0";
        strDIValue[11] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[4], 1) ? "1" : "0";
        strDIValue[12] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[4], 2) ? "1" : "0";
        strDIValue[13] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[4], 3) ? "1" : "0";
        strDIValue[14] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[4], 4) ? "1" : "0";
        strDIValue[15] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[4], 5) ? "1" : "0";
        strDIValue[16] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[4], 6) ? "1" : "0";
        strDIValue[17] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[4], 7) ? "1" : "0";
        strDIValue[18] = "-";
        strDIValue[19] = "-";
        iPWMValue = 0;
        break;
    case 5:  // ERM1 X11
        strDIValue[0] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[5], 0) ? "1" : "0";
        strDIValue[1] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[5], 1) ? "1" : "0";
        strDIValue[2] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[5], 2) ? "1" : "0";
        strDIValue[3] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[5], 3) ? "1" : "0";
        strDIValue[4] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[5], 4) ? "1" : "0";
        strDIValue[5] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[5], 5) ? "1" : "0";
        strDIValue[6] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[5], 6) ? "1" : "0";
        strDIValue[7] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[5], 7) ? "1" : "0";
        strDIValue[8] = "-";
        strDIValue[9] = "-";
        strDIValue[10] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[6], 0) ? "1" : "0";
        strDIValue[11] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[6], 1) ? "1" : "0";
        strDIValue[12] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[6], 2) ? "1" : "0";
        strDIValue[13] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[6], 3) ? "1" : "0";
        strDIValue[14] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[6], 4) ? "1" : "0";
        strDIValue[15] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[6], 5) ? "1" : "0";
        strDIValue[16] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[6], 6) ? "1" : "0";
        strDIValue[17] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM1[6], 7) ? "1" : "0";
        strDIValue[18] = "-";
        strDIValue[19] = "-";
        iPWMValue = DI_Data_ERM1[2];
        break;
    case 6:  // T X9
        strDIValue[0] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T[0], 0) ? "1" : "0";
        strDIValue[1] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T[0], 1) ? "1" : "0";
        strDIValue[2] = "-";
        strDIValue[3] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T[0], 2) ? "1" : "0";
        strDIValue[4] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T[0], 3) ? "1" : "0";
        strDIValue[5] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T[0], 4) ? "1" : "0";
        strDIValue[6] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T[0], 5) ? "1" : "0";
        strDIValue[7] = "-";
        strDIValue[8] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T[0], 6) ? "1" : "0";
        strDIValue[9] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T[0], 7) ? "1" : "0";
        strDIValue[10] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T[1], 0) ? "1" : "0";
        strDIValue[11] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T[1], 1) ? "1" : "0";
        strDIValue[12] = "-";
        strDIValue[13] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T[1], 2) ? "1" : "0";
        strDIValue[14] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T[1], 3) ? "1" : "0";
        strDIValue[15] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T[1], 4) ? "1" : "0";
        strDIValue[16] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T[1], 5) ? "1" : "0";
        strDIValue[17] = "-";
        strDIValue[18] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T[1], 6) ? "1" : "0";
        strDIValue[19] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T[1], 7) ? "1" : "0";
        iPWMValue = 0;
        break;
    case 7:  // M X9
        strDIValue[0] = canSignalsSlots->getCanDataBitFromByte(DI_Data_M[0], 0) ? "1" : "0";
        strDIValue[1] = canSignalsSlots->getCanDataBitFromByte(DI_Data_M[0], 1) ? "1" : "0";
        strDIValue[2] = "-";
        strDIValue[3] = canSignalsSlots->getCanDataBitFromByte(DI_Data_M[0], 2) ? "1" : "0";
        strDIValue[4] = canSignalsSlots->getCanDataBitFromByte(DI_Data_M[0], 3) ? "1" : "0";
        strDIValue[5] = canSignalsSlots->getCanDataBitFromByte(DI_Data_M[0], 4) ? "1" : "0";
        strDIValue[6] = canSignalsSlots->getCanDataBitFromByte(DI_Data_M[0], 5) ? "1" : "0";
        strDIValue[7] = "-";
        strDIValue[8] = canSignalsSlots->getCanDataBitFromByte(DI_Data_M[0], 6) ? "1" : "0";
        strDIValue[9] = canSignalsSlots->getCanDataBitFromByte(DI_Data_M[0], 7) ? "1" : "0";
        strDIValue[10] = canSignalsSlots->getCanDataBitFromByte(DI_Data_M[1], 0) ? "1" : "0";
        strDIValue[11] = canSignalsSlots->getCanDataBitFromByte(DI_Data_M[1], 1) ? "1" : "0";
        strDIValue[12] = "-";
        strDIValue[13] = canSignalsSlots->getCanDataBitFromByte(DI_Data_M[1], 2) ? "1" : "0";
        strDIValue[14] = canSignalsSlots->getCanDataBitFromByte(DI_Data_M[1], 3) ? "1" : "0";
        strDIValue[15] = canSignalsSlots->getCanDataBitFromByte(DI_Data_M[1], 4) ? "1" : "0";
        strDIValue[16] = canSignalsSlots->getCanDataBitFromByte(DI_Data_M[1], 5) ? "1" : "0";
        strDIValue[17] = "-";
        strDIValue[18] = canSignalsSlots->getCanDataBitFromByte(DI_Data_M[1], 6) ? "1" : "0";
        strDIValue[19] = canSignalsSlots->getCanDataBitFromByte(DI_Data_M[1], 7) ? "1" : "0";
        iPWMValue = 0;
        break;
    case 8:  // T1 X9
        strDIValue[0] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T1[0], 0) ? "1" : "0";
        strDIValue[1] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T1[0], 1) ? "1" : "0";
        strDIValue[2] = "-";
        strDIValue[3] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T1[0], 2) ? "1" : "0";
        strDIValue[4] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T1[0], 3) ? "1" : "0";
        strDIValue[5] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T1[0], 4) ? "1" : "0";
        strDIValue[6] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T1[0], 5) ? "1" : "0";
        strDIValue[7] = "-";
        strDIValue[8] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T1[0], 6) ? "1" : "0";
        strDIValue[9] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T1[0], 7) ? "1" : "0";
        strDIValue[10] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T1[1], 0) ? "1" : "0";
        strDIValue[11] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T1[1], 1) ? "1" : "0";
        strDIValue[12] = "-";
        strDIValue[13] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T1[1], 2) ? "1" : "0";
        strDIValue[14] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T1[1], 3) ? "1" : "0";
        strDIValue[15] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T1[1], 4) ? "1" : "0";
        strDIValue[16] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T1[1], 5) ? "1" : "0";
        strDIValue[17] = "-";
        strDIValue[18] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T1[1], 6) ? "1" : "0";
        strDIValue[19] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T1[1], 7) ? "1" : "0";
        iPWMValue = 0;
        break;
    case 9:  // T2 X9
        strDIValue[0] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T2[0], 0) ? "1" : "0";
        strDIValue[1] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T2[0], 1) ? "1" : "0";
        strDIValue[2] = "-";
        strDIValue[3] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T2[0], 2) ? "1" : "0";
        strDIValue[4] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T2[0], 3) ? "1" : "0";
        strDIValue[5] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T2[0], 4) ? "1" : "0";
        strDIValue[6] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T2[0], 5) ? "1" : "0";
        strDIValue[7] = "-";
        strDIValue[8] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T2[0], 6) ? "1" : "0";
        strDIValue[9] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T2[0], 7) ? "1" : "0";
        strDIValue[10] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T2[1], 0) ? "1" : "0";
        strDIValue[11] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T2[1], 1) ? "1" : "0";
        strDIValue[12] = "-";
        strDIValue[13] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T2[1], 2) ? "1" : "0";
        strDIValue[14] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T2[1], 3) ? "1" : "0";
        strDIValue[15] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T2[1], 4) ? "1" : "0";
        strDIValue[16] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T2[1], 5) ? "1" : "0";
        strDIValue[17] = "-";
        strDIValue[18] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T2[1], 6) ? "1" : "0";
        strDIValue[19] = canSignalsSlots->getCanDataBitFromByte(DI_Data_T2[1], 7) ? "1" : "0";
        iPWMValue = 0;
        break;
    case 10:  // MC2 X9
        strDIValue[0] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[0], 0) ? "1" : "0";
        strDIValue[1] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[0], 1) ? "1" : "0";
        strDIValue[2] = "-";
        strDIValue[3] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[0], 2) ? "1" : "0";
        strDIValue[4] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[0], 3) ? "1" : "0";
        strDIValue[5] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[0], 4) ? "1" : "0";
        strDIValue[6] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[0], 5) ? "1" : "0";
        strDIValue[7] = "-";
        strDIValue[8] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[0], 6) ? "1" : "0";
        strDIValue[9] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[0], 7) ? "1" : "0";
        strDIValue[10] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[1], 0) ? "1" : "0";
        strDIValue[11] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[1], 1) ? "1" : "0";
        strDIValue[12] = "-";
        strDIValue[13] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[1], 2) ? "1" : "0";
        strDIValue[14] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[1], 3) ? "1" : "0";
        strDIValue[15] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[1], 4) ? "1" : "0";
        strDIValue[16] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[1], 5) ? "1" : "0";
        strDIValue[17] = "-";
        strDIValue[18] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[1], 6) ? "1" : "0";
        strDIValue[19] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[1], 7) ? "1" : "0";
        iPWMValue = 0;
        break;
    case 11:  // MC2 X12
        strDIValue[0] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[2], 0) ? "1" : "0";
        strDIValue[1] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[2], 1) ? "1" : "0";
        strDIValue[2] = "-";
        strDIValue[3] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[2], 2) ? "1" : "0";
        strDIValue[4] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[2], 3) ? "1" : "0";
        strDIValue[5] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[2], 4) ? "1" : "0";
        strDIValue[6] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[2], 5) ? "1" : "0";
        strDIValue[7] = "-";
        strDIValue[8] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[2], 6) ? "1" : "0";
        strDIValue[9] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[2], 7) ? "1" : "0";
        strDIValue[10] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[3], 0) ? "1" : "0";
        strDIValue[11] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[3], 1) ? "1" : "0";
        strDIValue[12] = "-";
        strDIValue[13] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[3], 2) ? "1" : "0";
        strDIValue[14] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[3], 3) ? "1" : "0";
        strDIValue[15] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[3], 4) ? "1" : "0";
        strDIValue[16] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[3], 5) ? "1" : "0";
        strDIValue[17] = "-";
        strDIValue[18] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[3], 6) ? "1" : "0";
        strDIValue[19] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[3], 7) ? "1" : "0";
        iPWMValue = 0;
        break;
    case 12:  // MC2 X15
        strDIValue[0] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[4], 0) ? "1" : "0";
        strDIValue[1] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[4], 1) ? "1" : "0";
        strDIValue[2] = "-";
        strDIValue[3] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[4], 2) ? "1" : "0";
        strDIValue[4] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[4], 3) ? "1" : "0";
        strDIValue[5] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[4], 4) ? "1" : "0";
        strDIValue[6] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[4], 5) ? "1" : "0";
        strDIValue[7] = "-";
        strDIValue[8] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[4], 6) ? "1" : "0";
        strDIValue[9] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[4], 7) ? "1" : "0";
        strDIValue[10] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[5], 0) ? "1" : "0";
        strDIValue[11] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[5], 1) ? "1" : "0";
        strDIValue[12] = "-";
        strDIValue[13] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[5], 2) ? "1" : "0";
        strDIValue[14] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[5], 3) ? "1" : "0";
        strDIValue[15] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[5], 4) ? "1" : "0";
        strDIValue[16] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[5], 5) ? "1" : "0";
        strDIValue[17] = "-";
        strDIValue[18] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[5], 6) ? "1" : "0";
        strDIValue[19] = canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[5], 7) ? "1" : "0";
        iPWMValue = DI_Data_MC2[6];
        break;
    case 13:  // ERM2 X7
        strDIValue[0] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[0], 0) ? "1" : "0";
        strDIValue[1] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[0], 1) ? "1" : "0";
        strDIValue[2] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[0], 2) ? "1" : "0";
        strDIValue[3] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[0], 3) ? "1" : "0";
        strDIValue[4] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[0], 4) ? "1" : "0";
        strDIValue[5] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[0], 5) ? "1" : "0";
        strDIValue[6] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[0], 6) ? "1" : "0";
        strDIValue[7] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[0], 7) ? "1" : "0";
        strDIValue[8] = "-";
        strDIValue[9] = "-";
        strDIValue[10] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[1], 0) ? "1" : "0";
        strDIValue[11] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[1], 1) ? "1" : "0";
        strDIValue[12] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[1], 2) ? "1" : "0";
        strDIValue[13] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[1], 3) ? "1" : "0";
        strDIValue[14] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[1], 4) ? "1" : "0";
        strDIValue[15] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[1], 5) ? "1" : "0";
        strDIValue[16] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[1], 6) ? "1" : "0";
        strDIValue[17] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[1], 7) ? "1" : "0";
        strDIValue[18] = "-";
        strDIValue[19] = "-";
        iPWMValue = 0;
        break;
    case 14:  // ERM2 X9
        strDIValue[0] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[3], 0) ? "1" : "0";
        strDIValue[1] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[3], 1) ? "1" : "0";
        strDIValue[2] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[3], 2) ? "1" : "0";
        strDIValue[3] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[3], 3) ? "1" : "0";
        strDIValue[4] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[3], 4) ? "1" : "0";
        strDIValue[5] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[3], 5) ? "1" : "0";
        strDIValue[6] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[3], 6) ? "1" : "0";
        strDIValue[7] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[3], 7) ? "1" : "0";
        strDIValue[8] = "-";
        strDIValue[9] = "-";
        strDIValue[10] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[4], 0) ? "1" : "0";
        strDIValue[11] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[4], 1) ? "1" : "0";
        strDIValue[12] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[4], 2) ? "1" : "0";
        strDIValue[13] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[4], 3) ? "1" : "0";
        strDIValue[14] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[4], 4) ? "1" : "0";
        strDIValue[15] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[4], 5) ? "1" : "0";
        strDIValue[16] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[4], 6) ? "1" : "0";
        strDIValue[17] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[4], 7) ? "1" : "0";
        strDIValue[18] = "-";
        strDIValue[19] = "-";
        iPWMValue = 0;
        break;
    case 15:  // ERM2 X11
        strDIValue[0] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[5], 0) ? "1" : "0";
        strDIValue[1] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[5], 1) ? "1" : "0";
        strDIValue[2] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[5], 2) ? "1" : "0";
        strDIValue[3] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[5], 3) ? "1" : "0";
        strDIValue[4] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[5], 4) ? "1" : "0";
        strDIValue[5] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[5], 5) ? "1" : "0";
        strDIValue[6] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[5], 6) ? "1" : "0";
        strDIValue[7] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[5], 7) ? "1" : "0";
        strDIValue[8] = "-";
        strDIValue[9] = "-";
        strDIValue[10] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[6], 0) ? "1" : "0";
        strDIValue[11] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[6], 1) ? "1" : "0";
        strDIValue[12] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[6], 2) ? "1" : "0";
        strDIValue[13] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[6], 3) ? "1" : "0";
        strDIValue[14] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[6], 4) ? "1" : "0";
        strDIValue[15] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[6], 5) ? "1" : "0";
        strDIValue[16] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[6], 6) ? "1" : "0";
        strDIValue[17] = canSignalsSlots->getCanDataBitFromByte(DI_Data_ERM2[6], 7) ? "1" : "0";
        strDIValue[18] = "-";
        strDIValue[19] = "-";
        iPWMValue = DI_Data_ERM2[2];
        break;
    default:
        for(int i = 0; i < 20; i++){
            strDIValue[i] = "";
        }
        iPWMValue = 0;
        break;
    }
    if(mCurShowIndex >=0 && mCurShowIndex <= 15){
        for(int i = 0; i < mLabelList3.length(); i++){
            mLabelList3.at(i)->setMyLabelText(strDIValue[i]);
        }

        ui->label_PWM_2->setText(QString("%1%").arg(iPWMValue));
    }
}

void IOScreen::setLabelPWM(){
    if(2 == mCurShowIndex || 5 == mCurShowIndex || 12 == mCurShowIndex || 15 == mCurShowIndex){
        ui->label_PWM_0->setVisible(true);
        ui->label_PWM_1->setVisible(true);
        ui->label_PWM_2->setVisible(true);
    }
    else{
        ui->label_PWM_0->setVisible(false);
        ui->label_PWM_1->setVisible(false);
        ui->label_PWM_2->setVisible(false);
    }

    if(2 == mCurShowIndex || 12 == mCurShowIndex){
        ui->label_PWM_0->setText(QString("监控终端%1-PWM信号").arg(mCurCarShow));
    }
    else if(5 == mCurShowIndex){
        ui->label_PWM_0->setText(QString("事件记录仪1-PWM信号"));
    }
    else if(15 == mCurShowIndex){
        ui->label_PWM_0->setText(QString("事件记录仪2-PWM信号"));
    }
    else{
        // do nothing
    }
}

void IOScreen::setPNPBtnState(){
    if(1 == mCurCarShow || 6 == mCurCarShow){
        ui->buttonprevious->setText("上一页");
        ui->buttonnext->setText("下一页");
    }
    else{
        ui->buttonprevious->setText("");
        ui->buttonnext->setText("");
    }
}

void IOScreen::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void IOScreen::on_buttonMC1_clicked(){
    if(1 != mCurCarShow){
        clearBtnFocus();
        buttonBeep();
        mCurCarShow = 1;
        ui->buttonMC1->setStyleSheet(QSS_PBTN_SELECTED_GREEN);
        mCurShowIndex = mMC1PageIndex;
        bMultiPageFlag = true;

        setPNPBtnState();
        setLabelHeader();
        setLabelList1();
        setLabelList2();
        setLabelList3();
        setLabelPWM();
    }
}

void IOScreen::on_buttonT_clicked(){
    if(2 != mCurCarShow){
        clearBtnFocus();
        buttonBeep();
        mCurCarShow = 2;
        ui->buttonT->setStyleSheet(QSS_PBTN_SELECTED_GREEN);
        mCurShowIndex = 6;
        bMultiPageFlag = false;

        setPNPBtnState();
        setLabelHeader();
        setLabelList1();
        setLabelList2();
        setLabelList3();
        setLabelPWM();
    }
}

void IOScreen::on_buttonM_clicked(){
    if(3 != mCurCarShow){
        clearBtnFocus();
        buttonBeep();
        mCurCarShow = 3;
        ui->buttonM->setStyleSheet(QSS_PBTN_SELECTED_GREEN);
        mCurShowIndex = 7;
        bMultiPageFlag = false;

        setPNPBtnState();
        setLabelHeader();
        setLabelList1();
        setLabelList2();
        setLabelList3();
        setLabelPWM();
    }
}

void IOScreen::on_buttonT1_clicked(){
    if(4 != mCurCarShow){
        clearBtnFocus();
        buttonBeep();
        mCurCarShow = 4;
        ui->buttonT1->setStyleSheet(QSS_PBTN_SELECTED_GREEN);
        mCurShowIndex = 8;
        bMultiPageFlag = false;

        setPNPBtnState();
        setLabelHeader();
        setLabelList1();
        setLabelList2();
        setLabelList3();
        setLabelPWM();
    }
}

void IOScreen::on_buttonT2_clicked(){
    if(5 != mCurCarShow){
        clearBtnFocus();
        buttonBeep();
        mCurCarShow = 5;
        ui->buttonT2->setStyleSheet(QSS_PBTN_SELECTED_GREEN);
        mCurShowIndex = 9;
        bMultiPageFlag = false;

        setPNPBtnState();
        setLabelHeader();
        setLabelList1();
        setLabelList2();
        setLabelList3();
        setLabelPWM();
    }
}

void IOScreen::on_buttonMC2_clicked(){
    if(6 != mCurCarShow){
        clearBtnFocus();
        buttonBeep();
        mCurCarShow = 6;
        ui->buttonMC2->setStyleSheet(QSS_PBTN_SELECTED_GREEN);
        mCurShowIndex = mMC2PageIndex;
        bMultiPageFlag = true;

        setPNPBtnState();
        setLabelHeader();
        setLabelList1();
        setLabelList2();
        setLabelList3();
        setLabelPWM();
    }
}

void IOScreen::on_buttonprevious_clicked(){
    if(bMultiPageFlag){
        if(1 == mCurCarShow){
            if(0 == mMC1PageIndex){
                mMC1PageIndex = 5;
            }
            else{
                mMC1PageIndex--;
            }
            mCurShowIndex = mMC1PageIndex;
        }
        else{
            if(10 == mMC2PageIndex){
                mMC2PageIndex = 15;
            }
            else{
                mMC2PageIndex--;
            }
            mCurShowIndex = mMC2PageIndex;
        }
        buttonBeep();

        setPNPBtnState();
        setLabelHeader();
        setLabelList1();
        setLabelList2();
        setLabelList3();
        setLabelPWM();
    }
}

void IOScreen::on_buttonnext_clicked(){
    if(bMultiPageFlag){
        if(1 == mCurCarShow){
            if(5 == mMC1PageIndex){
                mMC1PageIndex = 0;
            }
            else{
                mMC1PageIndex++;
            }
            mCurShowIndex = mMC1PageIndex;
        }
        else{
            if(15 == mMC2PageIndex){
                mMC2PageIndex = 10;
            }
            else{
                mMC2PageIndex++;
            }
            mCurShowIndex = mMC2PageIndex;
        }
        buttonBeep();

        setPNPBtnState();
        setLabelHeader();
        setLabelList1();
        setLabelList2();
        setLabelList3();
        setLabelPWM();
    }
}

void IOScreen::on_buttonquit_clicked(){
    if(timerRefresh->isActive()){
        timerRefresh->stop();
    }
    buttonBeep();
    this->reject();
}

void IOScreen::slot_Car_DIDoorOperation_Update(CAR_ID carId){
    if(CAR_1 == carId && 0 == mCurShowIndex){
        ui->label_1_3->setMyLabelText(canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[0], 0) ? "1" : "0");
        ui->label_2_3->setMyLabelText(canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[0], 1) ? "1" : "0");
        ui->label_4_3->setMyLabelText(canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[0], 2) ? "1" : "0");
        ui->label_5_3->setMyLabelText(canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[0], 3) ? "1" : "0");
        ui->label_6_3->setMyLabelText(canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[0], 4) ? "1" : "0");
        ui->label_7_3->setMyLabelText(canSignalsSlots->getCanDataBitFromByte(DI_Data_MC1[0], 5) ? "1" : "0");
    }
    else if(CAR_6 == carId && 10 == mCurShowIndex){
        ui->label_1_3->setMyLabelText(canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[0], 0) ? "1" : "0");
        ui->label_2_3->setMyLabelText(canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[0], 1) ? "1" : "0");
        ui->label_4_3->setMyLabelText(canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[0], 2) ? "1" : "0");
        ui->label_5_3->setMyLabelText(canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[0], 3) ? "1" : "0");
        ui->label_6_3->setMyLabelText(canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[0], 4) ? "1" : "0");
        ui->label_7_3->setMyLabelText(canSignalsSlots->getCanDataBitFromByte(DI_Data_MC2[0], 5) ? "1" : "0");
    }
    else{
        // do nothing
    }
}

void IOScreen::slot_TimerRefresh_TimeOut(){
    setLabelList3();
}
