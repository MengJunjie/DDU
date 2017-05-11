#include "speedcheck.h"
#include "ui_speedcheck.h"
#include "speedcheckchange.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

speedcheck::speedcheck(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::speedcheck)
{
    ui->setupUi(this);

    bStartCheckFlag = false;
    b500MSFlag = false;

    iVVVFBCULevelState = 0;
    iVVVFBCULevelValue = 0;
    iP1PWMValue = 0;
    iP2PWMValue = 0;
    iMPWMValue = 0;

    iPlus1Flag = 0;
    iPlus2Flag = 0;
    iMinusFlag = 0;
    iCheckTimes = 0;

    iPlusS1 = 0;
    iPlusS2 = 0;
    iPlusS3 = 0;
    iMinusP1 = 0;
    iMinusP2 = 0;
    iRunDistance = 0;
    fRunDistance = 0;
    strStartPlus = "";
    strStartMinus = "";

    bPlus1OkFlag = false;
    bPlus2OkFlag = false;
    bMinusOkFlag = false;
    iRebornFlag = 1;
    bNextScreenFlag = false;
    speedDataList.clear();

    Init();
    slot_CarIcon_Update();
    slot_CarCode_Update();

    setBrakeLevel(BRAKE_URGENT_BRAKE);

    setBHB_BLB(CAR_3, false, false);
    setBHB_BLB(CAR_2, false, false);

    timer1S = new QTimer();
    connect(timer1S, SIGNAL(timeout()), this, SLOT(slot_TimerOut_1S()));
    timer1S_Distance = new QTimer();
    connect(timer1S_Distance, SIGNAL(timeout()), this, SLOT(slot_TimerOut_Distance_1S()));
    timer500ms = new QTimer();
    connect(timer500ms, SIGNAL(timeout()), this, SLOT(slot_TimerOut_500MS()));

    connect(canSignalsSlots, SIGNAL(sig_Car_Code_Change()), this, SLOT(slot_CarCode_Update()));
    connect(canSignalsSlots, SIGNAL(sig_Car_Direction_Change()), this, SLOT(slot_Car_Direction_Update()));
    connect(canSignalsSlots, SIGNAL(sig_VVVF_BCU_Level_Update(int, int)), this, SLOT(slot_VVVF_BCU_Level_Update(int, int)));
    connect(canSignalsSlots, SIGNAL(sig_VVVF_Elec_BCU_Update(CAR_ID,int,int,int)), this, SLOT(slot_VVVF_Elec_BCU_Update(CAR_ID,int,int,int)));
    connect(canSignalsSlots, SIGNAL(sig_BCU_BC_Kpa_Update(CAR_ID,int,int)), this, SLOT(slot_BCU_BC_Kpa_Update(CAR_ID,int,int)));
    connect(canSignalsSlots, SIGNAL(sig_BHB_BLB_State_Change(CAR_ID,bool,bool)), this, SLOT(slot_BHB_BLB_State_Update(CAR_ID,bool,bool)));
    connect(canSignalsSlots, SIGNAL(sig_UrgentShort(CAR_ID,bool)), this, SLOT(slot_UrgentShort_Update(CAR_ID,bool)));
    connect(canSignalsSlots, SIGNAL(sig_VVVF_Elec_Brake_State_Update(bool)), this, SLOT(slot_VVVF_Elec_Brake_State_Update(bool)));

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));
    connect(this, SIGNAL(sig_Speed_Check_Start()), canSignalsSlots, SLOT(slot_Speed_Check_Start()));

    emit sig_Speed_Check_Start();
}

speedcheck::~speedcheck(){
    delete ui;
}

