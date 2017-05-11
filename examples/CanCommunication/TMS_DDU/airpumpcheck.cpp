#include "airpumpcheck.h"
#include "ui_airpumpcheck.h"
#include "../objectdictdata.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

airpumpcheck::airpumpcheck(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::airpumpcheck)
{
    ui->setupUi(this);

    bCheckStartFlag = false;
    bACP1CheckFlag = false;
    bACP2CheckFlag = false;
    iACP1WorkTime = 0;
    iACP2WorkTime = 0;
    iACP1ActionFlag = ACTION_NOSET;
    iACP2ActionFlag = ACTION_NOSET;
    iACP1ResultFlag = RUNTIME_STANDBY;
    iACP2ResultFlag = RUNTIME_STANDBY;

    Init();
    slot_CarIcon_Update();
    slot_CarCode_Update();
    setVoltage();
    setCurrent();

    setAirpumpAction(CAR_1, ACTION_NOSET);
    setAirpumpAction(CAR_6, ACTION_NOSET);

    timerACP1 = new QTimer();
    connect(timerACP1, SIGNAL(timeout()), this, SLOT(slot_ACP1_Check_TimeOut()));
    timerACP2 = new QTimer();
    connect(timerACP2, SIGNAL(timeout()), this, SLOT(slot_ACP2_Check_TimeOut()));

    connect(canSignalsSlots, SIGNAL(sig_Car_Direction_Change()), this, SLOT(slot_Car_Direction_Update()));
    connect(canSignalsSlots, SIGNAL(sig_Car_Code_Change()), this, SLOT(slot_CarCode_Update()));
    connect(canSignalsSlots, SIGNAL(sig_AirPump_State_Change(CAR_ID,bool)), this, SLOT(slot_AirPump_State_Change(CAR_ID,bool)));

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));
    connect(this, SIGNAL(sig_ACP_Check_Start()), canSignalsSlots, SLOT(slot_ACP_Check_Start()));

    emit sig_ACP_Check_Start();
}

airpumpcheck::~airpumpcheck(){
    delete ui;
}

void  airpumpcheck::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

