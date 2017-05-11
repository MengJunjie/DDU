#include "doorcheck.h"
#include "ui_doorcheck.h"
#include "../objectdictdata.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

doorcheck::doorcheck(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::doorcheck)
{
    ui->setupUi(this);

    Init();
    slot_CarIcon_Update();
    slot_CarCode_Update();

    setDoorName();
    labelListInit();

    iLeftOperationFlag = 0;
    iRightOperationFlag = 0;
    bLeftJudgeFlag = false;
    bLeftTimerOutFlag = false;
    bRightJudgeFlag = false;
    bRightTimerOutFlag = false;

    for(int i = 0; i < 6; i++){
        iRightOpenResult[i] = 0;
        iRightCloseResult[i] = 0;
        iLeftOpenResult[i] = 0;
        iLeftCloseResult[i] = 0;
    }
    timerLeft = new QTimer();
    connect(timerLeft, SIGNAL(timeout()), this, SLOT(slot_TimerLeft_Out()));
    timerRight = new QTimer();
    connect(timerRight, SIGNAL(timeout()), this, SLOT(slot_TimerRight_Out()));

    connect(canSignalsSlots, SIGNAL(sig_Car_Code_Change()), this, SLOT(slot_CarCode_Update()));
    connect(canSignalsSlots, SIGNAL(sig_Car_Direction_Change()), this, SLOT(slot_Car_Direction_Update()));
    connect(canSignalsSlots, SIGNAL(sig_Door_State_Change(CAR_ID,DOOR_ID,RUN_DOOR_STATE)), this, SLOT(slot_Door_State_Update(CAR_ID,DOOR_ID,RUN_DOOR_STATE)));
    connect(canSignalsSlots, SIGNAL(sig_Car_DoorOperation_Update(int,int)), this, SLOT(slot_Car_DoorOperation_Update(int,int)));

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));
}

doorcheck::~doorcheck(){
    delete ui;
}

void  doorcheck::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

//    ui->labelarrow->setScaledContents(true);
    ui->carimagelabel1->setScaledContents(true);
    ui->carimagelabel2->setScaledContents(true);
    ui->carimagelabel3->setScaledContents(true);
    ui->carimagelabel4->setScaledContents(true);
    ui->carimagelabel5->setScaledContents(true);
    ui->carimagelabel6->setScaledContents(true);

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
    for(int i = 0; i < 8; i++){
        slot_Door_State_Update(CAR_1, DOOR_ID(i+1), RUN_DOOR_STATE(canSignalsSlots->doorMC1State[i]));
        slot_Door_State_Update(CAR_2, DOOR_ID(i+1), RUN_DOOR_STATE(canSignalsSlots->doorTState[i]));
        slot_Door_State_Update(CAR_3, DOOR_ID(i+1), RUN_DOOR_STATE(canSignalsSlots->doorMState[i]));
        slot_Door_State_Update(CAR_4, DOOR_ID(i+1), RUN_DOOR_STATE(canSignalsSlots->doorT1State[i]));
        slot_Door_State_Update(CAR_5, DOOR_ID(i+1), RUN_DOOR_STATE(canSignalsSlots->doorT2State[i]));
        slot_Door_State_Update(CAR_6, DOOR_ID(i+1), RUN_DOOR_STATE(canSignalsSlots->doorMC2State[i]));
    }
}

