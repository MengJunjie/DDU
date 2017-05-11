#include "airconditioninfo.h"
#include "ui_airconditioninfo.h"
#include "errinfo.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

airconditioninfo::airconditioninfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::airconditioninfo)
{
    ui->setupUi(this);

    Init();
    slot_CarIcon_Update();
    slot_CarCode_Update();

    connect(canSignalsSlots, SIGNAL(sig_Car_Direction_Change()), this, SLOT(slot_Car_Direction_Update()));
    connect(canSignalsSlots, SIGNAL(sig_Car_Code_Change()), this, SLOT(slot_CarCode_Update()));
    connect(canSignalsSlots, SIGNAL(sig_ACC_Temp_Update(CAR_ID,int,int,int)), this, SLOT(slot_ACC_Temp_Update(CAR_ID,int,int,int)));
    connect(canSignalsSlots, SIGNAL(sig_ACCData_Change(CAR_ID,int,int,int,int,int)), this, SLOT(slot_ACCData_Change(CAR_ID,int,int,int,int,int)));
    connect(canSignalsSlots, SIGNAL(sig_ACCCtrlMode(int)), this, SLOT(slot_ACCCtrlMode(int)));
    connect(canSignalsSlots, SIGNAL(sig_ACC_SetTempValue_Update()), this, SLOT(slot_ACC_SetTempValue_Update()));

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));
}

airconditioninfo::~airconditioninfo(){
    delete ui;
}

void airconditioninfo::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

//    ui->labelarrow->setScaledContents(true);
    ui->carimagelabel1->setScaledContents(true);
    ui->carimagelabel2->setScaledContents(true);
    ui->carimagelabel3->setScaledContents(true);
    ui->carimagelabel4->setScaledContents(true);
    ui->carimagelabel5->setScaledContents(true);
    ui->carimagelabel6->setScaledContents(true);

    ui->labelacctemp->setText(QString("%1℃").arg(carTempValue));
    ui->label_2->setVisible(false);
    ui->labelacctemp->setVisible(false);
    ui->buttonTempUp->setVisible(false);
    ui->buttonTempDown->setVisible(false);

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void airconditioninfo::setTEMP(CAR_ID carId, INSIDE_TEMP_STATE state, int tempValue, int /*errCode*/){
    QLabel * tempLabel = NULL;
    switch (carId) {
    case CAR_1:
        tempLabel = ui->labelinfo_2_2;
        break;
    case CAR_2:
        tempLabel = ui->labelinfo_2_3;
        break;
    case CAR_3:
        tempLabel = ui->labelinfo_2_4;
        break;
    case CAR_4:
        tempLabel = ui->labelinfo_2_5;
        break;
    case CAR_5:
        tempLabel = ui->labelinfo_2_6;
        break;
    case CAR_6:
        tempLabel = ui->labelinfo_2_7;
        break;
    default:
        tempLabel = NULL;
        break;
    }
    if(NULL == tempLabel){
        return ;
    }
    QString strText = "", strStyleSheet = "";
    switch (state) {
    case TEMP_COMO_ERR:
        strText = "";
        strStyleSheet = QSS_BG_PURPLE_BR_WHITE_WD_WHITE;
        break;
    case TEMP_BREAKDOWN:
        strText = QString("%1℃").arg(tempValue);
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
//        strText = canSignalsSlots->getACCTempErrorInfo(tempValue);
//        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
        break;
    case TEMP_RUN_NORMAL:
        strText = QString("%1℃").arg(tempValue);
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        break;
    default:
        strText = "";
        strStyleSheet = "";
        break;
    }
    tempLabel->setText(strText);
    tempLabel->setStyleSheet(strStyleSheet);
}