//    ui->labelarrow->setScaledContents(true);
    ui->carimagelabel1->setScaledContents(true);
    ui->carimagelabel2->setScaledContents(true);
    ui->carimagelabel3->setScaledContents(true);
    ui->carimagelabel4->setScaledContents(true);
    ui->carimagelabel5->setScaledContents(true);
    ui->carimagelabel6->setScaledContents(true);

    ui->buttonrecord->setEnabled(false);

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void airpumpcheck::setVoltage(){
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

void airpumpcheck::setCurrent(){
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

void airpumpcheck::setAirpumpAction(CAR_ID carId, AIRPUMP_ACTION_TYPE type){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    QLabel * tempLabel = NULL;
    switch (realCarId) {
    case CAR_1:
        tempLabel = ui->labelinfo_2_2_1;
        break;
    case CAR_2:
        tempLabel = ui->labelinfo_2_3_1;
        break;
    case CAR_3:
        tempLabel = ui->labelinfo_2_4_1;
        break;
    case CAR_4:
        tempLabel = ui->labelinfo_2_5_1;
        break;
    case CAR_5:
        tempLabel = ui->labelinfo_2_6_1;
        break;
    case CAR_6:
        tempLabel = ui->labelinfo_2_7_1;
        break;
    default:
        tempLabel = NULL;
        break;
    }

    if(NULL == tempLabel){
        return ;
    }

    QString strStyleSheet = "";
    switch (type) {
    case ACTION_NOSET:
        strStyleSheet = QSS_BG_BLACK_BR_BLACK_WD_WHITE;
        break;
    case ACTION_STOPED:
        strStyleSheet = QSS_BG_BLACK_BR_GREEN_WD_WHITE;
        break;
    case ACTION_RUNING:
        strStyleSheet = QSS_BG_GREEN_BR_GREEN_WD_BLACK;
        break;
    default:
        strStyleSheet = "";
        break;
    }
    tempLabel->setStyleSheet(strStyleSheet);
    tempLabel = NULL;
}

void airpumpcheck::setActionTime(CAR_ID carId, AIRPUMP_RUNTIME_TYPE type, int iValue){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    QLabel * tempLabel = NULL;
    switch (realCarId) {
    case CAR_1:
        tempLabel = ui->labelinfo_3_2;
        break;
    case CAR_6:
        tempLabel = ui->labelinfo_3_7;
        break;
    default:
        tempLabel = NULL;
        break;
    }

    if(NULL == tempLabel){
        return ;
    }

    QString strText = "";
    switch (type) {
    case RUNTIME_STANDBY:
        strText = "";
        break;
    case RUNTIME_JUDGING:
        strText = "判断中";
        break;
    case RUNTIME_50SEC:
        strText = QString("%1秒").arg(iValue);
        break;
    default:
        strText = "";
        break;
    }
    tempLabel->setText(strText);
}

void airpumpcheck::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
    setVoltage();
    setCurrent();
}

void airpumpcheck::on_buttonstrart_clicked(){
    if(bCheckStartFlag){
        return;
    }
    bCheckStartFlag = true;
    bool bBeepFlag = false;
    if(!bACP1CheckFlag){
        bBeepFlag = true;
        bACP1CheckFlag = true;
        iACP1WorkTime = 0;
        iACP1ResultFlag = RUNTIME_JUDGING;
        if(ACTION_RUNING == iACP1ActionFlag){
            timerACP1->start(ACP_CHECK_TOME_OUT);
        }
        setActionTime(CAR_1, iACP1ResultFlag, iACP1WorkTime);
    }
    if(!bACP2CheckFlag){
        bBeepFlag = true;
        bACP2CheckFlag = true;
        iACP2WorkTime = 0;
        iACP2ResultFlag = RUNTIME_JUDGING;
        if(ACTION_RUNING == iACP2ActionFlag){
            timerACP2->start(ACP_CHECK_TOME_OUT);
        }
        setActionTime(CAR_6, iACP2ResultFlag, iACP2WorkTime);
    }
    if(bBeepFlag){
        buttonBeep();
        ui->buttonrecord->setEnabled(true);
    }
}

void airpumpcheck::on_buttonstop_clicked(){
    if(!bCheckStartFlag){
        return;
    }
    bCheckStartFlag = false;
    bool bBeepFlag = false;
    if(RUNTIME_50SEC != iACP1ResultFlag){
        bBeepFlag = true;
        timerACP1->stop();
        bACP1CheckFlag = false;
        iACP1ResultFlag = RUNTIME_50SEC;
        setActionTime(CAR_1, iACP1ResultFlag, iACP1WorkTime);
    }
    if(RUNTIME_50SEC != iACP2ResultFlag){
        bBeepFlag = true;
        timerACP2->stop();
        bACP2CheckFlag = false;
        iACP2ResultFlag = RUNTIME_50SEC;
        setActionTime(CAR_6, iACP2ResultFlag, iACP2WorkTime);
    }
    if(bBeepFlag){
        buttonBeep();
    }
}

void airpumpcheck::on_buttonrecord_clicked(){
//    ui->buttonrecord->setEnabled(false);
    buttonBeep();
    ACPCheck_Data[0] = iACP1WorkTime;
    ACPCheck_Data[1] = iACP2WorkTime;
    canSignalsSlots->sendSDOCheckData(2);
    qDebug()<<"SIV Check Time ==  "<<iACP1WorkTime<<"     "<<iACP2WorkTime;
}

void airpumpcheck::on_buttonquit_clicked(){
    buttonBeep();
    this->reject();
}

void airpumpcheck::slot_CarIcon_Update(){
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

void airpumpcheck::slot_CarCode_Update(){
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

void airpumpcheck::slot_Car_Direction_Update(){
    slot_CarIcon_Update();
    slot_CarCode_Update();
    setAirpumpAction(CAR_1, iACP1ActionFlag);
    setAirpumpAction(CAR_6, iACP2ActionFlag);
    setActionTime(CAR_1, iACP1ResultFlag, iACP1WorkTime);
    setActionTime(CAR_6, iACP2ResultFlag, iACP2WorkTime);
}

void airpumpcheck::slot_AirPump_State_Change(CAR_ID carId, bool bState){
    if(CAR_1 == carId){
        if(bState){
            iACP1ActionFlag = ACTION_RUNING;
        }
        else{
            iACP1ActionFlag = ACTION_STOPED;
        }
        setAirpumpAction(carId, iACP1ActionFlag);
    }
    if(CAR_6 == carId){
        if(bState){
            iACP2ActionFlag = ACTION_RUNING;
        }
        else{
            iACP2ActionFlag = ACTION_STOPED;
        }
        setAirpumpAction(carId, iACP2ActionFlag);
    }

    if(bCheckStartFlag){
        if(CAR_1 == carId){
            if(bState){
                if(bACP1CheckFlag && RUNTIME_JUDGING == iACP1ResultFlag && !timerACP1->isActive()){
                    timerACP1->start(ACP_CHECK_TOME_OUT);
                }
            }
            else{
                if(RUNTIME_JUDGING == iACP1ResultFlag && timerACP1->isActive()){
                    timerACP1->stop();
                    bACP1CheckFlag = false;
                    if(!bACP2CheckFlag){
                        bCheckStartFlag = false;
                    }
                    iACP1ResultFlag = RUNTIME_50SEC;
                    setActionTime(carId, iACP1ResultFlag, iACP1WorkTime);
                }
            }
        }
        else{
            if(bState){
                if(bACP2CheckFlag && RUNTIME_JUDGING == iACP2ResultFlag && !timerACP2->isActive()){
                    timerACP2->start(ACP_CHECK_TOME_OUT);
                }
            }
            else{
                if(RUNTIME_JUDGING == iACP2ResultFlag && timerACP2->isActive()){
                    timerACP2->stop();
                    bACP2CheckFlag = false;
                    if(!bACP1CheckFlag){
                        bCheckStartFlag = false;
                    }
                    iACP2ResultFlag = RUNTIME_50SEC;
                    setActionTime(carId, iACP2ResultFlag, iACP2WorkTime);
                }
            }
        }
    }
}

void airpumpcheck::slot_ACP1_Check_TimeOut(){
//    if(bACP1WorkFlag){
    iACP1WorkTime++;
//    }
//    else{
//        timerACP1->stop();
//        iACP1WorkTime = 0;
//    }
}

void airpumpcheck::slot_ACP2_Check_TimeOut(){
//    if(bACP2WorkFlag){
    iACP2WorkTime++;
//    }
//    else{
//        timerACP2->stop();
//        iACP2WorkTime = 0;
//    }
}
