#include "errinfo.h"
#include "ui_errinfo.h"
//#include "viewerrlist1.h"
//#include "viewerrlist2.h"
#include "errdetail.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

errinfo::errinfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::errinfo)
{
    ui->setupUi(this);

    bErrorHappenedFlag = false;
    m_VVVFErrList.clear();
    m_SIVErrList.clear();
    m_BCUErrList.clear();
    m_ACCErrList.clear();
    m_DOORErrList.clear();
    m_VVVF_BCU_State = 0;
    m_VVVF_BCU_Value = 0;
    iCurrentFaltCount = 0;

    Init();
    slot_CarIcon_Update();
    slot_CarCode_Update();
    setDoorName();

    connect(ui->labelinfo_2_2, SIGNAL(clicked()), this, SLOT(slot_MyLabel_VVVF_clicked()));
    connect(ui->labelinfo_2_3, SIGNAL(clicked()), this, SLOT(slot_MyLabel_VVVF_clicked()));
    connect(ui->labelinfo_2_4, SIGNAL(clicked()), this, SLOT(slot_MyLabel_VVVF_clicked()));
    connect(ui->labelinfo_2_5, SIGNAL(clicked()), this, SLOT(slot_MyLabel_VVVF_clicked()));
    connect(ui->labelinfo_2_6, SIGNAL(clicked()), this, SLOT(slot_MyLabel_VVVF_clicked()));
    connect(ui->labelinfo_2_7, SIGNAL(clicked()), this, SLOT(slot_MyLabel_VVVF_clicked()));

    connect(ui->labelinfo_3_2, SIGNAL(clicked()), this, SLOT(slot_MyLabel_VVVF_clicked()));
    connect(ui->labelinfo_3_3, SIGNAL(clicked()), this, SLOT(slot_MyLabel_VVVF_clicked()));
    connect(ui->labelinfo_3_4, SIGNAL(clicked()), this, SLOT(slot_MyLabel_VVVF_clicked()));
    connect(ui->labelinfo_3_5, SIGNAL(clicked()), this, SLOT(slot_MyLabel_VVVF_clicked()));
    connect(ui->labelinfo_3_6, SIGNAL(clicked()), this, SLOT(slot_MyLabel_VVVF_clicked()));
    connect(ui->labelinfo_3_7, SIGNAL(clicked()), this, SLOT(slot_MyLabel_VVVF_clicked()));

    connect(ui->labelinfo_4_2, SIGNAL(clicked()), this, SLOT(slot_MyLabel_SIV_clicked()));
    connect(ui->labelinfo_4_3, SIGNAL(clicked()), this, SLOT(slot_MyLabel_SIV_clicked()));
    connect(ui->labelinfo_4_4, SIGNAL(clicked()), this, SLOT(slot_MyLabel_SIV_clicked()));
    connect(ui->labelinfo_4_5, SIGNAL(clicked()), this, SLOT(slot_MyLabel_SIV_clicked()));
    connect(ui->labelinfo_4_6, SIGNAL(clicked()), this, SLOT(slot_MyLabel_SIV_clicked()));
    connect(ui->labelinfo_4_7, SIGNAL(clicked()), this, SLOT(slot_MyLabel_SIV_clicked()));

    connect(ui->labelinfo_5_2, SIGNAL(clicked()), this, SLOT(slot_MyLabel_BCU_clicked()));
    connect(ui->labelinfo_5_3, SIGNAL(clicked()), this, SLOT(slot_MyLabel_BCU_clicked()));
    connect(ui->labelinfo_5_4, SIGNAL(clicked()), this, SLOT(slot_MyLabel_BCU_clicked()));
    connect(ui->labelinfo_5_5, SIGNAL(clicked()), this, SLOT(slot_MyLabel_BCU_clicked()));
    connect(ui->labelinfo_5_6, SIGNAL(clicked()), this, SLOT(slot_MyLabel_BCU_clicked()));
    connect(ui->labelinfo_5_7, SIGNAL(clicked()), this, SLOT(slot_MyLabel_BCU_clicked()));

    connect(ui->labelinfo_6_2, SIGNAL(clicked()), this, SLOT(slot_MyLabel_ACC_clicked()));
    connect(ui->labelinfo_6_3, SIGNAL(clicked()), this, SLOT(slot_MyLabel_ACC_clicked()));
    connect(ui->labelinfo_6_4, SIGNAL(clicked()), this, SLOT(slot_MyLabel_ACC_clicked()));
    connect(ui->labelinfo_6_5, SIGNAL(clicked()), this, SLOT(slot_MyLabel_ACC_clicked()));
    connect(ui->labelinfo_6_6, SIGNAL(clicked()), this, SLOT(slot_MyLabel_ACC_clicked()));
    connect(ui->labelinfo_6_7, SIGNAL(clicked()), this, SLOT(slot_MyLabel_ACC_clicked()));

    connect(ui->labelinfo_7_2_1, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_7_2_2, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_7_2_3, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_7_2_4, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));

    connect(ui->labelinfo_7_3_1, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_7_3_2, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_7_3_3, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_7_3_4, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));

    connect(ui->labelinfo_7_4_1, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_7_4_2, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_7_4_3, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_7_4_4, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));

    connect(ui->labelinfo_7_5_1, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_7_5_2, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_7_5_3, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_7_5_4, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));

    connect(ui->labelinfo_7_6_1, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_7_6_2, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_7_6_3, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_7_6_4, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));

    connect(ui->labelinfo_7_7_1, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_7_7_2, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_7_7_3, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_7_7_4, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));

    connect(ui->labelinfo_8_2_1, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_8_2_2, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_8_2_3, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_8_2_4, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));

    connect(ui->labelinfo_8_3_1, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_8_3_2, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_8_3_3, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_8_3_4, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));

    connect(ui->labelinfo_8_4_1, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_8_4_2, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_8_4_3, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_8_4_4, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));

    connect(ui->labelinfo_8_5_1, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_8_5_2, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_8_5_3, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_8_5_4, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));

    connect(ui->labelinfo_8_6_1, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_8_6_2, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_8_6_3, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_8_6_4, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));

    connect(ui->labelinfo_8_7_1, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_8_7_2, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_8_7_3, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));
    connect(ui->labelinfo_8_7_4, SIGNAL(clicked()), this, SLOT(slot_MyLabel_DOOR_clicked()));

    connect(canSignalsSlots, SIGNAL(sig_Car_Direction_Change()), this, SLOT(slot_Car_Direction_Update()));
    connect(canSignalsSlots, SIGNAL(sig_Car_Code_Change()), this, SLOT(slot_CarCode_Update()));
    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));
    connect(canSignalsSlots, SIGNAL(sig_VVVF_BCU_Level_Update(int,int)), this, SLOT(slot_VVVF_BCU_Level_Update(int,int)));
    connect(canSignalsSlots, SIGNAL(sig_VVVF_Elec_BCU_Update(CAR_ID,int,int,int)), this, SLOT(slot_VVVF_Elec_BCU_Update(CAR_ID,int,int,int)));
    connect(canSignalsSlots, SIGNAL(sig_SIV_Voltage_Freq_Update(CAR_ID,int,int,int)), this, SLOT(slot_SIV_Voltage_Freq_Update(CAR_ID,int,int,int)));
    connect(canSignalsSlots, SIGNAL(sig_BCU_BC_Kpa_Update(CAR_ID,int,int)), this, SLOT(slot_BCU_BC_Kpa_Update(CAR_ID,int,int)));
    connect(canSignalsSlots, SIGNAL(sig_ACC_Temp_Update(CAR_ID,int,int,int)), this, SLOT(slot_ACC_Temp_Update(CAR_ID,int,int,int)));
    connect(canSignalsSlots, SIGNAL(sig_Door_State_Change(CAR_ID,DOOR_ID,RUN_DOOR_STATE)), this, SLOT(slot_Door_State_Update(CAR_ID,DOOR_ID,RUN_DOOR_STATE)));
}

