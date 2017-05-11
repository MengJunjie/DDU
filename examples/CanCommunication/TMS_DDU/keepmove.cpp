#include "keepmove.h"
#include "ui_keepmove.h"
#include "errinfo.h"
#include "../can1operation.h"
#include "../can2operation.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

keepmove::keepmove(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::keepmove)
{
    ui->setupUi(this);

    Init();
    slot_CarIcon_Update();
    slot_CarCode_Update();
    setDoorName();
    slot_ATOModel_Update(0);

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));
    connect(canSignalsSlots, SIGNAL(sig_Car_Direction_Change()), this, SLOT(slot_Car_Direction_Update()));
    connect(canSignalsSlots, SIGNAL(sig_Car_Code_Change()), this, SLOT(slot_CarCode_Update()));
    connect(canSignalsSlots, SIGNAL(sig_Door_State_Change(CAR_ID,DOOR_ID,RUN_DOOR_STATE)), this, SLOT(slot_Door_State_Update(CAR_ID,DOOR_ID,RUN_DOOR_STATE)));
    connect(canSignalsSlots, SIGNAL(sig_Car_ATOModel_Change(int)), this, SLOT(slot_ATOModel_Update(int)));
    connect(canSignalsSlots, SIGNAL(sig_VVVF_Elec_BCU_Update(CAR_ID,int,int,int)), this, SLOT(slot_VVVF_Elec_BCU_Update(CAR_ID,int,int,int)));
    connect(canSignalsSlots, SIGNAL(sig_BCU_BC_Kpa_Update(CAR_ID,int,int)), this, SLOT(slot_BCU_BC_Kpa_Update(CAR_ID,int,int)));
    connect(canSignalsSlots, SIGNAL(sig_Load_Rate_Update(CAR_ID,int,int)), this, SLOT(slot_Load_Rate_Update(CAR_ID,int,int)));
    connect(canSignalsSlots, SIGNAL(sig_ACC_Temp_Update(CAR_ID,int,int,int)), this, SLOT(slot_ACC_Temp_Update(CAR_ID,int,int,int)));
    connect(canSignalsSlots, SIGNAL(sig_Urgent_Alert_Update(CAR_ID,bool)), this, SLOT(slot_Urgent_Alert_Update(CAR_ID,bool)));
    connect(canSignalsSlots, SIGNAL(sig_UrgentShort(CAR_ID,bool)), this, SLOT(slot_UrgentShort_Update(CAR_ID,bool)));
    connect(canSignalsSlots, SIGNAL(sig_BrakeIsolateDoor_Change(CAR_ID,bool)), this, SLOT(slot_BrakeIsolateDoor_Update(CAR_ID,bool)));
    connect(canSignalsSlots, SIGNAL(sig_Car_Arrive_Station_Update(int,int)), this, SLOT(slot_Car_Arrive_Station_Update(int,int)));
}

keepmove::~keepmove(){
    delete ui;
}

void keepmove::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

    ui->labelrunarrow->setPixmap(ARROW_LEFT_1);
//    ui->labelrunarrow->setScaledContents(true);

