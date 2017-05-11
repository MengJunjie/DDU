#include "maincarinfo.h"
#include "ui_maincarinfo.h"
#include  <QPixmap>
#include  "../errinfo.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

MainCarInfo::MainCarInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainCarInfo)
{
    ui->setupUi(this);

//    iVVVFBCULevelState = 0;
//    iVVVFBCULevelValue = 0;
    for(int  i= 0; i < 6; i++){
        VVVFElecBrakeState[i] = ELEC_BRAKE_POWER_OFF;
        iVVVFElecBrakeStateValue[i] = 0;
    }

    Init();
    slot_CarIcon_Update();
    slot_CarCode_Update();

    setBrakeLevel(0, 0);

    setAirPumpRun(CAR_1, AIR_PUMP_STOP_WORK);
    setAirPumpRun(CAR_6, AIR_PUMP_STOP_WORK);

    setBHB_BLB(CAR_3, false, false);
    setBHB_BLB(CAR_2, false, false);

    connect(canSignalsSlots, SIGNAL(sig_Car_Direction_Change()), this, SLOT(slot_Car_Direction_Update()));
    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));
    connect(canSignalsSlots, SIGNAL(sig_Car_Code_Change()), this, SLOT(slot_CarCode_Update()));
    connect(canSignalsSlots, SIGNAL(sig_AirPump_State_Change(CAR_ID,bool)), this, SLOT(slot_AirPump_State_Update(CAR_ID,bool)));
    connect(canSignalsSlots, SIGNAL(sig_SIV_Direction_Change(SIV_TX_DIRECTION)), this, SLOT(slot_SIV_Direction_Change(SIV_TX_DIRECTION)));
    connect(canSignalsSlots, SIGNAL(sig_BHB_BLB_State_Change(CAR_ID,bool,bool)), this, SLOT(slot_BHB_BLB_State_Change(CAR_ID,bool,bool)));
    connect(canSignalsSlots, SIGNAL(sig_UrgentShort(CAR_ID,bool)), this, SLOT(slot_UrgentShort_Change(CAR_ID,bool)));
    connect(canSignalsSlots, SIGNAL(sig_BrakeIsolateDoor_Change(CAR_ID,bool)), this, SLOT(slot_BrakeIsolateDoor_Change(CAR_ID,bool)));
    connect(canSignalsSlots, SIGNAL(sig_VVVF_BCU_Level_Update(int, int)), this, SLOT(slot_VVVF_BCU_Level_Update(int, int)));
    connect(canSignalsSlots, SIGNAL(sig_VVVF_Elec_BCU_Update(CAR_ID,int,int,int)), this, SLOT(slot_VVVF_Elec_BCU_Update(CAR_ID,int,int,int)));
    connect(canSignalsSlots, SIGNAL(sig_BCU_BC_Kpa_Update(CAR_ID,int,int)), this, SLOT(slot_BCU_BC_Kpa_Update(CAR_ID,int,int)));
    connect(canSignalsSlots, SIGNAL(sig_SIV_Voltage_Freq_Update(CAR_ID,int,int,int)), this, SLOT(slot_SIV_Voltage_Freq_Update(CAR_ID,int,int,int)));
}

MainCarInfo::~MainCarInfo(){
    delete ui;
}

