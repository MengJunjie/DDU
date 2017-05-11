#include "sivcheck.h"
#include "ui_sivcheck.h"
#include "../objectdictdata.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

sivcheck::sivcheck(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sivcheck)
{
    ui->setupUi(this);

    bCheckStartFlag = false;
    bCar2SIVCheckFlag = false;
    bCar4SIVCheckFlag = false;
    bCar2SIVValidFlag = false;
    bCar4SIVValidFlag = false;
    iCar2SIVCheckTime = 0;
    iCar4SIVCheckTime = 0;
    iCar2SIVCheckResult = 0;
    iCar4SIVCheckResult = 0;
    for(int i = 0; i < 3; i++){
        iCar2SIVData[i] = 0;
        iCar4SIVData[i] = 0;
    }
    iCar2SIVData[0] = -1;
    iCar4SIVData[0] = -1;

    Init();
    slot_CarIcon_Update();
    slot_CarCode_Update();
    setVoltage();
    setCurrent();

    timerCar2SIV = new QTimer();
    connect(timerCar2SIV, SIGNAL(timeout()), this, SLOT(slot_Car2SIV_TimeOut()));
    timerCar4SIV = new QTimer();
    connect(timerCar4SIV, SIGNAL(timeout()), this, SLOT(slot_Car4SIV_TimeOut()));

    connect(canSignalsSlots, SIGNAL(sig_Car_Code_Change()), this, SLOT(slot_CarCode_Update()));
    connect(canSignalsSlots, SIGNAL(sig_Car_Direction_Change()), this, SLOT(slot_Car_Direction_Update()));
    connect(canSignalsSlots, SIGNAL(sig_SIV_Voltage_Freq_Update(CAR_ID,int,int,int)), this, SLOT(slot_SIV_Voltage_Freq_Update(CAR_ID,int,int,int)));

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));
    connect(this, SIGNAL(sig_SIV_Check_Start()), canSignalsSlots, SLOT(slot_SIV_Check_Start()));

    emit sig_SIV_Check_Start();
}

sivcheck::~sivcheck(){
    delete ui;
}