//    ui->labelarrow->setScaledContents(true);
    ui->carimagelabel1->setScaledContents(true);
    ui->carimagelabel2->setScaledContents(true);
    ui->carimagelabel3->setScaledContents(true);
    ui->carimagelabel4->setScaledContents(true);
    ui->carimagelabel5->setScaledContents(true);
    ui->carimagelabel6->setScaledContents(true);

    ui->buttonruntable->setEnabled(false);
    ui->buttonforward->setEnabled(false);
    ui->buttonbackward->setEnabled(false);

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void keepmove::setDoorName(){
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        ui->labelinfo_5_2_1_1->setText("A64");
        ui->labelinfo_5_2_2_1->setText("A63");
        ui->labelinfo_5_2_3_1->setText("A62");
        ui->labelinfo_5_2_4_1->setText("A61");
        ui->labelinfo_5_3_1_1->setText("A54");
        ui->labelinfo_5_3_2_1->setText("A53");
        ui->labelinfo_5_3_3_1->setText("A52");
        ui->labelinfo_5_3_4_1->setText("A51");
        ui->labelinfo_5_4_1_1->setText("A44");
        ui->labelinfo_5_4_2_1->setText("A43");
        ui->labelinfo_5_4_3_1->setText("A42");
        ui->labelinfo_5_4_4_1->setText("A41");
        ui->labelinfo_5_5_1_1->setText("A34");
        ui->labelinfo_5_5_2_1->setText("A33");
        ui->labelinfo_5_5_3_1->setText("A32");
        ui->labelinfo_5_5_4_1->setText("A31");
        ui->labelinfo_5_6_1_1->setText("A24");
        ui->labelinfo_5_6_2_1->setText("A23");
        ui->labelinfo_5_6_3_1->setText("A22");
        ui->labelinfo_5_6_4_1->setText("A21");
        ui->labelinfo_5_7_1_1->setText("A14");
        ui->labelinfo_5_7_2_1->setText("A13");
        ui->labelinfo_5_7_3_1->setText("A12");
        ui->labelinfo_5_7_4_1->setText("A11");

        ui->labelinfo_6_2_1_1->setText("B64");
        ui->labelinfo_6_2_2_1->setText("B63");
        ui->labelinfo_6_2_3_1->setText("B62");
        ui->labelinfo_6_2_4_1->setText("B61");
        ui->labelinfo_6_3_1_1->setText("B54");
        ui->labelinfo_6_3_2_1->setText("B53");
        ui->labelinfo_6_3_3_1->setText("B52");
        ui->labelinfo_6_3_4_1->setText("B51");
        ui->labelinfo_6_4_1_1->setText("B44");
        ui->labelinfo_6_4_2_1->setText("B43");
        ui->labelinfo_6_4_3_1->setText("B42");
        ui->labelinfo_6_4_4_1->setText("B41");
        ui->labelinfo_6_5_1_1->setText("B34");
        ui->labelinfo_6_5_2_1->setText("B33");
        ui->labelinfo_6_5_3_1->setText("B32");
        ui->labelinfo_6_5_4_1->setText("B31");
        ui->labelinfo_6_6_1_1->setText("B24");
        ui->labelinfo_6_6_2_1->setText("B23");
        ui->labelinfo_6_6_3_1->setText("B22");
        ui->labelinfo_6_6_4_1->setText("B21");
        ui->labelinfo_6_7_1_1->setText("B14");
        ui->labelinfo_6_7_2_1->setText("B13");
        ui->labelinfo_6_7_3_1->setText("B12");
        ui->labelinfo_6_7_4_1->setText("B11");
    }
    else {
        ui->labelinfo_5_2_1_1->setText("B11");
        ui->labelinfo_5_2_2_1->setText("B12");
        ui->labelinfo_5_2_3_1->setText("B13");
        ui->labelinfo_5_2_4_1->setText("B14");
        ui->labelinfo_5_3_1_1->setText("B21");
        ui->labelinfo_5_3_2_1->setText("B22");
        ui->labelinfo_5_3_3_1->setText("B23");
        ui->labelinfo_5_3_4_1->setText("B24");
        ui->labelinfo_5_4_1_1->setText("B31");
        ui->labelinfo_5_4_2_1->setText("B32");
        ui->labelinfo_5_4_3_1->setText("B33");
        ui->labelinfo_5_4_4_1->setText("B34");
        ui->labelinfo_5_5_1_1->setText("B41");
        ui->labelinfo_5_5_2_1->setText("B42");
        ui->labelinfo_5_5_3_1->setText("B43");
        ui->labelinfo_5_5_4_1->setText("B44");
        ui->labelinfo_5_6_1_1->setText("B51");
        ui->labelinfo_5_6_2_1->setText("B52");
        ui->labelinfo_5_6_3_1->setText("B53");
        ui->labelinfo_5_6_4_1->setText("B54");
        ui->labelinfo_5_7_1_1->setText("B61");
        ui->labelinfo_5_7_2_1->setText("B62");
        ui->labelinfo_5_7_3_1->setText("B63");
        ui->labelinfo_5_7_4_1->setText("B64");

        ui->labelinfo_6_2_1_1->setText("A11");
        ui->labelinfo_6_2_2_1->setText("A12");
        ui->labelinfo_6_2_3_1->setText("A13");
        ui->labelinfo_6_2_4_1->setText("A14");
        ui->labelinfo_6_3_1_1->setText("A21");
        ui->labelinfo_6_3_2_1->setText("A22");
        ui->labelinfo_6_3_3_1->setText("A23");
        ui->labelinfo_6_3_4_1->setText("A24");
        ui->labelinfo_6_4_1_1->setText("A31");
        ui->labelinfo_6_4_2_1->setText("A32");
        ui->labelinfo_6_4_3_1->setText("A33");
        ui->labelinfo_6_4_4_1->setText("A34");
        ui->labelinfo_6_5_1_1->setText("A41");
        ui->labelinfo_6_5_2_1->setText("A42");
        ui->labelinfo_6_5_3_1->setText("A43");
        ui->labelinfo_6_5_4_1->setText("A44");
        ui->labelinfo_6_6_1_1->setText("A51");
        ui->labelinfo_6_6_2_1->setText("A52");
        ui->labelinfo_6_6_3_1->setText("A53");
        ui->labelinfo_6_6_4_1->setText("A54");
        ui->labelinfo_6_7_1_1->setText("A61");
        ui->labelinfo_6_7_2_1->setText("A62");
        ui->labelinfo_6_7_3_1->setText("A63");
        ui->labelinfo_6_7_4_1->setText("A64");
    }
}