errinfo::~errinfo(){
    delete ui;
}

void errinfo::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

    //    ui->labelarrow->setScaledContents(true);
    ui->carimagelabel1->setScaledContents(true);
    ui->carimagelabel2->setScaledContents(true);
    ui->carimagelabel3->setScaledContents(true);
    ui->carimagelabel4->setScaledContents(true);
    ui->carimagelabel5->setScaledContents(true);
    ui->carimagelabel6->setScaledContents(true);

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));

    timer_5s = new QTimer(this);
    connect(timer_5s, SIGNAL(timeout()), this, SLOT(slot_5S_Timer_Out()));
}

void errinfo::setDoorName(){
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        ui->labelinfo_7_2_1->setText("A64");
        ui->labelinfo_7_2_2->setText("A63");
        ui->labelinfo_7_2_3->setText("A62");
        ui->labelinfo_7_2_4->setText("A61");
        ui->labelinfo_7_3_1->setText("A54");
        ui->labelinfo_7_3_2->setText("A53");
        ui->labelinfo_7_3_3->setText("A52");
        ui->labelinfo_7_3_4->setText("A51");
        ui->labelinfo_7_4_1->setText("A44");
        ui->labelinfo_7_4_2->setText("A43");
        ui->labelinfo_7_4_3->setText("A42");
        ui->labelinfo_7_4_4->setText("A41");
        ui->labelinfo_7_5_1->setText("A34");
        ui->labelinfo_7_5_2->setText("A33");
        ui->labelinfo_7_5_3->setText("A32");
        ui->labelinfo_7_5_4->setText("A31");
        ui->labelinfo_7_6_1->setText("A24");
        ui->labelinfo_7_6_2->setText("A23");
        ui->labelinfo_7_6_3->setText("A22");
        ui->labelinfo_7_6_4->setText("A21");
        ui->labelinfo_7_7_1->setText("A14");
        ui->labelinfo_7_7_2->setText("A13");
        ui->labelinfo_7_7_3->setText("A12");
        ui->labelinfo_7_7_4->setText("A11");

        ui->labelinfo_8_2_1->setText("B64");
        ui->labelinfo_8_2_2->setText("B63");
        ui->labelinfo_8_2_3->setText("B62");
        ui->labelinfo_8_2_4->setText("B61");
        ui->labelinfo_8_3_1->setText("B54");
        ui->labelinfo_8_3_2->setText("B53");
        ui->labelinfo_8_3_3->setText("B52");
        ui->labelinfo_8_3_4->setText("B51");
        ui->labelinfo_8_4_1->setText("B44");
        ui->labelinfo_8_4_2->setText("B43");
        ui->labelinfo_8_4_3->setText("B42");
        ui->labelinfo_8_4_4->setText("B41");
        ui->labelinfo_8_5_1->setText("B34");
        ui->labelinfo_8_5_2->setText("B33");
        ui->labelinfo_8_5_3->setText("B32");
        ui->labelinfo_8_5_4->setText("B31");
        ui->labelinfo_8_6_1->setText("B24");
        ui->labelinfo_8_6_2->setText("B23");
        ui->labelinfo_8_6_3->setText("B22");
        ui->labelinfo_8_6_4->setText("B21");
        ui->labelinfo_8_7_1->setText("B14");
        ui->labelinfo_8_7_2->setText("B13");
        ui->labelinfo_8_7_3->setText("B12");
        ui->labelinfo_8_7_4->setText("B11");
    }
    else {
        ui->labelinfo_7_2_1->setText("B11");
        ui->labelinfo_7_2_2->setText("B12");
        ui->labelinfo_7_2_3->setText("B13");
        ui->labelinfo_7_2_4->setText("B14");
        ui->labelinfo_7_3_1->setText("B21");
        ui->labelinfo_7_3_2->setText("B22");
        ui->labelinfo_7_3_3->setText("B23");
        ui->labelinfo_7_3_4->setText("B24");
        ui->labelinfo_7_4_1->setText("B31");
        ui->labelinfo_7_4_2->setText("B32");
        ui->labelinfo_7_4_3->setText("B33");
        ui->labelinfo_7_4_4->setText("B34");
        ui->labelinfo_7_5_1->setText("B41");
        ui->labelinfo_7_5_2->setText("B42");
        ui->labelinfo_7_5_3->setText("B43");
        ui->labelinfo_7_5_4->setText("B44");
        ui->labelinfo_7_6_1->setText("B51");
        ui->labelinfo_7_6_2->setText("B52");
        ui->labelinfo_7_6_3->setText("B53");
        ui->labelinfo_7_6_4->setText("B54");
        ui->labelinfo_7_7_1->setText("B61");
        ui->labelinfo_7_7_2->setText("B62");
        ui->labelinfo_7_7_3->setText("B63");
        ui->labelinfo_7_7_4->setText("B64");

        ui->labelinfo_8_2_1->setText("A11");
        ui->labelinfo_8_2_2->setText("A12");
        ui->labelinfo_8_2_3->setText("A13");
        ui->labelinfo_8_2_4->setText("A14");
        ui->labelinfo_8_3_1->setText("A21");
        ui->labelinfo_8_3_2->setText("A22");
        ui->labelinfo_8_3_3->setText("A23");
        ui->labelinfo_8_3_4->setText("A24");
        ui->labelinfo_8_4_1->setText("A31");
        ui->labelinfo_8_4_2->setText("A32");
        ui->labelinfo_8_4_3->setText("A33");
        ui->labelinfo_8_4_4->setText("A34");
        ui->labelinfo_8_5_1->setText("A41");
        ui->labelinfo_8_5_2->setText("A42");
        ui->labelinfo_8_5_3->setText("A43");
        ui->labelinfo_8_5_4->setText("A44");
        ui->labelinfo_8_6_1->setText("A51");
        ui->labelinfo_8_6_2->setText("A52");
        ui->labelinfo_8_6_3->setText("A53");
        ui->labelinfo_8_6_4->setText("A54");
        ui->labelinfo_8_7_1->setText("A61");
        ui->labelinfo_8_7_2->setText("A62");
        ui->labelinfo_8_7_3->setText("A63");
        ui->labelinfo_8_7_4->setText("A64");
    }
}