void  sivcheck::Init(){
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

void sivcheck::setVoltage(){
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

void sivcheck::setCurrent(){
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

void sivcheck::setSIVOutVoltage(CAR_ID carId, SIV_STATE iState, int iValue){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    QLabel * tempLabel = NULL;
    switch (realCarId) {
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
    default:
        tempLabel = NULL;
        break;
    }
    if(CAR_2 == carId){
        ui->labelinfo_2_3->setText("");
        ui->labelinfo_2_3->setStyleSheet("");
        ui->labelinfo_2_6->setText("");
        ui->labelinfo_2_6->setStyleSheet("");
    }
    else if(CAR_4 == carId){
        ui->labelinfo_2_5->setText("");
        ui->labelinfo_2_5->setStyleSheet("");
        ui->labelinfo_2_4->setText("");
        ui->labelinfo_2_4->setStyleSheet("");
    }
    else{
        // do nothing
    }
    if(NULL == tempLabel){
        return ;
    }
    QString strText = "", strStyleSheet = "";
    switch (iState) {
    case SIV_COMN_ERR:
        strText = "";
        strStyleSheet = QSS_BG_PURPLE_BR_WHITE_WD_WHITE;
        break;
    case SIV_BREAKDOWN:
        strText = canSignalsSlots->getSIVErrorInfo(iValue);
        strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
        break;
    case SIV_POWER_OFF:
        strText = "OFF";
        strStyleSheet = QSS_BG_DARK_RED_BR_WHITE_WD_WHITE;
        break;
    case SIV_NORMAL:
        strText = QString("%1V").arg(iValue);
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

void sivcheck::setSIVOutFreq(CAR_ID carId, SIV_STATE iState, int iValue){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    QLabel * tempLabel = NULL;
    switch (realCarId) {
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
    default:
        tempLabel = NULL;
        break;
    }
    if(CAR_2 == carId){
        ui->labelinfo_3_3->setText("");
        ui->labelinfo_3_6->setText("");
    }
    else if(CAR_4 == carId){
        ui->labelinfo_3_5->setText("");
        ui->labelinfo_3_4->setText("");
    }
    else{
        // do nothing
    }
    if(NULL == tempLabel){
        return ;
    }
    QString strText = "";
    if(SIV_NORMAL == iState){
        strText = QString("%1Hz").arg(iValue);
    }
    else{
        strText = "";
    }
    tempLabel->setText(strText);
    tempLabel = NULL;
}

void sivcheck::setSIVCheckResult(CAR_ID carId, SIVCHECK_RESULT iRestlt){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    QLabel * tempLabel = NULL;
    switch (realCarId) {
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
    default:
        tempLabel = NULL;
        break;
    }
    if(CAR_2 == carId){
        ui->labelinfo_4_3->setText("");
        ui->labelinfo_4_3->setStyleSheet("");
        ui->labelinfo_4_6->setText("");
        ui->labelinfo_4_6->setStyleSheet("");
    }
    else if(CAR_4 == carId){
        ui->labelinfo_4_5->setText("");
        ui->labelinfo_4_5->setStyleSheet("");
        ui->labelinfo_4_4->setText("");
        ui->labelinfo_4_4->setStyleSheet("");
    }
    else{
        // do nothing
    }
    if(NULL == tempLabel){
        return ;
    }
    QString strText = "", strStyleSheet = "";
    switch (iRestlt) {
    case SIV_RESULT_STANDBY:
        strText = "";
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        break;
    case SIV_RESULT_JUDGING:
        strText = "判断中";
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        break;
    case SIV_RESULT_OK:
        strText = "OK";
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        break;
    case SIV_RESULT_NG:
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
    tempLabel = NULL;
}

void sivcheck::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
    setVoltage();
    setCurrent();
}

void sivcheck::on_buttonstrart_clicked(){
    if(bCheckStartFlag){
        return;
    }
    bCheckStartFlag = true;
    bool bBeepFlag = false;
    if(!bCar2SIVCheckFlag){
        bBeepFlag = true;
        if(bCar2SIVValidFlag){
            iCar2SIVCheckResult = 1;
            iCar2SIVCheckTime = 0;
            timerCar2SIV->start(SIV_CHECK_TIMEOUT);
            setSIVCheckResult(CAR_2, SIVCHECK_RESULT(iCar2SIVCheckResult));
            bCar2SIVCheckFlag = true;
        }
        else{
            iCar2SIVCheckResult = 3;
            setSIVCheckResult(CAR_2, SIVCHECK_RESULT(iCar2SIVCheckResult));
        }
    }

    if(!bCar4SIVCheckFlag){
        bBeepFlag = true;
        if(bCar4SIVValidFlag){
            iCar4SIVCheckResult = 1;
            iCar4SIVCheckTime = 0;
            timerCar4SIV->start(SIV_CHECK_TIMEOUT);
            setSIVCheckResult(CAR_4, SIVCHECK_RESULT(iCar4SIVCheckResult));
            bCar4SIVCheckFlag = true;
        }
        else{
            iCar4SIVCheckResult = 3;
            setSIVCheckResult(CAR_4, SIVCHECK_RESULT(iCar4SIVCheckResult));
        }
    }
    if(!bCar2SIVCheckFlag && !bCar4SIVCheckFlag){
        bCheckStartFlag = false;
    }
    if(bBeepFlag){
        buttonBeep();
        ui->buttonrecord->setEnabled(true);
    }
}

void sivcheck::on_buttonstop_clicked(){
    if(!bCheckStartFlag){
        return;
    }
    bCheckStartFlag = false;
    if(bCar2SIVCheckFlag){
        bCar2SIVCheckFlag = false;
        timerCar2SIV->stop();
        iCar2SIVCheckTime = 0;
        iCar2SIVCheckResult = 3;
        setSIVCheckResult(CAR_2, SIVCHECK_RESULT(iCar2SIVCheckResult));
    }
    if(bCar4SIVCheckFlag){
        bCar4SIVCheckFlag = false;
        timerCar4SIV->stop();
        iCar4SIVCheckTime = 0;
        iCar4SIVCheckResult = 3;
        setSIVCheckResult(CAR_4, SIVCHECK_RESULT(iCar4SIVCheckResult));
    }
    buttonBeep();
}

void sivcheck::on_buttonrecord_clicked(){
//    ui->buttonrecord->setEnabled(false);
    buttonBeep();
    SIVCheck_Data[0] = iCar2SIVCheckResult;
    SIVCheck_Data[1] = iCar4SIVCheckResult;

    canSignalsSlots->sendSDOCheckData(3);
}

void sivcheck::on_buttonquit_clicked(){
    buttonBeep();
    this->reject();
}

void sivcheck::slot_CarIcon_Update(){
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

void sivcheck::slot_CarCode_Update(){
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

void sivcheck::slot_Car_Direction_Update(){
    slot_CarIcon_Update();
    slot_CarCode_Update();

    setSIVOutVoltage(CAR_2, SIV_STATE(iCar2SIVData[0]), iCar2SIVData[1]);
    setSIVOutFreq(CAR_2, SIV_STATE(iCar2SIVData[0]), iCar2SIVData[2]);
    setSIVOutVoltage(CAR_4, SIV_STATE(iCar4SIVData[0]), iCar4SIVData[1]);
    setSIVOutFreq(CAR_4, SIV_STATE(iCar4SIVData[0]), iCar4SIVData[2]);
    setSIVCheckResult(CAR_2, SIVCHECK_RESULT(iCar2SIVCheckResult));
    setSIVCheckResult(CAR_4, SIVCHECK_RESULT(iCar4SIVCheckResult));
}

void sivcheck::slot_SIV_Voltage_Freq_Update(CAR_ID carId, int iState, int iValue1, int iValue2){
    bool bValidFlag = false;
    if(3 == iState && (iValue1 >= 342 && iValue1 <= 418) && (iValue2 >= 47.5 && iValue2 <= 52.5)){
        bValidFlag = true;
    }
    else{
        bValidFlag = false;
    }

    SIV_STATE tempState = SIV_STATE(iState);
    if(CAR_2 == carId){
        iCar2SIVData[0] = iState;
        iCar2SIVData[1] = iValue1;
        iCar2SIVData[2] = iValue2;
        bCar2SIVValidFlag = bValidFlag;
        setSIVOutVoltage(carId, tempState, iValue1);
        setSIVOutFreq(carId, tempState, iValue2);
    }

    if(CAR_4 == carId){
        iCar4SIVData[0] = iState;
        iCar4SIVData[1] = iValue1;
        iCar4SIVData[2] = iValue2;
        bCar4SIVValidFlag = bValidFlag;
        setSIVOutVoltage(carId, tempState, iValue1);
        setSIVOutFreq(carId, tempState, iValue2);
    }

    if(bCheckStartFlag){
        if(SIV_NORMAL == tempState && !bValidFlag){
            if(CAR_2 == carId && 1 == iCar2SIVCheckResult){
                timerCar2SIV->stop();
                iCar2SIVCheckResult = 3;
                bCar2SIVCheckFlag = false;
                if(0 != iCar4SIVCheckResult && 1 != iCar4SIVCheckResult && !bCar4SIVCheckFlag){
                    bCheckStartFlag = false;
                }
                setSIVCheckResult(carId, SIVCHECK_RESULT(iCar2SIVCheckResult));
            }

            if(CAR_4 == carId && 1 == iCar4SIVCheckResult){
                timerCar4SIV->stop();
                iCar4SIVCheckResult = 3;
                bCar4SIVCheckFlag = false;
                if(0 != iCar2SIVCheckResult && 1 != iCar2SIVCheckResult && !bCar2SIVCheckFlag){
                    bCheckStartFlag = false;
                }
                setSIVCheckResult(carId, SIVCHECK_RESULT(iCar4SIVCheckResult));
            }
        }
    }
}

void sivcheck::slot_Car2SIV_TimeOut(){
    iCar2SIVCheckTime++;

    if(60 == iCar2SIVCheckTime){
        timerCar2SIV->stop();
        bCar2SIVCheckFlag = false;
        iCar2SIVCheckResult = 2;
        setSIVCheckResult(CAR_2, SIV_RESULT_OK);
    }
}

void sivcheck::slot_Car4SIV_TimeOut(){
    iCar4SIVCheckTime++;

    if(60 == iCar4SIVCheckTime){
        timerCar4SIV->stop();
        bCar4SIVCheckFlag = false;
        iCar4SIVCheckResult = 2;
        setSIVCheckResult(CAR_4, SIV_RESULT_OK);
    }
}