void MainCarInfo::Init()
{
    setWindowFlags(Qt::FramelessWindowHint);

//    ui->labelarrow->setScaledContents(true);
    ui->carimagelabel1->setScaledContents(true);
    ui->carimagelabel2->setScaledContents(true);
    ui->carimagelabel3->setScaledContents(true);
    ui->carimagelabel4->setScaledContents(true);
    ui->carimagelabel5->setScaledContents(true);
    ui->carimagelabel6->setScaledContents(true);

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void MainCarInfo::setVoltage(){
    QString strText = "", strStyleSheet = "";
    if(0 == carVoltageValue[0]){
        strText = "0V";
        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
    }
    else{
        strText = QString("%1V").arg(carVoltageValue[1]);
        if(carVoltageValue[1] < 500){
            strStyleSheet = QSS_BG_YELLOW_BR_WHITE_WD_BLACK;
        }
        else if(carVoltageValue[1] >= 500 && carVoltageValue[1] <= 900){
            strStyleSheet = QSS_BG_GREEN_BR_WHITE_WD_BLACK;
        }
        else{
            strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
        }
    }
    ui->labelvoltage->setText(strText);
    ui->labelvoltage->setStyleSheet(strStyleSheet);
}

void MainCarInfo::setCurrent(){
    QString strText = "", strStyleSheet = "";
    if(0 == carCurrentValue[0]){
        strText = "0A";
        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
    }
    else{
        strText = QString("%1A").arg(carCurrentValue[1]);
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
    }
    ui->labelcurrent->setText(strText);
    ui->labelcurrent->setStyleSheet(strStyleSheet);
}

void MainCarInfo::setBrakeLevel(int iState, int iValue){
    QString strText = "", strStyleSheet = "";
    BRAKE_LEVEL_STATE brakeLevel = BRAKE_LEVEL_STATE(iState);
    switch(brakeLevel){
    case BRAKE_COMN_ERR:
        strText = "OFF";
        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
        break;
    case BRAKE_URGENT_BRAKE:
        strText = "紧急制动";
        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
        break;
    case BRAKE_BRAKE:
        strText = "制动"+QString("%1%").arg(iValue);
        strStyleSheet = QSS_BG_BROWN_BR_WHITE_WD_BLACK;
        break;
    case BRAKE_LAZY:
        strText = "OFF";
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        break;
    case BRAKE_DRAG:
        strText = "牵引"+QString("%1%").arg(iValue);
        strStyleSheet = QSS_BG_GREEN_BR_WHITE_WD_BLACK;
        break;
    default:
        strText = "";
        strStyleSheet = "";
        break;
    }
    ui->labelbrake->setText(strText);
    ui->labelbrake->setStyleSheet(strStyleSheet);
}

void MainCarInfo::setSpeed(){
    QString strText = "", strStyleSheet = "";
    if(0 == carSpeedValue[0]){
        strText = "0";
        strStyleSheet = QSS_BG_PURPLE_BR_WHITE_WD_WHITE;
    }
    else{
        strText = QString("%1").arg(carSpeedValue[1]);
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
    }
    ui->labelspeed->setText(strText);
    ui->labelspeed->setStyleSheet(strStyleSheet);
}

void MainCarInfo::setElecBrake(DRAG_ELEC_BRAKE_TYPE type, CAR_ID carId, DRAG_ELEC_BRAKE_STATE level, int iValue){
    QLabel * tempLabel = NULL;
    switch (type) {
    case DRAG_ELEC_BRAKE_1:
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
        break;
    case DRAG_ELEC_BRAKE_2:
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
    default:
        tempLabel = NULL;
        break;
    }
    if(NULL == tempLabel){
        return ;
    }
    if((CAR_3 == carId || CAR_4 == carId)){
        if(DRAG_ELEC_BRAKE_1 == type){
            ui->labelinfo_2_4->setText("");
            ui->labelinfo_2_4->setStyleSheet("");
            ui->labelinfo_2_5->setText("");
            ui->labelinfo_2_5->setStyleSheet("");
        }
        else{
            ui->labelinfo_3_4->setText("");
            ui->labelinfo_3_4->setStyleSheet("");
            ui->labelinfo_3_5->setText("");
            ui->labelinfo_3_5->setStyleSheet("");
        }
    }
    QString strText = "", strStyleSheet = "";
    switch (level) {
    case ELEC_BRAKE_COMN_ERR:
        strText = "";
        strStyleSheet = QSS_BG_PURPLE_BR_WHITE_WD_WHITE;
        break;
    case ELEC_BRAKE_BREAKDOWN:
        strText = canSignalsSlots->getVVVFErrorInfo(iValue);
        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
        break;
    case ELEC_BRAKE_CUT_OFF:
        strText = "切除";
        strStyleSheet = QSS_BG_PURPLE_BR_WHITE_WD_WHITE;
        break;
    case ELEC_BRAKE_LAZY:
        strText = "0"; //QString("%1").arg(iValue);//"0";
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        break;
    case ELEC_BRAKE_DRAG:
        strText = QString("%1").arg(iValue);
        strStyleSheet = QSS_BG_GREEN_BR_WHITE_WD_BLACK;
        break;
    case ELEC_BRAKE_BRAKE:
        strText = QString("-%1").arg(iValue);
        strStyleSheet = QSS_BG_BROWN_BR_WHITE_WD_BLACK;
        break;
    case ELEC_BRAKE_POWER_OFF:
        strText = "OFF";
        strStyleSheet = QSS_BG_DARK_RED_BR_WHITE_WD_WHITE;
        break;
    case ELEC_BRAKE_URGENT_BRAKE:
        strText = "";
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
    default:
        strText = "";
        strStyleSheet = "";
        break;
    }
    tempLabel->setText(strText);
    tempLabel->setStyleSheet(strStyleSheet);
}

void MainCarInfo::setBC_KPA(CAR_ID carId, BC_KPA_STATE state, int value){
    QLabel * tempLabel = NULL;
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
    if(NULL == tempLabel){
        return ;
    }
    QString strText = "", strStyleSheet = "";
    switch (state) {
    case BC_COMN_ERR:
        strText = "";
        strStyleSheet = QSS_BG_PURPLE_BR_WHITE_WD_WHITE;
        break;
    case BC_BREAKDOWN:
        strText = canSignalsSlots->getBCUErrorInfo(value);
        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
        break;
    case BC_PARK_BRAKE:
        strText = "停放";
        strStyleSheet = QSS_BG_DARK_RED_BR_WHITE_WD_WHITE;
        break;
    case BC_POWER_OFF:
        strText = "OFF";
        strStyleSheet = QSS_BG_DARK_RED_BR_WHITE_WD_WHITE;
        break;
    case BC_NORMAL:
        strText = QString("%1").arg(value);
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

void MainCarInfo::setAirPumpRun(CAR_ID carId, AIR_PUMP_RUN_STATE workstate){
    QLabel * tempLabel = NULL;
    switch (carId) {
    case CAR_1:
        tempLabel = ui->labelinfo_5_2_1;
        break;
    case CAR_6:
        tempLabel = ui->labelinfo_5_7_1;
        break;
    default:
        tempLabel = NULL;
        break;
    }
    if(NULL == tempLabel){
        return ;
    }
    QString strStyleSheet = "";
    switch (workstate) {
    case AIR_PUMP_STOP_WORK:
        strStyleSheet = QSS_BG_BLACK_BR_GREEN_WD_WHITE;
        break;
    case AIR_PUMP_WORKING:
        strStyleSheet = QSS_BG_GREEN_BR_GREEN_WD_BLACK;
        break;
    default:
        strStyleSheet = "";
        break;
    }
    tempLabel->setStyleSheet(strStyleSheet);
}

void MainCarInfo::setSIVValue(CAR_ID carId, SIV_STATE workstate, int iValue1, int iValue2){
    QLabel * tempLabel = NULL;
    switch (carId) {
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
    default:
        tempLabel = NULL;
        break;
    }
    if(NULL == tempLabel){
        return ;
    }
    if(CAR_2 == carId || CAR_5 == carId){
        ui->labelinfo_6_3->setText("");
        ui->labelinfo_6_3->setStyleSheet("");
        ui->labelinfo_6_6->setText("");
        ui->labelinfo_6_6->setStyleSheet("");
    }
    else if(CAR_4 == carId || CAR_3 == carId){
        ui->labelinfo_6_5->setText("");
        ui->labelinfo_6_5->setStyleSheet("");
        ui->labelinfo_6_4->setText("");
        ui->labelinfo_6_4->setStyleSheet("");
    }
    else{
        // do nothing
    }
    QString strText = "", strStyleSheet = "";
    switch (workstate) {
    case SIV_COMN_ERR:
        strText = "";
        strStyleSheet = QSS_BG_PURPLE_BR_WHITE_WD_WHITE;
        break;
    case SIV_BREAKDOWN:
        strText = canSignalsSlots->getSIVErrorInfo(iValue1);
        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
        break;
    case SIV_POWER_OFF:
        strText = "OFF";
        strStyleSheet = QSS_BG_DARK_RED_BR_WHITE_WD_WHITE;
        break;
    case SIV_NORMAL:
        strText = QString("%1V\n%2Hz").arg(iValue1).arg(iValue2);
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

void MainCarInfo::setBHB_BLB(CAR_ID carId, bool bBHBState, bool bBLBState){
    QList<QLabel*> bhb_blbList1;
    QList<QLabel*> bhb_blbList2;
    bhb_blbList1.clear();
    bhb_blbList2.clear();
    bhb_blbList1 << ui->labelinfo_8_3_1 << ui->labelinfo_8_3_2 << ui->labelinfo_8_4_1 << ui->labelinfo_8_4_2;
    bhb_blbList2 << ui->labelinfo_8_5_1 << ui->labelinfo_8_5_2 << ui->labelinfo_8_6_1 << ui->labelinfo_8_6_2;
    QLabel * tempLabel1 = NULL;
    QLabel * tempLabel2 = NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        for(int i = 0; i < bhb_blbList1.length(); i++){
            bhb_blbList1.at(i)->setText("");
            bhb_blbList1.at(i)->setStyleSheet("");
        }
        switch (carId) {
        case CAR_2:
            tempLabel1 = ui->labelinfo_8_6_1;
            tempLabel2 = ui->labelinfo_8_6_2;
            break;
        case CAR_3:
            tempLabel1 = ui->labelinfo_8_5_1;
            tempLabel2 = ui->labelinfo_8_5_2;
            break;
        default:
            tempLabel1 = NULL;
            tempLabel2 = NULL;
            break;
        }
    }
    else{
        for(int i = 0; i < bhb_blbList2.length(); i++){
            bhb_blbList2.at(i)->setText("");
            bhb_blbList2.at(i)->setStyleSheet("");
        }
        switch (carId) {
        case CAR_2:
            tempLabel1 = ui->labelinfo_8_3_1;
            tempLabel2 = ui->labelinfo_8_3_2;
            break;
        case CAR_3:
            tempLabel1 = ui->labelinfo_8_4_1;
            tempLabel2 = ui->labelinfo_8_4_2;
            break;
        default:
            tempLabel1 = NULL;
            tempLabel2 = NULL;
            break;
        }
    }
    if(NULL == tempLabel1 || NULL == tempLabel2){
        return;
    }
    tempLabel1->setText("H");
    tempLabel2->setText("L");
    if(bBHBState){
        tempLabel1->setStyleSheet(QSS_BG_GREEN_BR_GREEN_WD_BLACK);
    }
    else{
        tempLabel1->setStyleSheet(QSS_BG_BLACK_BR_GREEN_WD_WHITE);
    }
    if(bBLBState){
        tempLabel2->setStyleSheet(QSS_BG_GREEN_BR_GREEN_WD_BLACK);
    }
    else{
        tempLabel2->setStyleSheet(QSS_BG_BLACK_BR_GREEN_WD_WHITE);
    }
}

void MainCarInfo::setUrgentShort(CAR_ID carId, bool bState){
    QLabel * tempLabel = NULL;
    switch (carId) {
    case CAR_1:
        tempLabel = ui->labelinfo_9_2;
        break;
    case CAR_6:
        tempLabel = ui->labelinfo_9_7;
        break;
    default:
        tempLabel = NULL;
        break;
    }
    if(NULL == tempLabel){
        return ;
    }
    QString strStyleSheet = "";
    if(bState){
        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
    }
    else{
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
    }
    tempLabel->setStyleSheet(strStyleSheet);
}

void MainCarInfo::setDoorBrake(CAR_ID carId, bool bState){
    QLabel * tempLabel = NULL;
    switch (carId) {
    case CAR_1:
        tempLabel = ui->labelinfo_10_2;
        break;
    case CAR_2:
        tempLabel = ui->labelinfo_10_3;
        break;
    case CAR_3:
        tempLabel = ui->labelinfo_10_4;
        break;
    case CAR_4:
        tempLabel = ui->labelinfo_10_5;
        break;
    case CAR_5:
        tempLabel = ui->labelinfo_10_6;
        break;
    case CAR_6:
        tempLabel = ui->labelinfo_10_7;
        break;
    default:
        tempLabel = NULL;
        break;
    }
    if(NULL == tempLabel){
        return ;
    }
    QString strStyleSheet = "";
    if(bState){
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE; //QSS_BG_RED_BR_WHITE_WD_WHITE;
    }
    else{
        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE; //QSS_BG_BLACK_BR_WHITE_WD_WHITE;
    }
    tempLabel->setStyleSheet(strStyleSheet);
}

void MainCarInfo::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
    setVoltage();
    setCurrent();
    setSpeed();
}

void MainCarInfo::on_buttonmenu_clicked(){
    emit sig_LowPBtn_Clicked(MAINMENU_BTN_CLICKED);
}

void MainCarInfo::on_buttonaircondition_clicked(){
    emit sig_LowPBtn_Clicked(ACC_BTN_CLICKED);
}

void MainCarInfo::on_buttonerr_clicked(){
    emit sig_LowPBtn_Clicked(ERROR_BTN_CLICKED);
}

void MainCarInfo::on_buttonrun_clicked(){
    emit sig_LowPBtn_Clicked(KEEPMOVE_BTN_CLICKED);
}

void MainCarInfo::slot_CarIcon_Update(){
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
        ui->labelinfo_7_3->setVisible(true);
        ui->labelinfo_7_345->setVisible(false);
        ui->labelinfo_7_456->setVisible(true);
        ui->labelinfo_7_6->setVisible(false);
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
        ui->labelinfo_7_3->setVisible(false);
        ui->labelinfo_7_345->setVisible(true);
        ui->labelinfo_7_456->setVisible(false);
        ui->labelinfo_7_6->setVisible(true);
    }
}

void MainCarInfo::slot_CarCode_Update(){
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

void MainCarInfo::slot_Car_Direction_Update(){
    slot_CarIcon_Update();
    slot_CarCode_Update();
}

void MainCarInfo::slot_AirPump_State_Update(CAR_ID carId, bool bState){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    AIR_PUMP_RUN_STATE tempAirPumpState = AIR_PUMP_STOP_WORK;
    if(bState){
        tempAirPumpState = AIR_PUMP_WORKING;
    }
    else{
        tempAirPumpState = AIR_PUMP_STOP_WORK;
    }
    setAirPumpRun(realCarId, tempAirPumpState);
}

void MainCarInfo::slot_SIV_Direction_Change(SIV_TX_DIRECTION sivDirection){
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        switch (sivDirection) {
        case SIV_2_TO_4:
            ui->labelinfo_7_456->setPixmap(SIV_LEFT_BMP);
            break;
        case SIV_4_TO_2:
            ui->labelinfo_7_456->setPixmap(SIV_RIGHT_BMP);
            break;
        case SIV_NONE:
            ui->labelinfo_7_456->setPixmap(NO_QPIXMAP);
            break;
        default:
            ui->labelinfo_7_456->setPixmap(NO_QPIXMAP);
            break;
        }
    }
    else{
        switch (sivDirection) {
        case SIV_2_TO_4:
            ui->labelinfo_7_345->setPixmap(SIV_RIGHT_BMP);
            break;
        case SIV_4_TO_2:
            ui->labelinfo_7_345->setPixmap(SIV_LEFT_BMP);
            break;
        case SIV_NONE:
            ui->labelinfo_7_345->setPixmap(NO_QPIXMAP);
            break;
        default:
            ui->labelinfo_7_345->setPixmap(NO_QPIXMAP);
            break;
        }
    }
}

void MainCarInfo::slot_BHB_BLB_State_Change(CAR_ID carId, bool bBHBFlag, bool bBLBFlag){
    setBHB_BLB(carId, bBHBFlag, bBLBFlag);
}

void MainCarInfo::slot_UrgentShort_Change(CAR_ID carId, bool bState){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    setUrgentShort(realCarId, bState);
}

void MainCarInfo::slot_BrakeIsolateDoor_Change(CAR_ID carId, bool bState){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7-carId);
    }
    else{
        realCarId = carId;
    }
    setDoorBrake(realCarId, bState);
}

void MainCarInfo::slot_VVVF_BCU_Level_Update(int iState, int iValue){
//    iVVVFBCULevelState = iState;
//    iVVVFBCULevelValue = iValue;
    setBrakeLevel(iState, iValue);
}

void MainCarInfo::slot_VVVF_Elec_BCU_Update(CAR_ID carId, int vvvfIndex, int iState, int iValue){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    DRAG_ELEC_BRAKE_TYPE tempType = DRAG_ELEC_BRAKE_TYPE(vvvfIndex-1);
    DRAG_ELEC_BRAKE_STATE tempState = DRAG_ELEC_BRAKE_STATE(iState);
    setElecBrake(tempType, realCarId, tempState, iValue);
}

void MainCarInfo::slot_BCU_BC_Kpa_Update(CAR_ID carId, int iState, int iValue){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    BC_KPA_STATE tempState = BC_KPA_STATE(iState);
    setBC_KPA(realCarId, tempState, iValue);
}

void MainCarInfo::slot_SIV_Voltage_Freq_Update(CAR_ID carId, int iState, int iValue1, int iValue2){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    SIV_STATE tempState = SIV_STATE(iState);
    setSIVValue(realCarId, tempState, iValue1, iValue2);
}