void doorcheck::setDoorName(){
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        ui->labelinfo_3_2_1_1->setText("A64");
        ui->labelinfo_3_2_2_1->setText("A63");
        ui->labelinfo_3_2_3_1->setText("A62");
        ui->labelinfo_3_2_4_1->setText("A61");
        ui->labelinfo_3_3_1_1->setText("A54");
        ui->labelinfo_3_3_2_1->setText("A53");
        ui->labelinfo_3_3_3_1->setText("A52");
        ui->labelinfo_3_3_4_1->setText("A51");
        ui->labelinfo_3_4_1_1->setText("A44");
        ui->labelinfo_3_4_2_1->setText("A43");
        ui->labelinfo_3_4_3_1->setText("A42");
        ui->labelinfo_3_4_4_1->setText("A41");
        ui->labelinfo_3_5_1_1->setText("A34");
        ui->labelinfo_3_5_2_1->setText("A33");
        ui->labelinfo_3_5_3_1->setText("A32");
        ui->labelinfo_3_5_4_1->setText("A31");
        ui->labelinfo_3_6_1_1->setText("A24");
        ui->labelinfo_3_6_2_1->setText("A23");
        ui->labelinfo_3_6_3_1->setText("A22");
        ui->labelinfo_3_6_4_1->setText("A21");
        ui->labelinfo_3_7_1_1->setText("A14");
        ui->labelinfo_3_7_2_1->setText("A13");
        ui->labelinfo_3_7_3_1->setText("A12");
        ui->labelinfo_3_7_4_1->setText("A11");

        ui->labelinfo_7_2_1_1->setText("B64");
        ui->labelinfo_7_2_2_1->setText("B63");
        ui->labelinfo_7_2_3_1->setText("B62");
        ui->labelinfo_7_2_4_1->setText("B61");
        ui->labelinfo_7_3_1_1->setText("B54");
        ui->labelinfo_7_3_2_1->setText("B53");
        ui->labelinfo_7_3_3_1->setText("B52");
        ui->labelinfo_7_3_4_1->setText("B51");
        ui->labelinfo_7_4_1_1->setText("B44");
        ui->labelinfo_7_4_2_1->setText("B43");
        ui->labelinfo_7_4_3_1->setText("B42");
        ui->labelinfo_7_4_4_1->setText("B41");
        ui->labelinfo_7_5_1_1->setText("B34");
        ui->labelinfo_7_5_2_1->setText("B33");
        ui->labelinfo_7_5_3_1->setText("B32");
        ui->labelinfo_7_5_4_1->setText("B31");
        ui->labelinfo_7_6_1_1->setText("B24");
        ui->labelinfo_7_6_2_1->setText("B23");
        ui->labelinfo_7_6_3_1->setText("B22");
        ui->labelinfo_7_6_4_1->setText("B21");
        ui->labelinfo_7_7_1_1->setText("B14");
        ui->labelinfo_7_7_2_1->setText("B13");
        ui->labelinfo_7_7_3_1->setText("B12");
        ui->labelinfo_7_7_4_1->setText("B11");
    }
    else {
        ui->labelinfo_3_2_1_1->setText("B11");
        ui->labelinfo_3_2_2_1->setText("B12");
        ui->labelinfo_3_2_3_1->setText("B13");
        ui->labelinfo_3_2_4_1->setText("B14");
        ui->labelinfo_3_3_1_1->setText("B21");
        ui->labelinfo_3_3_2_1->setText("B22");
        ui->labelinfo_3_3_3_1->setText("B23");
        ui->labelinfo_3_3_4_1->setText("B24");
        ui->labelinfo_3_4_1_1->setText("B31");
        ui->labelinfo_3_4_2_1->setText("B32");
        ui->labelinfo_3_4_3_1->setText("B33");
        ui->labelinfo_3_4_4_1->setText("B34");
        ui->labelinfo_3_5_1_1->setText("B41");
        ui->labelinfo_3_5_2_1->setText("B42");
        ui->labelinfo_3_5_3_1->setText("B43");
        ui->labelinfo_3_5_4_1->setText("B44");
        ui->labelinfo_3_6_1_1->setText("B51");
        ui->labelinfo_3_6_2_1->setText("B52");
        ui->labelinfo_3_6_3_1->setText("B53");
        ui->labelinfo_3_6_4_1->setText("B54");
        ui->labelinfo_3_7_1_1->setText("B61");
        ui->labelinfo_3_7_2_1->setText("B62");
        ui->labelinfo_3_7_3_1->setText("B63");
        ui->labelinfo_3_7_4_1->setText("B64");

        ui->labelinfo_7_2_1_1->setText("A11");
        ui->labelinfo_7_2_2_1->setText("A12");
        ui->labelinfo_7_2_3_1->setText("A13");
        ui->labelinfo_7_2_4_1->setText("A14");
        ui->labelinfo_7_3_1_1->setText("A21");
        ui->labelinfo_7_3_2_1->setText("A22");
        ui->labelinfo_7_3_3_1->setText("A23");
        ui->labelinfo_7_3_4_1->setText("A24");
        ui->labelinfo_7_4_1_1->setText("A31");
        ui->labelinfo_7_4_2_1->setText("A32");
        ui->labelinfo_7_4_3_1->setText("A33");
        ui->labelinfo_7_4_4_1->setText("A34");
        ui->labelinfo_7_5_1_1->setText("A41");
        ui->labelinfo_7_5_2_1->setText("A42");
        ui->labelinfo_7_5_3_1->setText("A43");
        ui->labelinfo_7_5_4_1->setText("A44");
        ui->labelinfo_7_6_1_1->setText("A51");
        ui->labelinfo_7_6_2_1->setText("A52");
        ui->labelinfo_7_6_3_1->setText("A53");
        ui->labelinfo_7_6_4_1->setText("A54");
        ui->labelinfo_7_7_1_1->setText("A61");
        ui->labelinfo_7_7_2_1->setText("A62");
        ui->labelinfo_7_7_3_1->setText("A63");
        ui->labelinfo_7_7_4_1->setText("A64");
    }
}

void doorcheck::setDoorCheckCMD(DOORCHECK_ITEM item, DOORCHECK_CMD_TYPE cmdType){
    QString strStyleSheet = "";
    switch (cmdType) {
    case CMD_TYPE_0:
        strStyleSheet = QSS_BG_BLACK_BR_NONE_WD_WHITE;
        break;
    case CMD_OPEN_DOOR:
        strStyleSheet = QSS_BG_GREEN_BR_NONE_WD_BLACK;
        break;
    case CMD_CLOSE_DOOR:
        strStyleSheet = QSS_BG_YELLOW_BR_NONE_WD_BLACK;
        break;
    default:
        strStyleSheet = "";
        break;
    }
    switch (item) {
    case DOORCHECK_CMD_RIGHT:
        for(int i = 0; i < rightLabelList.length(); i++){
            rightLabelList.at(i)->setStyleSheet(strStyleSheet);
        }
        break;
    case DOORCHECK_CMD_LEFT:
        for(int i = 0; i < leftLabelList.length(); i++){
            leftLabelList.at(i)->setStyleSheet(strStyleSheet);
        }
        break;
    default:
        break;
    }
}