void airconditioninfo::setAirconditionState(AIRCONDITIONSTATE_ITEM item, CAR_ID carId, int iState, MACHINE_STATE state){
    QLabel * tempLabel = NULL;
    switch (item) {
    case MACHINE_1_STATE:
        switch (carId) {
        case CAR_1:
            tempLabel = ui->labelinfo_3_2;
            break;
        case CAR_2:
            tempLabel = ui->labelinfo_3_3;
            break;
        case CAR_3:
            tempLabel = ui->labelinfo_3_4;
            break;
        case CAR_4:
            tempLabel = ui->labelinfo_3_5;
            break;
        case CAR_5:
            tempLabel = ui->labelinfo_3_6;
            break;
        case CAR_6:
            tempLabel = ui->labelinfo_3_7;
            break;
        default:
            tempLabel = NULL;
            break;
        }
        break;
    case MACHINE_2_STATE:
        switch (carId) {
        case CAR_1:
            tempLabel = ui->labelinfo_4_2;
            break;
        case CAR_2:
            tempLabel = ui->labelinfo_4_3;
            break;
        case CAR_3:
            tempLabel = ui->labelinfo_4_4;
            break;
        case CAR_4:
            tempLabel = ui->labelinfo_4_5;
            break;
        case CAR_5:
            tempLabel = ui->labelinfo_4_6;
            break;
        case CAR_6:
            tempLabel = ui->labelinfo_4_7;
            break;
        default:
            tempLabel = NULL;
            break;
        }
        break;
    default:
        tempLabel = NULL;
        break;
    }
    if(NULL == tempLabel){
        return ;
    }
    QString strText = "", strStyleSheet = "";
    if(0 == iState){
        strText = "";
        strStyleSheet = QSS_BG_PURPLE_BR_WHITE_WD_WHITE;
    }
    else{
        switch (state) {
//        case TEMP_COMO_ERR:
//            strText = "";
//            break;
        case AIR_AUTO:
            strText = "自动";
            break;
        case AIR_AERATION:
            strText = "通风";
            break;
        case AIR_WORK_STOP:
            strText = "停机";
            break;
        case AIR_HALF_COLD:
            strText = "弱冷";
            break;
        case AIR_DEEP_COLD:
            strText = "强冷";
            break;
        case AIR_EMR_VENT:
            strText = "通风";
            break;
        case AIR_HALF_HOT:
            strText = "半暖";
            break;
        case AIR_ALL_HOT:
            strText = "全暖";
            break;
        default:
            strText = "";
            break;
        }
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
    }
    tempLabel->setText(strText);
    tempLabel->setStyleSheet(strStyleSheet);
}

void airconditioninfo::setAirconditionError(AIRCONDITIONSTATE_ITEM item, CAR_ID carId, int iState, int errCode){
    QLabel * tempLabel = NULL;
    switch (item) {
    case MACHINE_1_BREAKDOWN:
        switch (carId) {
        case CAR_1:
            tempLabel = ui->labelinfo_5_2;
            break;
        case CAR_2:
            tempLabel = ui->labelinfo_5_3;
            break;
        case CAR_3:
            tempLabel = ui->labelinfo_5_4;
            break;
        case CAR_4:
            tempLabel = ui->labelinfo_5_5;
            break;
        case CAR_5:
            tempLabel = ui->labelinfo_5_6;
            break;
        case CAR_6:
            tempLabel = ui->labelinfo_5_7;
            break;
        default:
            tempLabel = NULL;
            break;
        }
        break;
    case MACHINE_2_BREAKDOWN:
        switch (carId) {
        case CAR_1:
            tempLabel = ui->labelinfo_6_2;
            break;
        case CAR_2:
            tempLabel = ui->labelinfo_6_3;
            break;
        case CAR_3:
            tempLabel = ui->labelinfo_6_4;
            break;
        case CAR_4:
            tempLabel = ui->labelinfo_6_5;
            break;
        case CAR_5:
            tempLabel = ui->labelinfo_6_6;
            break;
        case CAR_6:
            tempLabel = ui->labelinfo_6_7;
            break;
        default:
            tempLabel = NULL;
            break;
        }
        break;
    default:
        tempLabel = NULL;
        break;
    }
    if(NULL == tempLabel){
        return ;
    }
    QString strText = "", strStyleSheet = "";
    if(0 == iState){
        strText = "";
        strStyleSheet = QSS_BG_PURPLE_BR_WHITE_WD_WHITE;
    }
    else{
        if(errCode > 0){
            strText = canSignalsSlots->getACCTempErrorInfo(errCode);
            strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
        }
        else{
            strText = "";
            strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        }
    }
    tempLabel->setText(strText);
    tempLabel->setStyleSheet(strStyleSheet);
    tempLabel = NULL;
}

void airconditioninfo::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void airconditioninfo::on_buttonTempUp_clicked(){
    if(carTempValue < 30){
        buttonBeep();
        if(18 == carTempValue){
            carTempValue = 20;
        }
        else{
            carTempValue++;
        }
        ui->labelacctemp->setText(QString("%1℃").arg(carTempValue));
        if(masterHMIFlag){
            canSignalsSlots->setDDUConfigInfo(2, 0, carTempValue);
        }
        if(nowCtrlHMIFlag){
            canSignalsSlots->sendCarACCSetTempValue();
        }
    }
}

void airconditioninfo::on_buttonTempDown_clicked(){
    if(carTempValue > 10){
        buttonBeep();
        if(20 == carTempValue){
            carTempValue = 18;
        }
        else{
            carTempValue--;
        }
        ui->labelacctemp->setText(QString("%1℃").arg(carTempValue));
        if(masterHMIFlag){
            canSignalsSlots->setDDUConfigInfo(2, 0, carTempValue);
        }
//        if(nowCtrlHMIFlag){
        canSignalsSlots->sendCarACCSetTempValue();
//        }
    }
}

void airconditioninfo::on_buttonmenu_clicked(){
    buttonBeep();
    emit sig_ACC_BackPBtn_Clicked();
}