void errinfo::setVoltage(){
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

void errinfo::setCurrent(){
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

void errinfo::setBrakeLevel(){
    QString strText = "", strStyleSheet = "";
    BRAKE_LEVEL_STATE level = BRAKE_LEVEL_STATE(m_VVVF_BCU_State);
    switch(level){
    case BRAKE_COMN_ERR:
        strText = "OFF";
        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
        break;
    case BRAKE_URGENT_BRAKE:
        strText = "紧急制动";
        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
        break;
    case BRAKE_BRAKE:
        strText = "制动"+QString("%1%").arg(m_VVVF_BCU_Value);
        strStyleSheet = QSS_BG_BROWN_BR_WHITE_WD_BLACK;
        break;
    case BRAKE_LAZY:
        strText = "OFF";
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        break;
    case BRAKE_DRAG:
        strText = "牵引"+QString("%1%").arg(m_VVVF_BCU_Value);
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

void errinfo::setSpeed(){
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

void errinfo::setErrorHappenInfo(){
    setVoltage();
    setCurrent();
    setBrakeLevel();
    setSpeed();
}

void errinfo::setErrorResetInfo(){
    if(0 == iCurrentFaltCount){
        QString strStyleSheet = "";
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        ui->labelvoltage->setText("");
        ui->labelvoltage->setStyleSheet(strStyleSheet);
        ui->labelcurrent->setText("");
        ui->labelcurrent->setStyleSheet(strStyleSheet);
        ui->labelbrake->setText("");
        ui->labelbrake->setStyleSheet(strStyleSheet);
        ui->labelspeed->setText("");
        ui->labelspeed->setStyleSheet(strStyleSheet);
        bErrorHappenedFlag = false;
    }
}

void errinfo::setVVVFError(ERROR_ID type, CAR_ID carId, DRAG_ELEC_BRAKE_STATE level, int iValue){
    MyLabel * tempLabel = NULL;
    switch (type) {
    case VVVF1_ERR:
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
    case VVVF2_ERR:
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
        if(VVVF1_ERR == type){
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
    bool bErrorFlag = false;
    QString strText = "", strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
    if(ELEC_BRAKE_BREAKDOWN == level){
        strText = canSignalsSlots->getVVVFErrorInfo(iValue);
        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
        bErrorFlag = true;
    }
    else{
        strText = "";
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        bErrorFlag = false;
    }
    tempLabel->setErrorState(bErrorFlag);
    tempLabel->setText(strText);
    tempLabel->setStyleSheet(strStyleSheet);
    tempLabel = NULL;
}

void errinfo::setSIVError(CAR_ID carId, SIV_STATE iState, int iValue){
    MyLabel * tempLabel = NULL;
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
    if(CAR_2 == carId || CAR_5 == carId){
        ui->labelinfo_4_3->setText("");
        ui->labelinfo_4_3->setStyleSheet("");
        ui->labelinfo_4_6->setText("");
        ui->labelinfo_4_6->setStyleSheet("");
    }
    else if(CAR_4 == carId || CAR_3 == carId){
        ui->labelinfo_4_5->setText("");
        ui->labelinfo_4_5->setStyleSheet("");
        ui->labelinfo_4_4->setText("");
        ui->labelinfo_4_4->setStyleSheet("");
    }
    bool bErrorFlag = false;
    QString strText = "", strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
    if(SIV_BREAKDOWN == iState){
        strText = canSignalsSlots->getSIVErrorInfo(iValue);
        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
        bErrorFlag = true;
    }
    else{
        strText = "";
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        bErrorFlag = false;
    }
    tempLabel->setErrorState(bErrorFlag);
    tempLabel->setText(strText);
    tempLabel->setStyleSheet(strStyleSheet);
    tempLabel = NULL;
}

void errinfo::setBCUError(CAR_ID carId, BC_KPA_STATE iState, int iValue){
    MyLabel * tempLabel = NULL;
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
    if(NULL == tempLabel){
        return ;
    }
    bool bErrorFlag = false;
    QString strText = "", strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
    if(BC_BREAKDOWN == iState){
        strText = canSignalsSlots->getBCUErrorInfo(iValue);
        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
        bErrorFlag = true;
    }
    else{
        strText = "";
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        bErrorFlag = false;
    }
    tempLabel->setErrorState(bErrorFlag);
    tempLabel->setText(strText);
    tempLabel->setStyleSheet(strStyleSheet);
    tempLabel = NULL;
}

void errinfo::setACCError(CAR_ID carId, INSIDE_TEMP_STATE iState, int iValue){
    MyLabel * tempLabel = NULL;
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
    if(NULL == tempLabel){
        return ;
    }
    bool bErrorFlag = false;
    QString strText = "", strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
    if(TEMP_BREAKDOWN == iState){
        strText = canSignalsSlots->getACCTempErrorInfo(iValue);
        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
        bErrorFlag = true;
    }
    else{
        strText = "";
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        bErrorFlag = false;
    }
    tempLabel->setErrorState(bErrorFlag);
    tempLabel->setText(strText);
    tempLabel->setStyleSheet(strStyleSheet);
    tempLabel = NULL;
}

void errinfo::setDoor_ERR(ERROR_ID errId, CAR_ID carId, DOOR_ID doorId, DOOR_ERR errType){
    MyLabel * tempLabel = NULL;
    switch (errId) {
    case RIGHT_DOOR_ERR:
        switch (carId) {
        case CAR_1:
            switch (doorId) {
            case DOOR_1:
                tempLabel = ui->labelinfo_7_2_1;
                break;
            case DOOR_2:
                tempLabel = ui->labelinfo_7_2_2;
                break;
            case DOOR_3:
                tempLabel = ui->labelinfo_7_2_3;
                break;
            case DOOR_4:
                tempLabel = ui->labelinfo_7_2_4;
                break;
            default:
                tempLabel = NULL;
                break;
            }
            break;
        case CAR_2:
            switch (doorId) {
            case DOOR_1:
                tempLabel = ui->labelinfo_7_3_1;
                break;
            case DOOR_2:
                tempLabel = ui->labelinfo_7_3_2;
                break;
            case DOOR_3:
                tempLabel = ui->labelinfo_7_3_3;
                break;
            case DOOR_4:
                tempLabel = ui->labelinfo_7_3_4;
                break;
            default:
                tempLabel = NULL;
                break;
            }
            break;
        case CAR_3:
            switch (doorId) {
            case DOOR_1:
                tempLabel = ui->labelinfo_7_4_1;
                break;
            case DOOR_2:
                tempLabel = ui->labelinfo_7_4_2;
                break;
            case DOOR_3:
                tempLabel = ui->labelinfo_7_4_3;
                break;
            case DOOR_4:
                tempLabel = ui->labelinfo_7_4_4;
                break;
            default:
                tempLabel = NULL;
                break;
            }
            break;
        case CAR_4:
            switch (doorId) {
            case DOOR_1:
                tempLabel = ui->labelinfo_7_5_1;
                break;
            case DOOR_2:
                tempLabel = ui->labelinfo_7_5_2;
                break;
            case DOOR_3:
                tempLabel = ui->labelinfo_7_5_3;
                break;
            case DOOR_4:
                tempLabel = ui->labelinfo_7_5_4;
                break;
            default:
                tempLabel = NULL;
                break;
            }
            break;
        case CAR_5:
            switch (doorId) {
            case DOOR_1:
                tempLabel = ui->labelinfo_7_6_1;
                break;
            case DOOR_2:
                tempLabel = ui->labelinfo_7_6_2;
                break;
            case DOOR_3:
                tempLabel = ui->labelinfo_7_6_3;
                break;
            case DOOR_4:
                tempLabel = ui->labelinfo_7_6_4;
                break;
            default:
                tempLabel = NULL;
                break;
            }
            break;
        case CAR_6:
            switch (doorId) {
            case DOOR_1:
                tempLabel = ui->labelinfo_7_7_1;
                break;
            case DOOR_2:
                tempLabel = ui->labelinfo_7_7_2;
                break;
            case DOOR_3:
                tempLabel = ui->labelinfo_7_7_3;
                break;
            case DOOR_4:
                tempLabel = ui->labelinfo_7_7_4;
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
        break;
    case LEFT_DOOR_ERR:
        switch (carId) {
        case CAR_1:
            switch (doorId) {
            case DOOR_1:
                tempLabel = ui->labelinfo_8_2_1;
                break;
            case DOOR_2:
                tempLabel = ui->labelinfo_8_2_2;
                break;
            case DOOR_3:
                tempLabel = ui->labelinfo_8_2_3;
                break;
            case DOOR_4:
                tempLabel = ui->labelinfo_8_2_4;
                break;
            default:
                tempLabel = NULL;
                break;
            }
            break;
        case CAR_2:
            switch (doorId) {
            case DOOR_1:
                tempLabel = ui->labelinfo_8_3_1;
                break;
            case DOOR_2:
                tempLabel = ui->labelinfo_8_3_2;
                break;
            case DOOR_3:
                tempLabel = ui->labelinfo_8_3_3;
                break;
            case DOOR_4:
                tempLabel = ui->labelinfo_8_3_4;
                break;
            default:
                tempLabel = NULL;
                break;
            }
            break;
        case CAR_3:
            switch (doorId) {
            case DOOR_1:
                tempLabel = ui->labelinfo_8_4_1;
                break;
            case DOOR_2:
                tempLabel = ui->labelinfo_8_4_2;
                break;
            case DOOR_3:
                tempLabel = ui->labelinfo_8_4_3;
                break;
            case DOOR_4:
                tempLabel = ui->labelinfo_8_4_4;
                break;
            default:
                tempLabel = NULL;
                break;
            }
            break;
        case CAR_4:
            switch (doorId) {
            case DOOR_1:
                tempLabel = ui->labelinfo_8_5_1;
                break;
            case DOOR_2:
                tempLabel = ui->labelinfo_8_5_2;
                break;
            case DOOR_3:
                tempLabel = ui->labelinfo_8_5_3;
                break;
            case DOOR_4:
                tempLabel = ui->labelinfo_8_5_4;
                break;
            default:
                tempLabel = NULL;
                break;
            }
            break;
        case CAR_5:
            switch (doorId) {
            case DOOR_1:
                tempLabel = ui->labelinfo_8_6_1;
                break;
            case DOOR_2:
                tempLabel = ui->labelinfo_8_6_2;
                break;
            case DOOR_3:
                tempLabel = ui->labelinfo_8_6_3;
                break;
            case DOOR_4:
                tempLabel = ui->labelinfo_8_6_4;
                break;
            default:
                tempLabel = NULL;
                break;
            }
            break;
        case CAR_6:
            switch (doorId) {
            case DOOR_1:
                tempLabel = ui->labelinfo_8_7_1;
                break;
            case DOOR_2:
                tempLabel = ui->labelinfo_8_7_2;
                break;
            case DOOR_3:
                tempLabel = ui->labelinfo_8_7_3;
                break;
            case DOOR_4:
                tempLabel = ui->labelinfo_8_7_4;
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
        break;
    default:
        tempLabel = NULL;
        break;
    }
    if(NULL == tempLabel){
        return ;
    }
    QString strStyleSheet = "";
    bool bErrorFlag = false;
    switch (errType) {
    case ERR_DOOR_BREAKDOWN:
        strStyleSheet = QSS_BG_RED_BR_NONE_WD_WHITE;
        bErrorFlag = true;
        break;
    case ERR_DOOR_COMN_ERR:
        strStyleSheet = QSS_BG_GREY_BR_NONE_WD_BLACK;
        bErrorFlag = false;
        break;
    case ERR_DOOR_URGENT_OPERATION:
        strStyleSheet = QSS_BG_PURPLE_BR_NONE_WD_WHITE;
        bErrorFlag = false;
        break;
    case ERR_DOOR_RUN_NORMAL:
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        bErrorFlag = false;
        break;
    default:
        strStyleSheet = "";
        bErrorFlag = false;
        break;
    }
    tempLabel->setErrorState(bErrorFlag);
    tempLabel->setStyleSheet(strStyleSheet);
    tempLabel = NULL;
}

void errinfo::showErrorScreen(){
    //    if(!bErrorHappenedFlag){
    timer_5s->start(TIMER_ERROR_AUTO_BACK_5S);
    //    }
}

void  errinfo::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void errinfo::on_buttonok_clicked(){
    buttonBeep();
    if(timer_5s->isActive()){
        timer_5s->stop();
    }
    emit sig_Error_OKBtn_Clicked();
}
void errinfo::slot_5S_Timer_Out(){
    if(!bErrorHappenedFlag){
        timer_5s->stop();
        emit sig_Error_OKBtn_Clicked();
    }
}

void errinfo::slot_MyLabel_VVVF_clicked(){
    MyLabel * tempLabel = dynamic_cast<MyLabel*>(sender());

    if(tempLabel->getErrorState()){
        QString strObjectName = tempLabel->objectName();
        int errType = strObjectName.section("_", 1, 1).toInt();
        int carId = strObjectName.section("_", 2, 2).toInt()-1;
        int vvvfIndex = 0;
        QString strErrTypeName= "";
        switch (errType) {
        case 2:
            strErrTypeName = "VVVF1故障";
            vvvfIndex = 1;
            break;
        case 3:
            strErrTypeName = "VVVF2故障";
            vvvfIndex = 2;
            break;
        default:
            strErrTypeName = "";
            vvvfIndex = 0;
            break;
        }
        if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
            carId = 7- carId;
        }
        int errCode = 0;
        errdetail * m_errdetail = new errdetail();
        m_errdetail->setCarId(carId);
        m_errdetail->setErrName(strErrTypeName);
        for(int i = 0; i < m_VVVFErrList.length(); i++){
            if(m_VVVFErrList.at(i).carId == carId && m_VVVFErrList.at(i).iVVVFIndex == vvvfIndex){
                errCode = m_VVVFErrList.at(i).iErrCode;
                break;
            }
        }
//        qDebug()<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   ERROR Screen Show Error, VVVF  VVVF  VVVF Error Code ==  "<<errCode;
        m_errdetail->setErrCode(1, errCode);
        m_errdetail->setModal(true);
        m_errdetail->show();
    }
}

void errinfo::slot_MyLabel_SIV_clicked(){
    MyLabel * tempLabel = dynamic_cast<MyLabel*>(sender());

    if(tempLabel->getErrorState()){
        QString strObjectName = tempLabel->objectName();
        int errType = strObjectName.section("_", 1, 1).toInt();
        int carId = strObjectName.section("_", 2, 2).toInt()-1;
        QString strErrTypeName= "";
        if(4 == errType){
            strErrTypeName = "SIV故障";
        }
        else{
            strErrTypeName = "";
        }
        if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
            carId = 7- carId;
        }
        int errCode = 0;
        errdetail * m_errdetail = new errdetail();
        m_errdetail->setCarId(carId);
        m_errdetail->setErrName(strErrTypeName);
        for(int i = 0; i < m_SIVErrList.length(); i++){
            if(m_SIVErrList.at(i).carId == carId){
                errCode = m_SIVErrList.at(i).iErrCode;
                break;
            }
        }
//        qDebug()<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   ERROR Screen Show Error, SIV  SIV  SIV Error Code ==  "<<errCode;
        m_errdetail->setErrCode(2, errCode);
        m_errdetail->setModal(true);
        m_errdetail->show();
    }
}

void errinfo::slot_MyLabel_BCU_clicked(){
    MyLabel * tempLabel = dynamic_cast<MyLabel*>(sender());

    if(tempLabel->getErrorState()){
        QString strObjectName = tempLabel->objectName();
        int errType = strObjectName.section("_", 1, 1).toInt();
        int carId = strObjectName.section("_", 2, 2).toInt()-1;
        QString strErrTypeName= "";
        if(5 == errType){
            strErrTypeName = "制动装置故障";
        }
        else{
            strErrTypeName = "";
        }
        if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
            carId = 7- carId;
        }
        int errCode = 0;
        errdetail * m_errdetail = new errdetail();
        m_errdetail->setCarId(carId);
        m_errdetail->setErrName(strErrTypeName);
        for(int i = 0; i < m_BCUErrList.length(); i++){
            if(m_BCUErrList.at(i).carId == carId){
                errCode = m_BCUErrList.at(i).iErrCode;
                break;
            }
        }
//        qDebug()<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   ERROR Screen Show Error, BCU  BCU  BCU Error Code ==  "<<errCode;
        m_errdetail->setErrCode(3, errCode);
        m_errdetail->setModal(true);
        m_errdetail->show();
    }
}

void errinfo::slot_MyLabel_ACC_clicked(){
    MyLabel * tempLabel = dynamic_cast<MyLabel*>(sender());

    if(tempLabel->getErrorState()){
        QString strObjectName = tempLabel->objectName();
        int errType = strObjectName.section("_", 1, 1).toInt();
        int carId = strObjectName.section("_", 2, 2).toInt()-1;
        QString strErrTypeName= "";
        if(6 == errType){
            strErrTypeName = "空调装置故障";
        }
        else{
            strErrTypeName = "";
        }
        if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
            carId = 7- carId;
        }
        int errCode = 0;
        errdetail * m_errdetail = new errdetail();
        m_errdetail->setCarId(carId);
        m_errdetail->setErrName(strErrTypeName);
        for(int i = 0; i < m_ACCErrList.length(); i++){
            if(m_ACCErrList.at(i).carId == carId){
                errCode = m_ACCErrList.at(i).iErrCode;
                break;
            }
        }
//        qDebug()<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^   ERROR Screen Show Error, ACC  ACC  ACC Error Code ==  "<<errCode;
        m_errdetail->setErrCode(4, errCode);
        m_errdetail->setModal(true);
        m_errdetail->show();
    }
}

void errinfo::slot_MyLabel_DOOR_clicked(){
    MyLabel * tempLabel = dynamic_cast<MyLabel*>(sender());

    if(tempLabel->getErrorState()){
        QString strObjectName = tempLabel->objectName();
        int errType = strObjectName.section("_", 1, 1).toInt();
        int carId = strObjectName.section("_", 2, 2).toInt()-1;

        QString strErrTypeName= "";
        switch (errType) {
        case 7:
            strErrTypeName = "右侧门故障";
            break;
        case 8:
            strErrTypeName = "左侧门故障";
            break;
        default:
            strErrTypeName = "";
            break;
        }
        int doorId = 0;
        int realDoorId = 0, errCode = 0;
        if(7 == errType || 8 == errType){
            doorId = strObjectName.section("_", 3, 3).toInt();
        }
        if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
            carId = 7- carId;
            if(7 == errType || 8 == errType){
                if(carId <= 3){
                    if(7 == errType){
                        realDoorId = 8-(doorId-1)*2;
                    }
                    else{
                        realDoorId = 7-(doorId-1)*2;
                    }
                }
                else{
                    if(7 == errType){
                        realDoorId = doorId*2-1;
                    }
                    else{
                        realDoorId = doorId*2;
                    }
                }
                doorId = 5 - doorId;
            }
        }
        else{
            if(carId <= 3){
                if(7 == errType){
                    realDoorId = doorId*2-1;
                }
                else{
                    realDoorId = doorId*2;
                }
            }
            else{
                if(7 == errType){
                    realDoorId = 8-(doorId-1)*2;
                }
                else{
                    realDoorId = 7-(doorId-1)*2;
                }
            }
        }

        for(int i = 0; i < m_DOORErrList.length(); i++){
            if(m_DOORErrList.at(i).carId == int(carId) && m_DOORErrList.at(i).doorId == int(realDoorId)){
                errCode = m_DOORErrList.at(i).iErrCode;
                qDebug()<<"Door Door Door Error Info ::::::::::::  Car Id == "<<carId<<"    Door Id  ==  "<<realDoorId<<"   Error Code ==  "<<errCode;
                break;
            }
        }

        errdetail * m_errdetail = new errdetail();
        m_errdetail->setCarId(carId);
        m_errdetail->setErrName(strErrTypeName);
        if(7 == errType || 8 == errType){
            m_errdetail->setDoorId(doorId);
        }
        m_errdetail->setErrCode(5, errCode);
        m_errdetail->setModal(true);
        m_errdetail->show();
    }
}

void errinfo::slot_CarIcon_Update(){
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

void errinfo::slot_CarCode_Update(){
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

void errinfo::slot_Car_Direction_Update(){
    slot_CarIcon_Update();
    slot_CarCode_Update();
    setDoorName();
}

void errinfo::slot_VVVF_BCU_Level_Update(int iState, int iValue){
    m_VVVF_BCU_State = iState;
    m_VVVF_BCU_Value = iValue;
}

void errinfo::slot_VVVF_Elec_BCU_Update(CAR_ID carId, int vvvfIndex, int iState, int iValue){
    int iVVVFErrorState = 0;   //0：当前状态保持，1：新增故障，2：故障复位
    if(ELEC_BRAKE_BREAKDOWN == DRAG_ELEC_BRAKE_STATE(iState)){
        bool bHappendFalg = false;
        for(int i = 0; i < m_VVVFErrList.length(); i++){
            if(m_VVVFErrList.at(i).carId == int(carId) && m_VVVFErrList.at(i).iVVVFIndex == vvvfIndex){
                bHappendFalg = true;
                if(m_VVVFErrList.at(i).iErrCode != iValue){
                    m_VVVFErrList.removeAt(i);
                    iCurrentFaltCount--;
                    iVVVFErrorState = 1;
                }
                else{
                    iVVVFErrorState = 0;
                }
                break;
            }
        }
        if(!bHappendFalg || (bHappendFalg && 1 == iVVVFErrorState)){
            struct_VVVFErr tempVVVFErr;
            tempVVVFErr.carId = int(carId);
            tempVVVFErr.iVVVFIndex = vvvfIndex;
            tempVVVFErr.iErrCode = iValue;
            m_VVVFErrList.append(tempVVVFErr);
            iVVVFErrorState = 1;
            iCurrentFaltCount++;
            setErrorHappenInfo();
        }
    }
    else{
        for(int i = 0; i < m_VVVFErrList.length(); i++){
            if(m_VVVFErrList.at(i).carId == int(carId) && m_VVVFErrList.at(i).iVVVFIndex == vvvfIndex){
                m_VVVFErrList.removeAt(i);
                iVVVFErrorState = 2;
                iCurrentFaltCount--;
                setErrorResetInfo();
                break;
            }
        }
    }
    if(1 == iVVVFErrorState){
        bErrorHappenedFlag = true;
        emit sig_Error_Happened();
    }
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    ERROR_ID tempType = ERROR_ID(vvvfIndex);
    DRAG_ELEC_BRAKE_STATE tempLevel = DRAG_ELEC_BRAKE_STATE(iState);
    setVVVFError(tempType, realCarId, tempLevel, iValue);
}

void errinfo::slot_SIV_Voltage_Freq_Update(CAR_ID carId, int iState, int iValue1, int /*iValue2*/){
    int iSIVErrorState = 0;   //0：当前状态保持，1：新增故障，2：故障复位
    if(SIV_BREAKDOWN == SIV_STATE(iState)){
        bool bHappendFalg = false;
        for(int i = 0; i < m_SIVErrList.length(); i++){
            if(m_SIVErrList.at(i).carId == int(carId)){
                bHappendFalg = true;
                if(m_SIVErrList.at(i).iErrCode != iValue1){
                    m_SIVErrList.removeAt(i);
                    iCurrentFaltCount--;
                    iSIVErrorState = 1;
                }
                else{
                    iSIVErrorState = 0;
                }
                break;
            }
        }
        if(!bHappendFalg || (bHappendFalg && 1 == iSIVErrorState)){
            struct_SIVErr tempSIVErr;
            tempSIVErr.carId = int(carId);
            tempSIVErr.iErrCode = iValue1;
            m_SIVErrList.append(tempSIVErr);
            iSIVErrorState = 1;
            iCurrentFaltCount++;
            setErrorHappenInfo();
        }
    }
    else{
        for(int i = 0; i < m_SIVErrList.length(); i++){
            if(m_SIVErrList.at(i).carId == int(carId)){
                m_SIVErrList.removeAt(i);
                iSIVErrorState = 2;
                iCurrentFaltCount--;
                setErrorResetInfo();
                break;
            }
        }
    }
    if(1 == iSIVErrorState){
        bErrorHappenedFlag = true;
        emit sig_Error_Happened();
    }
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    SIV_STATE tempState = SIV_STATE(iState);
    setSIVError(realCarId, tempState, iValue1);
}

void errinfo::slot_BCU_BC_Kpa_Update(CAR_ID carId, int iState, int iValue){
    int iBCUErrorState = 0;   //0：当前状态保持，1：新增故障，2：故障复位
    if(BC_BREAKDOWN == BC_KPA_STATE(iState)){
        bool bHappendFalg = false;
        for(int i = 0; i < m_BCUErrList.length(); i++){
            if(m_BCUErrList.at(i).carId == int(carId)){
                bHappendFalg = true;
                if(m_BCUErrList.at(i).iErrCode != iValue){
                    m_BCUErrList.removeAt(i);
                    iCurrentFaltCount--;
                    iBCUErrorState = 1;
                }
                else{
                    iBCUErrorState = 0;
                }
                break;
            }
        }
        if(!bHappendFalg || (bHappendFalg && 1 == bHappendFalg)){
            struct_BCUErr tempBCUErr;
            tempBCUErr.carId = int(carId);
            tempBCUErr.iErrCode = iValue;
            m_BCUErrList.append(tempBCUErr);
            iBCUErrorState = 1;
            iCurrentFaltCount++;
            setErrorHappenInfo();
        }
    }
    else{
        for(int i = 0; i < m_BCUErrList.length(); i++){
            if(m_BCUErrList.at(i).carId == int(carId)){
                m_BCUErrList.removeAt(i);
                iBCUErrorState = 2;
                iCurrentFaltCount--;
                setErrorResetInfo();
                break;
            }
        }
    }
    if(1 == iBCUErrorState){
        bErrorHappenedFlag = true;
        emit sig_Error_Happened();
    }
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    BC_KPA_STATE tempState = BC_KPA_STATE(iState);
    setBCUError(realCarId, tempState, iValue);
}

void errinfo::slot_ACC_Temp_Update(CAR_ID carId, int iState, int /*iValue1*/, int iValue2){
    int iACCErrorState = 0;   //0：当前状态保持，1：新增故障，2：故障复位
    if(TEMP_BREAKDOWN == INSIDE_TEMP_STATE(iState)){
        bool bHappendFalg = false;
        for(int i = 0; i < m_ACCErrList.length(); i++){
            if(m_ACCErrList.at(i).carId == int(carId)){
                bHappendFalg = true;
                if(m_ACCErrList.at(i).iErrCode != iValue2){
                    m_ACCErrList.removeAt(i);
                    iCurrentFaltCount--;
                    iACCErrorState = 1;
                }
                else{
                    iACCErrorState = 0;
                }
                break;
            }
        }
        if(!bHappendFalg || (bHappendFalg && 1 == iACCErrorState)){
            struct_ACCErr tempACCErr;
            tempACCErr.carId = int(carId);
            tempACCErr.iErrCode = iValue2;
            m_ACCErrList.append(tempACCErr);
            iACCErrorState = 1;
            iCurrentFaltCount++;
            setErrorHappenInfo();
        }
    }
    else{
        for(int i = 0; i < m_ACCErrList.length(); i++){
            if(m_ACCErrList.at(i).carId == int(carId)){
                m_ACCErrList.removeAt(i);
                iACCErrorState = 2;
                iCurrentFaltCount--;
                setErrorResetInfo();
                break;
            }
        }
    }
    if(1 == iACCErrorState){
        bErrorHappenedFlag = true;
        emit sig_Error_Happened();
    }
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    INSIDE_TEMP_STATE tempState = INSIDE_TEMP_STATE(iState);
    setACCError(realCarId, tempState, iValue2);
}

void errinfo::slot_Door_State_Update(CAR_ID carId, DOOR_ID doorId, RUN_DOOR_STATE doorState){
    int iDOORErrorState = 0;   //0：当前状态保持，1：新增故障，2：故障复位
    if(RUN_DOOR_BREAKDOWN == RUN_DOOR_STATE(doorState)){
        bool bHappendFalg = false;
        for(int i = 0; i < m_DOORErrList.length(); i++){
            if(m_DOORErrList.at(i).carId == int(carId) && m_DOORErrList.at(i).doorId == int(doorId)){
                bHappendFalg = true;
                if(m_DOORErrList.at(i).iErrCode != canSignalsSlots->mDoorFirstFaultCode[int(carId-1)*8+int(doorId-1)]){
                    m_DOORErrList.removeAt(i);
                    iCurrentFaltCount--;
                    iDOORErrorState = 1;
                }
                else{
                    iDOORErrorState = 0;
                }
                break;
            }
        }

        if(!bHappendFalg || (bHappendFalg && 1 == iDOORErrorState)){
            struct_DOORErr tempDOORErr;
            tempDOORErr.carId = int(carId);
            tempDOORErr.doorId = int(doorId);
            tempDOORErr.iErrCode = canSignalsSlots->mDoorFirstFaultCode[int(carId-1)*8+int(doorId-1)];
            m_DOORErrList.append(tempDOORErr);
            iDOORErrorState = 1;
            iCurrentFaltCount++;
            setErrorHappenInfo();
        }
    }
    else{
        for(int i = 0; i < m_DOORErrList.length(); i++){
            if(m_DOORErrList.at(i).carId == int(carId) && m_DOORErrList.at(i).doorId == int(doorId)){
                m_DOORErrList.removeAt(i);
                iDOORErrorState = 2;
                iCurrentFaltCount--;
                setErrorResetInfo();
                break;
            }
        }
    }
    if(1 == iDOORErrorState){
        bErrorHappenedFlag = true;
        emit sig_Error_Happened();
    }
    ERROR_ID errorId = NONE_ERR;
    int realCarNo =0, realDoorNo = 0;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        if(carId <= 3){
            if(1 == (doorId % 2)){
                errorId = LEFT_DOOR_ERR;
                realDoorNo = 4-int(doorId)/2;
            }
            else{
                errorId = RIGHT_DOOR_ERR;
                realDoorNo = 5-int(doorId)/2;
            }
        }
        else{
            if(1 == (doorId % 2)){
                errorId = RIGHT_DOOR_ERR;
                realDoorNo = int(doorId)/2+1;
            }
            else{
                errorId = LEFT_DOOR_ERR;
                realDoorNo = int(doorId)/2;
            }
        }
        realCarNo = 7 - int(carId);
    }
    else{
        if(carId <= 3){
            if(1 == (doorId % 2)){
                errorId = RIGHT_DOOR_ERR;
                realDoorNo = int(doorId)/2+1;
            }
            else{
                errorId = LEFT_DOOR_ERR;
                realDoorNo = int(doorId)/2;
            }
        }
        else{
            if(1 == (doorId % 2)){
                errorId = LEFT_DOOR_ERR;
                realDoorNo = 4-int(doorId)/2;
            }
            else{
                errorId = RIGHT_DOOR_ERR;
                realDoorNo = int(doorId)/2;
            }
        }
        realCarNo = int(carId);
    }
    DOOR_ERR doorErrType = ERR_DOOR_RUN_NORMAL;
    switch (doorState) {
    case RUN_DOOR_COMN_ERR:
        doorErrType = ERR_DOOR_COMN_ERR;
        break;
    case RUN_DOOR_URGENT_ACTION:
        doorErrType = ERR_DOOR_URGENT_OPERATION;
        break;
    case RUN_DOOR_BREAKDOWN:
        doorErrType = ERR_DOOR_BREAKDOWN;
        break;
    default:
        doorErrType = ERR_DOOR_RUN_NORMAL;
        break;
    }
    setDoor_ERR(errorId, CAR_ID(realCarNo), DOOR_ID(realDoorNo), doorErrType);
}