void doorcheck::setDoorCheckState(DOORCHECK_ITEM item, CAR_ID carId, DOOR_ID doorId, RUN_DOOR_STATE stateType){
    QLabel * tempLabel1 = NULL;
    QLabel * tempLabel2 = NULL;
    switch (item) {
    case DOORCHECK_STATE_RIGHT:
        switch (carId) {
        case CAR_1:
            switch (doorId) {
            case DOOR_1:
                tempLabel1 = ui->labelinfo_3_2_1;
                tempLabel2 = ui->labelinfo_3_2_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_3_2_2;
                tempLabel2 = ui->labelinfo_3_2_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_3_2_3;
                tempLabel2 = ui->labelinfo_3_2_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_3_2_4;
                tempLabel2 = ui->labelinfo_3_2_4_1;
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
                tempLabel1 = ui->labelinfo_3_3_1;
                tempLabel2 = ui->labelinfo_3_3_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_3_3_2;
                tempLabel2 = ui->labelinfo_3_3_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_3_3_3;
                tempLabel2 = ui->labelinfo_3_3_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_3_3_4;
                tempLabel2 = ui->labelinfo_3_3_4_1;
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
                tempLabel1 = ui->labelinfo_3_4_1;
                tempLabel2 = ui->labelinfo_3_4_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_3_4_2;
                tempLabel2 = ui->labelinfo_3_4_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_3_4_3;
                tempLabel2 = ui->labelinfo_3_4_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_3_4_4;
                tempLabel2 = ui->labelinfo_3_4_4_1;
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
                tempLabel1 = ui->labelinfo_3_5_1;
                tempLabel2 = ui->labelinfo_3_5_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_3_5_2;
                tempLabel2 = ui->labelinfo_3_5_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_3_5_3;
                tempLabel2 = ui->labelinfo_3_5_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_3_5_4;
                tempLabel2 = ui->labelinfo_3_5_4_1;
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
                tempLabel1 = ui->labelinfo_3_6_1;
                tempLabel2 = ui->labelinfo_3_6_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_3_6_2;
                tempLabel2 = ui->labelinfo_3_6_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_3_6_3;
                tempLabel2 = ui->labelinfo_3_6_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_3_6_4;
                tempLabel2= ui->labelinfo_3_6_4_1;
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
                tempLabel1 = ui->labelinfo_3_7_1;
                tempLabel2 = ui->labelinfo_3_7_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_3_7_2;
                tempLabel2= ui->labelinfo_3_7_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_3_7_3;
                tempLabel2 = ui->labelinfo_3_7_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_3_7_4;
                tempLabel2 = ui->labelinfo_3_7_4_1;
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
    case DOORCHECK_STATE_LEFT:
        switch (carId) {
        case CAR_1:
            switch (doorId) {
            case DOOR_1:
                tempLabel1 = ui->labelinfo_7_2_1;
                tempLabel2 = ui->labelinfo_7_2_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_7_2_2;
                tempLabel2 = ui->labelinfo_7_2_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_7_2_3;
                tempLabel2 = ui->labelinfo_7_2_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_7_2_4;
                tempLabel2 = ui->labelinfo_7_2_4_1;
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
                tempLabel1 = ui->labelinfo_7_3_1;
                tempLabel2 = ui->labelinfo_7_3_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_7_3_2;
                tempLabel2 = ui->labelinfo_7_3_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_7_3_3;
                tempLabel2 = ui->labelinfo_7_3_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_7_3_4;
                tempLabel2 = ui->labelinfo_7_3_4_1;
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
                tempLabel1 = ui->labelinfo_7_4_1;
                tempLabel2 = ui->labelinfo_7_4_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_7_4_2;
                tempLabel2 = ui->labelinfo_7_4_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_7_4_3;
                tempLabel2 = ui->labelinfo_7_4_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_7_4_4;
                tempLabel2 = ui->labelinfo_7_4_4_1;
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
                tempLabel1 = ui->labelinfo_7_5_1;
                tempLabel2 = ui->labelinfo_7_5_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_7_5_2;
                tempLabel2 = ui->labelinfo_7_5_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_7_5_3;
                tempLabel2 = ui->labelinfo_7_5_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_7_5_4;
                tempLabel2 = ui->labelinfo_7_5_4_1;
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
                tempLabel1 = ui->labelinfo_7_6_1;
                tempLabel2 = ui->labelinfo_7_6_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_7_6_2;
                tempLabel2 = ui->labelinfo_7_6_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_7_6_3;
                tempLabel2 = ui->labelinfo_7_6_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_7_6_4;
                tempLabel2 = ui->labelinfo_7_6_4_1;
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
                tempLabel1 = ui->labelinfo_7_7_1;
                tempLabel2 = ui->labelinfo_7_7_1_1;
                break;
            case DOOR_2:
                tempLabel1 = ui->labelinfo_7_7_2;
                tempLabel2 = ui->labelinfo_7_7_2_1;
                break;
            case DOOR_3:
                tempLabel1 = ui->labelinfo_7_7_3;
                tempLabel2 = ui->labelinfo_7_7_3_1;
                break;
            case DOOR_4:
                tempLabel1 = ui->labelinfo_7_7_4;
                tempLabel2 = ui->labelinfo_7_7_4_1;
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
    switch (stateType) {
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

void doorcheck::setDoorCheckResult(DOORCHECK_ITEM item, CAR_ID carId, DOORCHECK_RESULT_TYPE type){
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        carId = CAR_ID(7-carId);
    }
    QLabel * tempLabel = NULL;
    switch (item) {
    case DOORCHECK_OPENED_RIGHT:
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
    case DOORCHECK_CLOSED_RIGHT:
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
    case DOORCHECK_OPENED_LEFT:
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
        break;
    case DOORCHECK_CLOSED_LEFT:
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
        break;
    default:
        tempLabel = NULL;
        break;
    }
    if(NULL == tempLabel){
        return ;
    }
    QString strText = "", strStyleSheet = "";
    switch (type) {
    case DOOR_RESULT_STANDBY:
        strText = "";
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        break;
    case DOOR_RESULT_JUDGING:
        strText = "判断中";
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        break;
    case DOOR_RESULT_OK:
        strText = "OK";
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        break;
    case DOOR_RESULT_NG:
        strText = "NG";
        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
        break;
    default:
        strText = "";
        strStyleSheet = "";
        break;
    }

    tempLabel->setText(strText);
    tempLabel->setStyleSheet(strStyleSheet);
}

void doorcheck::labelListInit(){
    qDeleteAll(rightLabelList);
    rightLabelList.clear();

    qDeleteAll(leftLabelList);
    leftLabelList.clear();

    rightLabelList << ui->labelinfo_2_2_1 << ui->labelinfo_2_3_1 << ui->labelinfo_2_4_1 << ui->labelinfo_2_5_1 << ui->labelinfo_2_6_1 << ui->labelinfo_2_7_1;
    leftLabelList << ui->labelinfo_6_2_1 << ui->labelinfo_6_3_1 << ui->labelinfo_6_4_1 << ui->labelinfo_6_5_1 << ui->labelinfo_6_6_1 << ui->labelinfo_6_7_1;
}

void doorcheck::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void doorcheck::on_buttonrecord_clicked(){
    buttonBeep();

    for(int i = 0; i < 6; i++){
        DoorCheck_Data[i] = iRightOpenResult[i] *16 + iRightCloseResult[i];
        DoorCheck_Data[i+6] = iLeftOpenResult[i] * 16 + iLeftCloseResult[i];
    }
    canSignalsSlots->sendSDOCheckData(1);
}

void doorcheck::on_buttonquit_clicked(){
    buttonBeep();
    this->reject();
}

void doorcheck::slot_CarIcon_Update(){
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

void doorcheck::slot_CarCode_Update(){
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

void doorcheck::slot_Car_Direction_Update(){
    slot_CarIcon_Update();
    slot_CarCode_Update();
    setDoorName();
    for(int i = 0; i < 6; i++){
        setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_ID(i+1), DOORCHECK_RESULT_TYPE(iRightCloseResult[i]));
        setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_ID(i+1), DOORCHECK_RESULT_TYPE(iRightOpenResult[i]));
        setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_ID(i+1), DOORCHECK_RESULT_TYPE(iLeftCloseResult[i]));
        setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_ID(i+1), DOORCHECK_RESULT_TYPE(iLeftOpenResult[i]));
    }
}

void doorcheck::slot_Door_State_Update(CAR_ID carId, DOOR_ID doorId, RUN_DOOR_STATE doorState){
    DOORCHECK_ITEM item = DOORCHECK_NONE;
    int realCarNo =0, realDoorNo = 0;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        if(carId <= 3){
            if(1 == (doorId % 2)){
                item = DOORCHECK_STATE_LEFT;
                realDoorNo = 4-int(doorId)/2;
            }
            else{
                item = DOORCHECK_STATE_RIGHT;
                realDoorNo = 5-int(doorId)/2;
            }
        }
        else{
            if(1 == (doorId % 2)){
                item = DOORCHECK_STATE_RIGHT;
                realDoorNo = int(doorId)/2+1;
            }
            else{
                item = DOORCHECK_STATE_LEFT;
                realDoorNo = int(doorId)/2;
            }
        }
        realCarNo = 7 - int(carId);
    }
    else{
        if(carId <= 3){
            if(1 == (doorId % 2)){
                item = DOORCHECK_STATE_RIGHT;
                realDoorNo = int(doorId)/2+1;
            }
            else{
                item = DOORCHECK_STATE_LEFT;
                realDoorNo = int(doorId)/2;
            }
        }
        else{
            if(1 == (doorId % 2)){
                item = DOORCHECK_STATE_LEFT;
                realDoorNo = 4-int(doorId)/2;
            }
            else{
                item = DOORCHECK_STATE_RIGHT;
                realDoorNo = 5-int(doorId)/2;
            }
        }
        realCarNo = int(carId);
    }
    if(DOORCHECK_NONE == item){
        return;
    }
    setDoorCheckState(item, CAR_ID(realCarNo), DOOR_ID(realDoorNo), doorState);
}

void doorcheck::slot_Car_DoorOperation_Update(int doorFlag, int operation){
    DOORCHECK_ITEM item = DOORCHECK_NONE;
    DOORCHECK_CMD_TYPE type = CMD_TYPE_0;
    if(1 == doorFlag){   //左侧门
        if(!bLeftJudgeFlag){
            item = DOORCHECK_CMD_LEFT;
            if(1 == operation){
                type = CMD_OPEN_DOOR;
                iLeftOperationFlag = 1;
                for(int i = 0; i < 6; i++){
                    iLeftOpenResult[i] = 1;
                    setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_ID(i+1), DOOR_RESULT_JUDGING);
                }
            }
            else{
                type = CMD_CLOSE_DOOR;
                iLeftOperationFlag = 2;
                for(int i = 0; i < 6; i++){
                    iLeftCloseResult[i] = 1;
                    setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_ID(i+1), DOOR_RESULT_JUDGING);
                }
            }
            setDoorCheckCMD(item, type);
            bLeftJudgeFlag = true;
            timerLeft->start(DOORCHECK_TIMEOUT);
        }
    }
    else if(2 == doorFlag){   //右侧门
        if(!bRightJudgeFlag){
            item = DOORCHECK_CMD_RIGHT;
            if(1 == operation){
                type = CMD_OPEN_DOOR;
                iRightOperationFlag = 1;
                for(int i = 0; i < 6; i++){
                    iRightOpenResult[i] = 1;
                    setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_ID(i+1), DOOR_RESULT_JUDGING);
                }
            }
            else{
                type = CMD_CLOSE_DOOR;
                iRightOperationFlag = 2;
                for(int i = 0; i < 6; i++){
                    iRightCloseResult[i] = 1;
                    setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_ID(i+1), DOOR_RESULT_JUDGING);
                }
            }
            setDoorCheckCMD(item, type);
            bRightJudgeFlag = true;
            timerRight->start(DOORCHECK_TIMEOUT);
        }
    }
    else{
        //do nothing
    }
}