void keepmove::slot_ATOModel_Update(int model){
    QString strText = "", strStyleSheet = "";
    ATO_MODEL atoMode = ATO_MODEL(model);
    switch (atoMode) {
    case ATO_POWER_OFF:
        strText = "ATO无效";
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        break;
    case ATO_COMN_ERR:
        strText = "ATO无效";
        strStyleSheet = QSS_BG_PURPLE_BR_PURPLE_WD_BLACK;
        break;
    case ATO_INVALID:
        strText = "ATO无效";
        strStyleSheet = QSS_BG_YELLOW_BR_NONE_WD_BLACK;
        break;
    case ATO_VALID:
        strText = "ATO模式";
        strStyleSheet = QSS_BG_GREEN_BR_NONE_WD_BLACK;
        break;
    case ATC_CUT_OFF:
        strText = "ATC切除";
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        break;
    default:
        strText = "";
        strStyleSheet = "";
        break;
    }
    ui->labelATO->setText(strText);
    ui->labelATO->setStyleSheet(strStyleSheet);
}

void keepmove::setVoltage(){
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
    ui->title_4->setText(strText);
    ui->title_4->setStyleSheet(strStyleSheet);
}

void keepmove::setCurrent(){
    QString strText = "", strStyleSheet = "";
    if(0 == carCurrentValue[0]){
        strText = "0A";
        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
    }
    else{
        strText = QString("%1A").arg(carCurrentValue[1]);
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
    }
    ui->title_6->setText(strText);
    ui->title_6->setStyleSheet(strStyleSheet);
}

//void keepmove::setEndStation(int endIndex){

//}

//void keepmove::setNextStation(int nextIndex){

//}

//void keepmove::setCurrentStation(int curIndex){

//}