void airconditioninfo::slot_CarIcon_Update(){
    if(CAR_NO_HEADER != carHeaderFlag){
        ui->labelarrow->setPixmap(ARROW_LEFT);
    }
    else{
        ui->labelarrow->setPixmap(NO_QPIXMAP);
    }
    ui->carimagelabel1->setPixmap(MC1_CAR_BMP);
    ui->carimagelabel2->setPixmap(T_CAR_BMP);
    ui->carimagelabel5->setPixmap(T_CAR_BMP);
    ui->carimagelabel6->setPixmap(MC2_CAR_BMP);

    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        ui->carimagelabel3->setPixmap(T_CAR_BMP);
        ui->carimagelabel4->setPixmap(M_CAR_BMP);

        ui->labelinfo_1_2->setText(CAR_6_ID);
        ui->labelinfo_1_3->setText(CAR_5_ID);
        ui->labelinfo_1_4->setText(CAR_4_ID);
        ui->labelinfo_1_5->setText(CAR_3_ID);
        ui->labelinfo_1_6->setText(CAR_2_ID);
        ui->labelinfo_1_7->setText(CAR_1_ID);
    }
    else{
        ui->carimagelabel3->setPixmap(M_CAR_BMP);
        ui->carimagelabel4->setPixmap(T_CAR_BMP);

        ui->labelinfo_1_2->setText(CAR_1_ID);
        ui->labelinfo_1_3->setText(CAR_2_ID);
        ui->labelinfo_1_4->setText(CAR_3_ID);
        ui->labelinfo_1_5->setText(CAR_4_ID);
        ui->labelinfo_1_6->setText(CAR_5_ID);
        ui->labelinfo_1_7->setText(CAR_6_ID);
    }
}

void airconditioninfo::slot_CarCode_Update(){
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        ui->carlabel1->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[5]));
        ui->carlabel2->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[4]));
        ui->carlabel3->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[3]));
        ui->carlabel4->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[2]));
        ui->carlabel5->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[1]));
        ui->carlabel6->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[0]));
    }
    else{
        ui->carlabel1->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[0]));
        ui->carlabel2->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[1]));
        ui->carlabel3->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[2]));
        ui->carlabel4->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[3]));
        ui->carlabel5->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[4]));
        ui->carlabel6->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[5]));
    }
}

void airconditioninfo::slot_Car_Direction_Update(){
    slot_CarIcon_Update();
    slot_CarCode_Update();
}

void airconditioninfo::slot_ACCData_Change(CAR_ID carId, int iState, int acc1Mode, int acc2Mode, int acc1Err, int acc2Err){
    int realCarId = 0;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = 7 - int(carId);
    }
    else{
        realCarId = carId;
    }
    setAirconditionState(MACHINE_1_STATE, CAR_ID(realCarId), iState, MACHINE_STATE(acc1Mode));
    setAirconditionState(MACHINE_2_STATE, CAR_ID(realCarId), iState, MACHINE_STATE(acc2Mode));
    setAirconditionError(MACHINE_1_BREAKDOWN, CAR_ID(realCarId), iState, acc1Err);
    setAirconditionError(MACHINE_2_BREAKDOWN, CAR_ID(realCarId), iState, acc2Err);
}

void airconditioninfo::slot_ACC_Temp_Update(CAR_ID carId, int iState, int iValue1, int iValue2){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    setTEMP(realCarId, INSIDE_TEMP_STATE(iState), iValue1, iValue2);
}

void airconditioninfo::slot_ACCCtrlMode(int ctrlMode){
    ACC_MODE_STATE accMode = ACC_MODE_STATE(ctrlMode);
    QString strText = "";
    switch (ctrlMode) {
    case ACCMODE_COMO_ERR:
        strText = "";
        break;
    case ACCMODE_WORK_STOP:
        strText = "停机";
        break;
    case ACCMODE_AERATION:
        strText = "通风";
        break;
    case ACCMODE_AUTO:
        strText = "自动";
        break;
    case ACCMODE_NET_CTRL:
        strText = "网控";
        break;
    case ACCMODE_HALF_COLD:
        strText = "弱冷";
        break;
    case ACCMODE_DEEP_COLD:
        strText = "强冷";
        break;
    case ACCMODE_HALF_HOT:
        strText = "半暖";
        break;
    case ACCMODE_ALL_HOT:
        strText = "全暖";
        break;
    default:
        strText = "";
        break;
    }
    if(/*nowCtrlHMIFlag &&*/ACCMODE_NET_CTRL == accMode){
        ui->label_2->setVisible(true);
        ui->labelacctemp->setVisible(true);
        ui->buttonTempUp->setVisible(true);
        ui->buttonTempDown->setVisible(true);
    }
    else{
        ui->label_2->setVisible(false);
        ui->labelacctemp->setVisible(false);
        ui->buttonTempUp->setVisible(false);
        ui->buttonTempDown->setVisible(false);
    }
    ui->labelaccmode->setText(strText);
}

void airconditioninfo::slot_ACC_SetTempValue_Update(){
    ui->labelacctemp->setText(QString("%1℃").arg(carTempValue));
}