void doorcheck::slot_TimerLeft_Out(){
    timerLeft->stop();
    bLeftJudgeFlag = false;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        if(1 == iLeftOperationFlag){
            if(5 == canSignalsSlots->doorMC1State[0] && 5 == canSignalsSlots->doorMC1State[2] && 5 == canSignalsSlots->doorMC1State[4] && 5 == canSignalsSlots->doorMC1State[6]){
                iLeftOpenResult[0] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_1, DOOR_RESULT_OK);
            }
            else{
                iLeftOpenResult[0] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_1, DOOR_RESULT_NG);
            }
            if(5 == canSignalsSlots->doorTState[0] && 5 == canSignalsSlots->doorTState[2] && 5 == canSignalsSlots->doorTState[4] && 5 == canSignalsSlots->doorTState[6]){
                iLeftOpenResult[1] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_2, DOOR_RESULT_OK);
            }
            else{
                iLeftOpenResult[1] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_2, DOOR_RESULT_NG);
            }
            if(5 == canSignalsSlots->doorMState[0] && 5 == canSignalsSlots->doorMState[2] && 5 == canSignalsSlots->doorMState[4] && 5 == canSignalsSlots->doorMState[6]){
                iLeftOpenResult[2] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_3, DOOR_RESULT_OK);
            }
            else{
                iLeftOpenResult[2] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_3, DOOR_RESULT_NG);
            }
            if(5 == canSignalsSlots->doorT1State[1] && 5 == canSignalsSlots->doorT1State[3] && 5 == canSignalsSlots->doorT1State[5] && 5 == canSignalsSlots->doorT1State[7]){
                iLeftOpenResult[3] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_4, DOOR_RESULT_OK);
            }
            else{
                iLeftOpenResult[3] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_4, DOOR_RESULT_NG);
            }
            if(5 == canSignalsSlots->doorT2State[1] && 5 == canSignalsSlots->doorT2State[3] && 5 == canSignalsSlots->doorT2State[5] && 5 == canSignalsSlots->doorT2State[7]){
                iLeftOpenResult[4] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_5, DOOR_RESULT_OK);
            }
            else{
                iLeftOpenResult[4] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_5, DOOR_RESULT_NG);
            }
            if(5 == canSignalsSlots->doorMC2State[1] && 5 == canSignalsSlots->doorMC2State[3] && 5 == canSignalsSlots->doorMC2State[5] && 5 == canSignalsSlots->doorMC2State[7]){
                iLeftOpenResult[5] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_6, DOOR_RESULT_OK);
            }
            else{
                iLeftOpenResult[5] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_6, DOOR_RESULT_NG);
            }
        }
        else if(2 == iLeftOperationFlag){
            if(7 == canSignalsSlots->doorMC1State[0] && 7 == canSignalsSlots->doorMC1State[2] && 7 == canSignalsSlots->doorMC1State[4] && 7 == canSignalsSlots->doorMC1State[6]){
                iLeftCloseResult[0] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_1, DOOR_RESULT_OK);
            }
            else{
                iLeftCloseResult[0] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_1, DOOR_RESULT_NG);
            }
            if(7 == canSignalsSlots->doorTState[0] && 7 == canSignalsSlots->doorTState[2] && 7 == canSignalsSlots->doorTState[4] && 7 == canSignalsSlots->doorTState[6]){
                iLeftCloseResult[1] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_2, DOOR_RESULT_OK);
            }
            else{
                iLeftCloseResult[1] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_2, DOOR_RESULT_NG);
            }
            if(7 == canSignalsSlots->doorMState[0] && 7 == canSignalsSlots->doorMState[2] && 7 == canSignalsSlots->doorMState[4] && 7 == canSignalsSlots->doorMState[6]){
                iLeftCloseResult[2] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_3, DOOR_RESULT_OK);
            }
            else{
                iLeftCloseResult[2] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_3, DOOR_RESULT_NG);
            }
            if(7 == canSignalsSlots->doorT1State[1] && 7 == canSignalsSlots->doorT1State[3] && 7 == canSignalsSlots->doorT1State[5] && 7 == canSignalsSlots->doorT1State[7]){
                iLeftCloseResult[3] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_4, DOOR_RESULT_OK);
            }
            else{
                iLeftCloseResult[3] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_4, DOOR_RESULT_NG);
            }
            if(7 == canSignalsSlots->doorT2State[1] && 7 == canSignalsSlots->doorT2State[3] && 7 == canSignalsSlots->doorT2State[5] && 7 == canSignalsSlots->doorT2State[7]){
                iLeftCloseResult[4] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_5, DOOR_RESULT_OK);
            }
            else{
                iLeftCloseResult[4] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_5, DOOR_RESULT_NG);
            }
            if(7 == canSignalsSlots->doorMC2State[1] && 7 == canSignalsSlots->doorMC2State[3] && 7 == canSignalsSlots->doorMC2State[5] && 7 == canSignalsSlots->doorMC2State[7]){
                iLeftCloseResult[5] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_6, DOOR_RESULT_OK);
            }
            else{
                iLeftCloseResult[5] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_6, DOOR_RESULT_NG);
            }
        }
        else{
            return ;
        }
    }
    else{
        if(1 == iLeftOperationFlag){
            if(5 == canSignalsSlots->doorMC1State[1] && 5 == canSignalsSlots->doorMC1State[3] && 5 == canSignalsSlots->doorMC1State[5] && 5 == canSignalsSlots->doorMC1State[7]){
                iLeftOpenResult[0] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_1, DOOR_RESULT_OK);
            }
            else{
                iLeftOpenResult[0] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_1, DOOR_RESULT_NG);
            }
            if(5 == canSignalsSlots->doorTState[1] && 5 == canSignalsSlots->doorTState[3] && 5 == canSignalsSlots->doorTState[5] && 5 == canSignalsSlots->doorTState[7]){
                iLeftOpenResult[1] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_2, DOOR_RESULT_OK);
            }
            else{
                iLeftOpenResult[1] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_2, DOOR_RESULT_NG);
            }
            if(5 == canSignalsSlots->doorMState[1] && 5 == canSignalsSlots->doorMState[3] && 5 == canSignalsSlots->doorMState[5] && 5 == canSignalsSlots->doorMState[7]){
                iLeftOpenResult[2] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_3, DOOR_RESULT_OK);
            }
            else{
                iLeftOpenResult[2] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_3, DOOR_RESULT_NG);
            }
            if(5 == canSignalsSlots->doorT1State[0] && 5 == canSignalsSlots->doorT1State[2] && 5 == canSignalsSlots->doorT1State[4] && 5 == canSignalsSlots->doorT1State[6]){
                iLeftOpenResult[3] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_4, DOOR_RESULT_OK);
            }
            else{
                iLeftOpenResult[3] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_4, DOOR_RESULT_NG);
            }
            if(5 == canSignalsSlots->doorT2State[0] && 5 == canSignalsSlots->doorT2State[2] && 5 == canSignalsSlots->doorT2State[4] && 5 == canSignalsSlots->doorT2State[6]){
                iLeftOpenResult[4] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_5, DOOR_RESULT_OK);
            }
            else{
                iLeftOpenResult[4] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_5, DOOR_RESULT_NG);
            }
            if(5 == canSignalsSlots->doorMC2State[0] && 5 == canSignalsSlots->doorMC2State[2] && 5 == canSignalsSlots->doorMC2State[4] && 5 == canSignalsSlots->doorMC2State[6]){
                iLeftOpenResult[5] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_6, DOOR_RESULT_OK);
            }
            else{
                iLeftOpenResult[5] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_LEFT, CAR_6, DOOR_RESULT_NG);
            }
        }
        else if(2 == iLeftOperationFlag){
            if(7 == canSignalsSlots->doorMC1State[1] && 7 == canSignalsSlots->doorMC1State[3] && 7 == canSignalsSlots->doorMC1State[5] && 7 == canSignalsSlots->doorMC1State[7]){
                iLeftCloseResult[0] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_1, DOOR_RESULT_OK);
            }
            else{
                iLeftCloseResult[0] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_1, DOOR_RESULT_NG);
            }
            if(7 == canSignalsSlots->doorTState[1] && 7 == canSignalsSlots->doorTState[3] && 7 == canSignalsSlots->doorTState[5] && 7 == canSignalsSlots->doorTState[7]){
                iLeftCloseResult[1] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_2, DOOR_RESULT_OK);
            }
            else{
                iLeftCloseResult[1] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_2, DOOR_RESULT_NG);
            }
            if(7 == canSignalsSlots->doorMState[1] && 7 == canSignalsSlots->doorMState[3] && 7 == canSignalsSlots->doorMState[5] && 7 == canSignalsSlots->doorMState[7]){
                iLeftCloseResult[2] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_3, DOOR_RESULT_OK);
            }
            else{
                iLeftCloseResult[2] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_3, DOOR_RESULT_NG);
            }
            if(7 == canSignalsSlots->doorT1State[0] && 7 == canSignalsSlots->doorT1State[2] && 7 == canSignalsSlots->doorT1State[4] && 7 == canSignalsSlots->doorT1State[6]){
                iLeftCloseResult[3] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_4, DOOR_RESULT_OK);
            }
            else{
                iLeftCloseResult[3] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_4, DOOR_RESULT_NG);
            }
            if(7 == canSignalsSlots->doorT2State[0] && 7 == canSignalsSlots->doorT2State[2] && 7 == canSignalsSlots->doorT2State[4] && 7 == canSignalsSlots->doorT2State[6]){
                iLeftCloseResult[4] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_5, DOOR_RESULT_OK);
            }
            else{
                iLeftCloseResult[4] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_5, DOOR_RESULT_NG);
            }
            if(7 == canSignalsSlots->doorMC2State[0] && 7 == canSignalsSlots->doorMC2State[2] && 7 == canSignalsSlots->doorMC2State[4] && 7 == canSignalsSlots->doorMC2State[6]){
                iLeftCloseResult[5] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_6, DOOR_RESULT_OK);
            }
            else{
                iLeftCloseResult[5] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_LEFT, CAR_6, DOOR_RESULT_NG);
            }
        }
        else{
            return ;
        }
    }
}