void speedcheck::Init(){
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

void speedcheck::setVoltage(){
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

void speedcheck::setCurrent(){
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

void speedcheck::setBrakeLevel(BRAKE_LEVEL_STATE level){
    QString strText = "", strStyleSheet = "";

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
        strText = "制动"+QString("%1%").arg(iVVVFBCULevelValue);
        strStyleSheet = QSS_BG_BROWN_BR_WHITE_WD_BLACK;
        break;
    case BRAKE_LAZY:
        strText = "OFF";
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        break;
    case BRAKE_DRAG:
        strText = "牵引"+QString("%1%").arg(iVVVFBCULevelValue);
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

void speedcheck::setSpeed(){
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

void speedcheck::setElecBrake(DRAG_ELEC_BRAKE_TYPE type, CAR_ID carId, DRAG_ELEC_BRAKE_STATE level, int iValue){
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
    tempLabel = NULL;
}

void speedcheck::setBC_KPA(CAR_ID carId, BC_KPA_STATE state, int value){
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
    tempLabel = NULL;
}

void speedcheck::setBHB_BLB(CAR_ID carId, bool bBHBState, bool bBLBState){
    QList<QLabel*> bhb_blbList1;
    QList<QLabel*> bhb_blbList2;
    bhb_blbList1.clear();
    bhb_blbList2.clear();
    bhb_blbList1 << ui->labelinfo_5_3_1 << ui->labelinfo_5_3_2 << ui->labelinfo_5_4_1 << ui->labelinfo_5_4_2;
    bhb_blbList2 << ui->labelinfo_5_5_1 << ui->labelinfo_5_5_2 << ui->labelinfo_5_6_1 << ui->labelinfo_5_6_2;
    QLabel * tempLabel1 = NULL;
    QLabel * tempLabel2 = NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        for(int i = 0; i < bhb_blbList1.length(); i++){
            bhb_blbList1.at(i)->setText("");
            bhb_blbList1.at(i)->setStyleSheet("");
        }
        switch (carId) {
        case CAR_2:
            tempLabel1 = ui->labelinfo_5_6_1;
            tempLabel2 = ui->labelinfo_5_6_2;
            break;
        case CAR_3:
            tempLabel1 = ui->labelinfo_5_5_1;
            tempLabel2 = ui->labelinfo_5_5_2;
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
            tempLabel1 = ui->labelinfo_5_3_1;
            tempLabel2 = ui->labelinfo_5_3_2;
            break;
        case CAR_3:
            tempLabel1 = ui->labelinfo_5_4_1;
            tempLabel2 = ui->labelinfo_5_4_2;
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
    tempLabel1 = NULL;
    tempLabel2 = NULL;
}

void speedcheck::setUrgentShort(CAR_ID carId, bool bState){
    QLabel * tempLabel = NULL;
    switch (carId) {
    case CAR_1:
        tempLabel = ui->labelinfo_6_2;
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
    QString strStyleSheet = "";
    if(bState){
        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
    }
    else{
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
    }
    tempLabel->setStyleSheet(strStyleSheet);
    tempLabel = NULL;
}

void speedcheck::speedChecking(){
    if(4 == iVVVFBCULevelState && (2 != iPlus1Flag || 2 != iPlus2Flag || 2 != iMinusFlag)){
        if(1 == iMinusFlag){
            iMinusFlag = 2;
            bMinusOkFlag = false;
        }
        //加速度1，加速度2判定开始
        if(0 == iPlus1Flag || 0 == iPlus2Flag){
            if(carSpeedValue[1] < 40){
                iPlus1Flag = 1;
                iPlus2Flag = 1;
                iPlusS1 = carSpeedValue[1];
                mQTPlus1 = QDateTime::currentDateTime();
                strStartPlus = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss");
                if(!b500MSFlag){
                    timer500ms->start(SPEED_CHECK_500MS);
                }
            }
            else if(carSpeedValue[1] >= 40 && carSpeedValue[1] < 80){
                iPlus1Flag = 2;
                bPlus1OkFlag = false;
                iPlus2Flag = 1;
                iPlusS1 = carSpeedValue[1];
                mQTPlus1 = QDateTime::currentDateTime();
                strStartPlus = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss");
                if(!b500MSFlag){
                    timer500ms->start(SPEED_CHECK_500MS);
                }
            }
            else{
                iPlus1Flag = 2;
                bPlus1OkFlag = false;
                iPlus2Flag = 2;
                bPlus2OkFlag = false;
            }
        }

        if(carSpeedValue[1] >= 40 && 1 == iPlus1Flag){  //加速度1测定结束
            iPlus1Flag = 2;
            iPlusS2 = carSpeedValue[1];
            mQTPlus2 = QDateTime::currentDateTime();
//            speedDataList.append(speedData);
            bPlus1OkFlag = true;
        }

        if(carSpeedValue[1] >= 80 && 1 == iPlus2Flag){  //加速度2测定结束
            iPlus2Flag = 2;
            iPlusS3 = carSpeedValue[1];
            mQTPlus3 = QDateTime::currentDateTime();
//            speedDataList.append(speedData);
            bPlus2OkFlag = true;
        }
    }
    else if((1 == iVVVFBCULevelState || 2 == iVVVFBCULevelState) && (2 != iPlus1Flag || 2 != iPlus2Flag || 2 != iMinusFlag)){
        if(carSpeedValue[1] > 0 && 2 != iPlus1Flag){
            iPlus1Flag = 2;
            bPlus1OkFlag = false;
        }
        if(carSpeedValue[1] > 0 && 2 != iPlus2Flag){
            iPlus2Flag = 2;
            bPlus2OkFlag = false;
        }

        //减速度判定开始
        if(0 == iMinusFlag){
            if(carSpeedValue[1] > 0){
                iMinusFlag = 1;
                iMinusP1 = carSpeedValue[1];
                fRunDistance = 0;
                iRunDistance = 0;
                iMPWMValue = 0;
                strStartMinus = QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss");
                timer1S_Distance->start(SPEED_CHECK_1S);
            }
        }

        if(0 == carSpeedValue[1] && 1 == iMinusFlag){  //减速度测定结束
            iMinusFlag = 2;
            iMinusP2 = carSpeedValue[1];
            iRunDistance = qRound(fRunDistance);
//            speedDataList.append(speedData);
            bMinusOkFlag = true;
        }
    }
}

void speedcheck::getSpeedCheckResult(){
    if(2 == iPlus1Flag && bPlus1OkFlag){
        structSpeedCheck * checkPlus1 = new structSpeedCheck;
        int iTimeData = mQTPlus1.secsTo(mQTPlus2);
        checkPlus1->strDate = strStartPlus.section(" ", 0, 0);
        checkPlus1->strTime = strStartPlus.section(" ", 1, 1);
        checkPlus1->iCheckType = 1;
        checkPlus1->iSpeedValue = (iPlusS2- iPlusS1)/(iTimeData*1.0)*100;
        checkPlus1->iPWMValue = iP1PWMValue;
        checkPlus1->iRebornFlag = 0;
        checkPlus1->iBrakeSpeed = 0;
        checkPlus1->iBrakeDistance = 0;
        speedDataList.append(checkPlus1);
        canSignalsSlots->mSpeedCheckList.append(checkPlus1);
    }

    if(2 == iPlus2Flag && bPlus2OkFlag){
        structSpeedCheck * checkPlus2 = new structSpeedCheck;
        int iTimeData = mQTPlus1.secsTo(mQTPlus3);
        checkPlus2->strDate = strStartPlus.section(" ", 0, 0);
        checkPlus2->strTime = strStartPlus.section(" ", 1, 1);
        checkPlus2->iCheckType = 2;
        checkPlus2->iSpeedValue = (iPlusS3- iPlusS1)/(iTimeData*1.0)*100;
        checkPlus2->iPWMValue = iP2PWMValue;
        checkPlus2->iRebornFlag = 0;
        checkPlus2->iBrakeSpeed = 0;
        checkPlus2->iBrakeDistance = 0;
        speedDataList.append(checkPlus2);
        canSignalsSlots->mSpeedCheckList.append(checkPlus2);
    }

    if(2 == iMinusFlag && bMinusOkFlag){
        structSpeedCheck * checkMinus = new structSpeedCheck;
        float fSpeedValue = (iMinusP2*KMH_TO_MS_UNIT_VALUE*iMinusP2*KMH_TO_MS_UNIT_VALUE - iMinusP1*KMH_TO_MS_UNIT_VALUE*iMinusP1*KMH_TO_MS_UNIT_VALUE)/(iRunDistance*2*1.0)*100;
        checkMinus->strDate = strStartMinus.section(" ", 0, 0);
        checkMinus->strTime = strStartMinus.section(" ", 1, 1);
        checkMinus->iCheckType = 3;
        checkMinus->iSpeedValue = qRound(fSpeedValue);
        checkMinus->iPWMValue = iMPWMValue;
        checkMinus->iRebornFlag = iRebornFlag;
        checkMinus->iBrakeSpeed = iMinusP1;
        checkMinus->iBrakeDistance = iRunDistance;
        speedDataList.append(checkMinus);
        canSignalsSlots->mSpeedCheckList.append(checkMinus);
    }

    if(canSignalsSlots->mSpeedCheckList.length() > 0){
        canSignalsSlots->mSpeedCheckSendData[0] = canSignalsSlots->mSpeedCheckList.length();
        canSignalsSlots->mSpeedCheckSendData[1] = 1;
        canSignalsSlots->mSpeedCheckSendData[2] = 1;
        canSignalsSlots->sendSpeedCheckData(1);
        canSignalsSlots->sendSpeedCheckData(2);
    }

    emit sig_Speed_CheckData_Update();
}

void speedcheck::showNextScreen(int iShowFlag){
    if(!bNextScreenFlag){
        bNextScreenFlag = true;
        speedcheckchange * m_speedcheckchange = new speedcheckchange(iShowFlag, this);
        connect(m_speedcheckchange, SIGNAL(sig_Closed()), this, SLOT(reject()));
        connect(m_speedcheckchange, SIGNAL(sig_PBtn_Start_Clicked()), this, SLOT(on_buttonstart_clicked()));
        connect(m_speedcheckchange, SIGNAL(sig_PBtn_Stop_Clicked()), this, SLOT(on_buttonstop_clicked()));
        connect(this, SIGNAL(sig_Speed_CheckData_Update()), m_speedcheckchange, SLOT(slot_SpeedCheckData_Update()));

        m_speedcheckchange->setModal(true);
        m_speedcheckchange->show();
    }
}

void speedcheck::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
    setVoltage();
    setCurrent();
    setSpeed();
}

void speedcheck::on_buttonstart_clicked(){
    if(!bStartCheckFlag){
        bStartCheckFlag = true;
        iPlus1Flag = 0;
        iPlus2Flag = 0;
        iMinusFlag = 0;
        iP1PWMValue = 0;
        iP2PWMValue = 0;
        iMPWMValue = 0;
        iPlusS1 = 0;
        iPlusS2 = 0;
        iPlusS3 = 0;
        iMinusP1 = 0;
        iMinusP2 = 0;
        iRunDistance = 0;
        fRunDistance = 0;
//        mQTPlus1.setTime_t(0);
//        mQTPlus2.setTime_t(0);
//        mQTPlus3.setTime_t(0);
        strStartPlus = "";
        strStartMinus = "";
        bPlus1OkFlag = false;
        bPlus2OkFlag = false;
        bMinusOkFlag = false;
        iRebornFlag = 1;
        ui->buttonstart->setStyleSheet(QSS_PBTN_SELECTED_GREY);
        buttonBeep();
        speedChecking();
        timer1S->start(SPEED_CHECK_1S);
    }
}

void speedcheck::on_buttonstop_clicked(){
    if(bStartCheckFlag){
        qDeleteAll(canSignalsSlots->mSpeedCheckList);
        canSignalsSlots->mSpeedCheckList.clear();

        getSpeedCheckResult();

        if(2 != iPlus1Flag){
            iPlus1Flag = 2;
            bPlus1OkFlag = false;
        }
        if(2 != iPlus2Flag){
            iPlus2Flag = 2;
            bPlus2OkFlag = false;
        }
        if(2 != iMinusFlag){
            iMinusFlag = 2;
            bMinusOkFlag = false;
        }

        bStartCheckFlag = false;
        ui->buttonstart->setStyleSheet(QSS_PBTN_NO_SELECTED);
        buttonBeep();
        if(speedDataList.length() > 0){
            showNextScreen(2);
        }
    }
}

void speedcheck::on_buttonchange_clicked(){
    buttonBeep();
    showNextScreen(1);
}

void speedcheck::on_buttonquit_clicked(){
    buttonBeep();
    this->reject();
}

void speedcheck::slot_CarIcon_Update(){
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

void speedcheck::slot_CarCode_Update(){
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

void speedcheck::slot_Car_Direction_Update(){
    slot_CarIcon_Update();
    slot_CarCode_Update();
}

void speedcheck::slot_VVVF_BCU_Level_Update(int iState, int iValue){
    iVVVFBCULevelState = iState;
    iVVVFBCULevelValue = iValue;
    setBrakeLevel(BRAKE_LEVEL_STATE(iVVVFBCULevelState));
}

void speedcheck::slot_VVVF_Elec_BCU_Update(CAR_ID carId, int vvvfIndex, int iState, int iValue){
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

void speedcheck::slot_BCU_BC_Kpa_Update(CAR_ID carId, int iState, int iValue){
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

void speedcheck::slot_BHB_BLB_State_Update(CAR_ID carId, bool bBHBFlag, bool bBLBFlag){
    setBHB_BLB(carId, bBHBFlag, bBLBFlag);
}

void speedcheck::slot_UrgentShort_Update(CAR_ID carId, bool bState){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    setUrgentShort(realCarId, bState);
}

void speedcheck::slot_VVVF_Elec_Brake_State_Update(bool bState){
    if(1 == iMinusFlag ){
        if(2 != iRebornFlag && bState){
            iRebornFlag = 2;
        }
    }

    if(2 != iRebornFlag){
        iRebornFlag = 1;
    }
}

void speedcheck::slot_TimerOut_1S(){
    speedChecking();

    if(2 == iPlus1Flag && 2 == iPlus2Flag && 2 == iMinusFlag){
        timer1S->stop();
    }
}

void speedcheck::slot_TimerOut_Distance_1S(){
    if(1 == iMinusFlag){
        fRunDistance = fRunDistance + carSpeedValue[1]*KMH_TO_MS_UNIT_VALUE;
    }
    if(2 == iMinusFlag){
        timer1S_Distance->stop();
    }
}

void speedcheck::slot_TimerOut_500MS(){
    if(1 == iPlus1Flag){
        if(iVVVFBCULevelValue > iP1PWMValue){
            iP1PWMValue = iVVVFBCULevelValue;
        }
    }

    if(1 == iPlus2Flag){
        if(iVVVFBCULevelValue > iP2PWMValue){
            iP2PWMValue = iVVVFBCULevelValue;
        }
    }

    if(1 == iMinusFlag){
        if(iVVVFBCULevelValue > iMPWMValue){
            iMPWMValue = 0-iVVVFBCULevelValue;
        }
    }

    if(2 == iPlus1Flag && 2 == iPlus2Flag && 2 == iMinusFlag){
        timer500ms->stop();
    }
}