void keepmove::setElecBrake(DRAG_ELEC_BRAKE_TYPE type, CAR_ID carId, DRAG_ELEC_BRAKE_STATE level, int iValue){
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

void keepmove::setBC_KPA(CAR_ID carId, BC_KPA_STATE state, int value){
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

void keepmove::setDoorState(KEEPMOVE_ITEM item, CAR_ID carId, DOOR_ID doorId, RUN_DOOR_STATE doorState){
    QLabel * tempLabel1 = NULL;
    QLabel * tempLabel2 = NULL;
    switch (item) {
    case RUN_RIGHT_DOOR:
        switch (carId) {
        case CAR_1:
            switch (doorId) {
            case DOOR_1:
                tempLabel1 = ui->labelinfo_5_2_1;
                tempLabel2 = ui->labelinfo_5_2_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_5_2_2;
                tempLabel2 = ui->labelinfo_5_2_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_5_2_3;
                tempLabel2 = ui->labelinfo_5_2_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_5_2_4;
                tempLabel2 = ui->labelinfo_5_2_4_1;
                break;
            default:
                tempLabel1 = NULL;
                tempLabel2 = NULL;
                break;
            }
            break;
        case CAR_2:
            switch (doorId) {
            case DOOR_1:
                tempLabel1 = ui->labelinfo_5_3_1;
                tempLabel2 = ui->labelinfo_5_3_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_5_3_2;
                tempLabel2 = ui->labelinfo_5_3_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_5_3_3;
                tempLabel2 = ui->labelinfo_5_3_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_5_3_4;
                tempLabel2 = ui->labelinfo_5_3_4_1;
                break;
            default:
                tempLabel1 = NULL;
                tempLabel2 = NULL;
                break;
            }
            break;
        case CAR_3:
            switch (doorId) {
            case DOOR_1:
                tempLabel1 = ui->labelinfo_5_4_1;
                tempLabel2 = ui->labelinfo_5_4_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_5_4_2;
                tempLabel2 = ui->labelinfo_5_4_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_5_4_3;
                tempLabel2 = ui->labelinfo_5_4_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_5_4_4;
                tempLabel2 = ui->labelinfo_5_4_4_1;
                break;
            default:
                tempLabel1 = NULL;
                tempLabel2 = NULL;
                break;
            }
            break;
        case CAR_4:
            switch (doorId) {
            case DOOR_1:
                tempLabel1 = ui->labelinfo_5_5_1;
                tempLabel2 = ui->labelinfo_5_5_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_5_5_2;
                tempLabel2 = ui->labelinfo_5_5_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_5_5_3;
                tempLabel2 = ui->labelinfo_5_5_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_5_5_4;
                tempLabel2 = ui->labelinfo_5_5_4_1;
                break;
            default:
                tempLabel1 = NULL;
                tempLabel2 = NULL;
                break;
            }
            break;
        case CAR_5:
            switch (doorId) {
            case DOOR_1:
                tempLabel1 = ui->labelinfo_5_6_1;
                tempLabel2 = ui->labelinfo_5_6_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_5_6_2;
                tempLabel2 = ui->labelinfo_5_6_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_5_6_3;
                tempLabel2 = ui->labelinfo_5_6_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_5_6_4;
                tempLabel2 = ui->labelinfo_5_6_4_1;
                break;
            default:
                tempLabel1 = NULL;
                tempLabel2 = NULL;
                break;
            }
            break;
        case CAR_6:
            switch (doorId) {
            case DOOR_1:
                tempLabel1 = ui->labelinfo_5_7_1;
                tempLabel2 = ui->labelinfo_5_7_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_5_7_2;
                tempLabel2 = ui->labelinfo_5_7_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_5_7_3;
                tempLabel2 = ui->labelinfo_5_7_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_5_7_4;
                tempLabel2 = ui->labelinfo_5_7_4_1;
                break;
            default:
                tempLabel1 = NULL;
                tempLabel2 = NULL;
                break;
            }
            break;
        default:
            tempLabel1 = NULL;
            tempLabel2 = NULL;
            break;
        }
        break;
    case RUN_LEFT_DOOR:
        switch (carId) {
        case CAR_1:
            switch (doorId) {
            case DOOR_1:
                tempLabel1 = ui->labelinfo_6_2_1;
                tempLabel2 = ui->labelinfo_6_2_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_6_2_2;
                tempLabel2 = ui->labelinfo_6_2_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_6_2_3;
                tempLabel2 = ui->labelinfo_6_2_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_6_2_4;
                tempLabel2 = ui->labelinfo_6_2_4_1;
                break;
            default:
                tempLabel1 = NULL;
                tempLabel2 = NULL;
                break;
            }
            break;
        case CAR_2:
            switch (doorId) {
            case DOOR_1:
                tempLabel1 = ui->labelinfo_6_3_1;
                tempLabel2 = ui->labelinfo_6_3_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_6_3_2;
                tempLabel2 = ui->labelinfo_6_3_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_6_3_3;
                tempLabel2 = ui->labelinfo_6_3_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_6_3_4;
                tempLabel2 = ui->labelinfo_6_3_4_1;
                break;
            default:
                tempLabel1 = NULL;
                tempLabel2 = NULL;
                break;
            }
            break;
        case CAR_3:
            switch (doorId) {
            case DOOR_1:
                tempLabel1 = ui->labelinfo_6_4_1;
                tempLabel2 = ui->labelinfo_6_4_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_6_4_2;
                tempLabel2 = ui->labelinfo_6_4_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_6_4_3;
                tempLabel2 = ui->labelinfo_6_4_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_6_4_4;
                tempLabel2 = ui->labelinfo_6_4_4_1;
                break;
            default:
                tempLabel1 = NULL;
                tempLabel2 = NULL;
                break;
            }
            break;
        case CAR_4:
            switch (doorId) {
            case DOOR_1:
                tempLabel1 = ui->labelinfo_6_5_1;
                tempLabel2 = ui->labelinfo_6_5_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_6_5_2;
                tempLabel2 = ui->labelinfo_6_5_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_6_5_3;
                tempLabel2 = ui->labelinfo_6_5_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_6_5_4;
                tempLabel2 = ui->labelinfo_6_5_4_1;
                break;
            default:
                tempLabel1 = NULL;
                tempLabel2 = NULL;
                break;
            }
            break;
        case CAR_5:
            switch (doorId) {
            case DOOR_1:
                tempLabel1 = ui->labelinfo_6_6_1;
                tempLabel2 = ui->labelinfo_6_6_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_6_6_2;
                tempLabel2 = ui->labelinfo_6_6_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_6_6_3;
                tempLabel2 = ui->labelinfo_6_6_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_6_6_4;
                tempLabel2 = ui->labelinfo_6_6_4_1;
                break;
            default:
                tempLabel1 = NULL;
                tempLabel2 = NULL;
                break;
            }
            break;
        case CAR_6:
            switch (doorId) {
            case DOOR_1:
                tempLabel1 = ui->labelinfo_6_7_1;
                tempLabel2 = ui->labelinfo_6_7_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_6_7_2;
                tempLabel2 = ui->labelinfo_6_7_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_6_7_3;
                tempLabel2 = ui->labelinfo_6_7_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_6_7_4;
                tempLabel2 = ui->labelinfo_6_7_4_1;
                break;
            default:
                tempLabel1 = NULL;
                tempLabel2 = NULL;
                break;
            }
            break;
        default:
            tempLabel1 = NULL;
            tempLabel2 = NULL;
            break;
        }
        break;
    default:
        tempLabel1 = NULL;
        tempLabel2 = NULL;
        break;
    }
    if(NULL == tempLabel1 || NULL == tempLabel2){
        return ;
    }
    QString strText = "", strStyleSheet1 = "", strStyleSheet2 = "";
    switch (doorState) {
    case RUN_DOOR_COMN_ERR:
        strStyleSheet1 = QSS_BG_GREY_BR_NONE_WD_BLACK;
        strStyleSheet2 = QSS_BG_GREY_BR_NONE_WD_BLACK;
        break;
    case RUN_DOOR_ISOLATE:
        strText = "╳";  //"X";
        strStyleSheet1 = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        strStyleSheet2 = QSS_BG_BLACK_BR_NONE_WD_WHITE;
        break;
    case RUN_DOOR_URGENT_ACTION:
        strStyleSheet1 = QSS_BG_PURPLE_BR_NONE_WD_WHITE;
        strStyleSheet2 = QSS_BG_PURPLE_BR_NONE_WD_WHITE;
        break;
    case RUN_DOOR_BREAKDOWN:
        strStyleSheet1 = QSS_BG_RED_BR_NONE_WD_WHITE;
        strStyleSheet2 = QSS_BG_RED_BR_NONE_WD_WHITE;
        break;
    case RUN_DOOR_PREVENT_SQUEEZE:
        strStyleSheet1 = QSS_BG_ORANGE_BR_NONE_WD_WHITE;
        strStyleSheet2 = QSS_BG_ORANGE_BR_NONE_WD_WHITE;
        break;
    case RUN_DOOR_OPENED:
        strStyleSheet1 = QSS_BG_YELLOW_BR_NONE_WD_BLACK;
        strStyleSheet2 = QSS_BG_YELLOW_BR_NONE_WD_BLACK;
        break;
    case RUN_DOOR_OPENING_CLOSING:
        strStyleSheet1 = QSS_BG_BLUE_BR_NONE_WD_WHITE;
        strStyleSheet2 = QSS_BG_BLUE_BR_NONE_WD_WHITE;
        break;
    case RUN_DOOR_CLOSED:
        strStyleSheet1 = QSS_BG_GREEN_BR_NONE_WD_BLACK;
        strStyleSheet2 = QSS_BG_GREEN_BR_NONE_WD_BLACK;
        break;
    default:
        strText = "";
        strStyleSheet1 = "";
        strStyleSheet2 = "";
        break;
    }
    tempLabel1->setText(strText);
    tempLabel1->setStyleSheet(strStyleSheet1);
    tempLabel2->setStyleSheet(strStyleSheet2);
}

void keepmove::setLoadRate(CAR_ID carId, RUN_RIDE_RATE_STATE state, int value){
    QLabel * tempLabel = NULL;
    switch (carId) {
    case CAR_1:
        tempLabel = ui->labelinfo_7_2;
        break;
    case CAR_2:
        tempLabel = ui->labelinfo_7_3;
        break;
    case CAR_3:
        tempLabel = ui->labelinfo_7_4;
        break;
    case CAR_4:
        tempLabel = ui->labelinfo_7_5;
        break;
    case CAR_5:
        tempLabel = ui->labelinfo_7_6;
        break;
    case CAR_6:
        tempLabel = ui->labelinfo_7_7;
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
    case RIDE_RATE_COMN_ERR:
        strText = "";
        strStyleSheet = QSS_BG_PURPLE_BR_WHITE_WD_WHITE;
        break;
    case RIDE_RATE_POWER_OFF:
        strText = "OFF";
        strStyleSheet = QSS_BG_PURPLE_BR_WHITE_WD_WHITE;
        break;
    case RIDE_RATE_BREAKDOWN:
        strText = "";
        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
        break;
    case RIDE_RATE_NORMAL:
        if(255 == value){
            strText = QString("128%");
        }
        else{
            strText = QString("%1%").arg(value);
        }
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

void keepmove::setInsideTEMP(CAR_ID carId, INSIDE_TEMP_STATE state, int tempValue, int errCode){
    QLabel * tempLabel = NULL;
    switch (carId) {
    case CAR_1:
        tempLabel = ui->labelinfo_8_2;
        break;
    case CAR_2:
        tempLabel = ui->labelinfo_8_3;
        break;
    case CAR_3:
        tempLabel = ui->labelinfo_8_4;
        break;
    case CAR_4:
        tempLabel = ui->labelinfo_8_5;
        break;
    case CAR_5:
        tempLabel = ui->labelinfo_8_6;
        break;
    case CAR_6:
        tempLabel = ui->labelinfo_8_7;
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
        strText = canSignalsSlots->getACCTempErrorInfo(errCode);
        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
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

void keepmove::setUrgent_Alert(CAR_ID carId, bool bState){
    QLabel * tempLabel = NULL;

    switch (carId) {
    case CAR_1:
        tempLabel = ui->labelinfo_9_2;
        break;
    case CAR_2:
        tempLabel = ui->labelinfo_9_3;
        break;
    case CAR_3:
        tempLabel = ui->labelinfo_9_4;
        break;
    case CAR_4:
        tempLabel = ui->labelinfo_9_5;
        break;
    case CAR_5:
        tempLabel = ui->labelinfo_9_6;
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

void keepmove::setUrgent_Short(CAR_ID carId, bool bState){
    QLabel * tempLabel = NULL;

    switch (carId) {
    case CAR_1:
        tempLabel = ui->labelinfo_10_2;
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
        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
    }
    else{
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
    }

    tempLabel->setStyleSheet(strStyleSheet);
}

void keepmove::setBrakeIsolateDoor(CAR_ID carId, bool bState){
    QLabel * tempLabel = NULL;
    switch (carId) {
    case CAR_1:
        tempLabel = ui->labelinfo_11_2;
        break;
    case CAR_2:
        tempLabel = ui->labelinfo_11_3;
        break;
    case CAR_3:
        tempLabel = ui->labelinfo_11_4;
        break;
    case CAR_4:
        tempLabel = ui->labelinfo_11_5;
        break;
    case CAR_5:
        tempLabel = ui->labelinfo_11_6;
        break;
    case CAR_6:
        tempLabel = ui->labelinfo_11_7;
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

void keepmove::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
    setVoltage();
    setCurrent();
}

void keepmove::on_buttonmenu_clicked(){
    emit sig_LowPBtn_Clicked(MAINMENU_BTN_CLICKED);
}

void keepmove::on_buttoncarinfo_clicked(){
    emit sig_LowPBtn_Clicked(CARINFO_BTN_CLICKED);
}

void keepmove::on_buttonaccset_clicked(){
    emit sig_LowPBtn_Clicked(ACC_BTN_CLICKED);
}

void keepmove::on_buttonerr_clicked(){
    emit sig_LowPBtn_Clicked(ERROR_BTN_CLICKED);
}

void keepmove::slot_CarIcon_Update(){
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

void keepmove::slot_CarCode_Update(){
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

void keepmove::slot_Car_Direction_Update(){
    slot_CarIcon_Update();
    slot_CarCode_Update();
    setDoorName();
}

void keepmove::slot_Door_State_Update(CAR_ID carId, DOOR_ID doorId, RUN_DOOR_STATE doorState){
//    qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@   keepmove  slot_Door_State_Update  Car Id == "<<carId<<"   Door Id == "<<doorId <<"   Door State == "<<doorState;
    KEEPMOVE_ITEM item = RUN_LEFT_DOOR;
    int realCarNo =0, realDoorNo = 0;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        if(carId <= 3){
            if(1 == (doorId % 2)){
                item = RUN_LEFT_DOOR;
                realDoorNo = 4-int(doorId)/2;
            }
            else{
                item = RUN_RIGHT_DOOR;
                realDoorNo = 5-int(doorId)/2;
            }
        }
        else{
            if(1 == (doorId % 2)){
                item = RUN_RIGHT_DOOR;
                realDoorNo = int(doorId)/2+1;
            }
            else{
                item = RUN_LEFT_DOOR;
                realDoorNo = int(doorId)/2;
            }
        }
        realCarNo = 7 - int(carId);
    }
    else{
        if(carId <= 3){
            if(1 == (doorId % 2)){
                item = RUN_RIGHT_DOOR;
                realDoorNo = int(doorId)/2+1;
            }
            else{
                item = RUN_LEFT_DOOR;
                realDoorNo = int(doorId)/2;
            }
        }
        else{
            if(1 == (doorId % 2)){
                item = RUN_LEFT_DOOR;
                realDoorNo = 4-int(doorId)/2;
            }
            else{
                item = RUN_RIGHT_DOOR;
                realDoorNo = 5-int(doorId)/2;
            }
        }
        realCarNo = int(carId);
    }
    setDoorState(item, CAR_ID(realCarNo), DOOR_ID(realDoorNo), doorState);
}

void keepmove::slot_VVVF_Elec_BCU_Update(CAR_ID carId, int vvvfIndex, int iState, int iValue){
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

void keepmove::slot_BCU_BC_Kpa_Update(CAR_ID carId, int iState, int iValue){
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

void keepmove::slot_Load_Rate_Update(CAR_ID carId, int iState, int iValue){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    setLoadRate(realCarId, RUN_RIDE_RATE_STATE(iState), iValue);
}

void keepmove::slot_ACC_Temp_Update(CAR_ID carId, int iState, int iValue1, int iValue2){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    setInsideTEMP(CAR_ID(realCarId), INSIDE_TEMP_STATE(iState), iValue1, iValue2);
}

void keepmove::slot_Urgent_Alert_Update(CAR_ID carId, bool bState){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    setUrgent_Alert(realCarId, bState);
}

void keepmove::slot_UrgentShort_Update(CAR_ID carId, bool bState){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    setUrgent_Short(realCarId, bState);
}

void keepmove::slot_BrakeIsolateDoor_Update(CAR_ID carId, bool bState){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    setBrakeIsolateDoor(realCarId, bState);
}

void keepmove::slot_Car_Arrive_Station_Update(int iFlag, int stationId){
    QLabel * tempLabel = NULL;
    switch (iFlag) {
    case 1:
        tempLabel = ui->labelcurrentstation;
        break;
    case 2:
        tempLabel = ui->labelnextstation;
        break;
    case 3:
        tempLabel = ui->labelendstation;
        break;
    default:
        tempLabel = NULL;
        break;
    }
    if(NULL == tempLabel){
        return ;
    }
//    qDebug()<<"Keepmove Screen   @@@@   Station Falg ==  "<<iFlag<<"     Code  == "<<stationId;
    QString strStationName = canSignalsSlots->getCarStationName(stationId);
//    qDebug()<<"Keepmove Screen   ########   Station Name ==  "<<strStationName;
    tempLabel->setText(strStationName);
    tempLabel = NULL;
}