void doorcheck::slot_TimerRight_Out(){
    timerRight->stop();
    bRightJudgeFlag = false;

    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        if(1 == iRightOperationFlag){
            if(5 == canSignalsSlots->doorMC1State[1] && 5 == canSignalsSlots->doorMC1State[3] && 5 == canSignalsSlots->doorMC1State[5] && 5 == canSignalsSlots->doorMC1State[7]){
                iRightOpenResult[0] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_1, DOOR_RESULT_OK);
            }
            else{
                iRightOpenResult[0] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_1, DOOR_RESULT_NG);
            }
            if(5 == canSignalsSlots->doorTState[1] && 5 == canSignalsSlots->doorTState[3] && 5 == canSignalsSlots->doorTState[5] && 5 == canSignalsSlots->doorTState[7]){
                iRightOpenResult[1] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_2, DOOR_RESULT_OK);
            }
            else{
                iRightOpenResult[1] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_2, DOOR_RESULT_NG);
            }
            if(5 == canSignalsSlots->doorMState[1] && 5 == canSignalsSlots->doorMState[3] && 5 == canSignalsSlots->doorMState[5] && 5 == canSignalsSlots->doorMState[7]){
                iRightOpenResult[2] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_3, DOOR_RESULT_OK);
            }
            else{
                iRightOpenResult[2] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_3, DOOR_RESULT_NG);
            }
            if(5 == canSignalsSlots->doorT1State[0] && 5 == canSignalsSlots->doorT1State[2] && 5 == canSignalsSlots->doorT1State[4] && 5 == canSignalsSlots->doorT1State[6]){
                iRightOpenResult[3] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_4, DOOR_RESULT_OK);
            }
            else{
                iRightOpenResult[3] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_4, DOOR_RESULT_NG);
            }
            if(5 == canSignalsSlots->doorT2State[0] && 5 == canSignalsSlots->doorT2State[2] && 5 == canSignalsSlots->doorT2State[4] && 5 == canSignalsSlots->doorT2State[6]){
                iRightOpenResult[4] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_5, DOOR_RESULT_OK);
            }
            else{
                iRightOpenResult[4] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_5, DOOR_RESULT_NG);
            }
            if(5 == canSignalsSlots->doorMC2State[0] && 5 == canSignalsSlots->doorMC2State[2] && 5 == canSignalsSlots->doorMC2State[4] && 5 == canSignalsSlots->doorMC2State[6]){
                iRightOpenResult[5] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_6, DOOR_RESULT_OK);
            }
            else{
                iRightOpenResult[5] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_6, DOOR_RESULT_NG);
            }
        }
        else if(2 == iRightOperationFlag){
            if(7 == canSignalsSlots->doorMC1State[1] && 7 == canSignalsSlots->doorMC1State[3] && 7 == canSignalsSlots->doorMC1State[5] && 7 == canSignalsSlots->doorMC1State[7]){
                iRightCloseResult[0] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_1, DOOR_RESULT_OK);
            }
            else{
                iRightCloseResult[0] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_1, DOOR_RESULT_NG);
            }
            if(7 == canSignalsSlots->doorTState[1] && 7 == canSignalsSlots->doorTState[3] && 7 == canSignalsSlots->doorTState[5] && 7 == canSignalsSlots->doorTState[7]){
                iRightCloseResult[1] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_2, DOOR_RESULT_OK);
            }
            else{
                iRightCloseResult[1] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_2, DOOR_RESULT_NG);
            }
            if(7 == canSignalsSlots->doorMState[1] && 7 == canSignalsSlots->doorMState[3] && 7 == canSignalsSlots->doorMState[5] && 7 == canSignalsSlots->doorMState[7]){
                iRightCloseResult[2] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_3, DOOR_RESULT_OK);
            }
            else{
                iRightCloseResult[2] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_3, DOOR_RESULT_NG);
            }
            if(7 == canSignalsSlots->doorT1State[0] && 7 == canSignalsSlots->doorT1State[2] && 7 == canSignalsSlots->doorT1State[4] && 7 == canSignalsSlots->doorT1State[6]){
                iRightCloseResult[3] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_4, DOOR_RESULT_OK);
            }
            else{
                iRightCloseResult[3] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_4, DOOR_RESULT_NG);
            }
            if(7 == canSignalsSlots->doorT2State[0] && 7 == canSignalsSlots->doorT2State[2] && 7 == canSignalsSlots->doorT2State[4] && 7 == canSignalsSlots->doorT2State[6]){
                iRightCloseResult[4] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_5, DOOR_RESULT_OK);
            }
            else{
                iRightCloseResult[4] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_5, DOOR_RESULT_NG);
            }
            if(7 == canSignalsSlots->doorMC2State[0] && 7 == canSignalsSlots->doorMC2State[2] && 7 == canSignalsSlots->doorMC2State[4] && 7 == canSignalsSlots->doorMC2State[6]){
                iRightCloseResult[5] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_6, DOOR_RESULT_OK);
            }
            else{
                iRightCloseResult[5] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_6, DOOR_RESULT_NG);
            }
        }
        else {
            return ;
        }
    }
    else{
        if(1 == iRightOperationFlag){
            if(5 == canSignalsSlots->doorMC1State[0] && 5 == canSignalsSlots->doorMC1State[2] && 5 == canSignalsSlots->doorMC1State[4] && 5 == canSignalsSlots->doorMC1State[6]){
                iRightOpenResult[0] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_1, DOOR_RESULT_OK);
            }
            else{
                iRightOpenResult[0] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_1, DOOR_RESULT_NG);
            }
            if(5 == canSignalsSlots->doorTState[0] && 5 == canSignalsSlots->doorTState[2] && 5 == canSignalsSlots->doorTState[4] && 5 == canSignalsSlots->doorTState[6]){
                iRightOpenResult[1] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_2, DOOR_RESULT_OK);
            }
            else{
                iRightOpenResult[1] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_2, DOOR_RESULT_NG);
            }
            if(5 == canSignalsSlots->doorMState[0] && 5 == canSignalsSlots->doorMState[2] && 5 == canSignalsSlots->doorMState[4] && 5 == canSignalsSlots->doorMState[6]){
                iRightOpenResult[2] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_3, DOOR_RESULT_OK);
            }
            else{
                iRightOpenResult[2] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_3, DOOR_RESULT_NG);
            }
            if(5 == canSignalsSlots->doorT1State[1] && 5 == canSignalsSlots->doorT1State[3] && 5 == canSignalsSlots->doorT1State[5] && 5 == canSignalsSlots->doorT1State[7]){
                iRightOpenResult[3] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_4, DOOR_RESULT_OK);
            }
            else{
                iRightOpenResult[3] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_4, DOOR_RESULT_NG);
            }
            if(5 == canSignalsSlots->doorT2State[1] && 5 == canSignalsSlots->doorT2State[3] && 5 == canSignalsSlots->doorT2State[5] && 5 == canSignalsSlots->doorT2State[7]){
                iRightOpenResult[4] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_5, DOOR_RESULT_OK);
            }
            else{
                iRightOpenResult[4] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_5, DOOR_RESULT_NG);
            }
            if(5 == canSignalsSlots->doorMC2State[1] && 5 == canSignalsSlots->doorMC2State[3] && 5 == canSignalsSlots->doorMC2State[5] && 5 == canSignalsSlots->doorMC2State[7]){
                iRightOpenResult[5] = 2;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_6, DOOR_RESULT_OK);
            }
            else{
                iRightOpenResult[5] = 3;
                setDoorCheckResult(DOORCHECK_OPENED_RIGHT, CAR_6, DOOR_RESULT_NG);
            }
        }
        else if(2 == iRightOperationFlag){
            if(7 == canSignalsSlots->doorMC1State[0] && 7 == canSignalsSlots->doorMC1State[2] && 7 == canSignalsSlots->doorMC1State[4] && 7 == canSignalsSlots->doorMC1State[6]){
                iRightCloseResult[0] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_1, DOOR_RESULT_OK);
            }
            else{
                iRightCloseResult[0] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_1, DOOR_RESULT_NG);
            }
            if(7 == canSignalsSlots->doorTState[0] && 7 == canSignalsSlots->doorTState[2] && 7 == canSignalsSlots->doorTState[4] && 7 == canSignalsSlots->doorTState[6]){
                iRightCloseResult[1] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_2, DOOR_RESULT_OK);
            }
            else{
                iRightCloseResult[1] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_2, DOOR_RESULT_NG);
            }
            if(7 == canSignalsSlots->doorMState[0] && 7 == canSignalsSlots->doorMState[2] && 7 == canSignalsSlots->doorMState[4] && 7 == canSignalsSlots->doorMState[6]){
                iRightCloseResult[2] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_3, DOOR_RESULT_OK);
            }
            else{
                iRightCloseResult[2] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_3, DOOR_RESULT_NG);
            }
            if(7 == canSignalsSlots->doorT1State[1] && 7 == canSignalsSlots->doorT1State[3] && 7 == canSignalsSlots->doorT1State[5] && 7 == canSignalsSlots->doorT1State[7]){
                iRightCloseResult[3] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_4, DOOR_RESULT_OK);
            }
            else{
                iRightCloseResult[3] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_4, DOOR_RESULT_NG);
            }
            if(7 == canSignalsSlots->doorT2State[1] && 7 == canSignalsSlots->doorT2State[3] && 7 == canSignalsSlots->doorT2State[5] && 7 == canSignalsSlots->doorT2State[7]){
                iRightCloseResult[4] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_5, DOOR_RESULT_OK);
            }
            else{
                iRightCloseResult[4] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_5, DOOR_RESULT_NG);
            }
            if(7 == canSignalsSlots->doorMC2State[1] && 7 == canSignalsSlots->doorMC2State[3] && 7 == canSignalsSlots->doorMC2State[5] && 7 == canSignalsSlots->doorMC2State[7]){
                iRightCloseResult[5] = 2;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_6, DOOR_RESULT_OK);
            }
            else{
                iRightCloseResult[5] = 3;
                setDoorCheckResult(DOORCHECK_CLOSED_RIGHT, CAR_6, DOOR_RESULT_NG);
            }
        }
        else{
            return ;
        }
    }
}
