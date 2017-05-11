#include "speedcheckchange.h"
#include "ui_speedcheckchange.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

speedcheckchange::speedcheckchange(int iFlag, speedcheck *tempSpeedCheck, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::speedcheckchange)
{
    ui->setupUi(this);
    mSpeedCheck = tempSpeedCheck;
    showFlag = iFlag;
    iShowIndex = 0;

    Init();
    setScreenShow();

    timer5S = new QTimer();
    connect(timer5S, SIGNAL(timeout()), this, SLOT(slot_TimerOut_5S()));

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));
    if(2 == showFlag){
        timer5S->start(BACK_TIME);
    }
}

speedcheckchange::~speedcheckchange(){
    delete ui;
}

void speedcheckchange::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

    if(mSpeedCheck->bStartCheckFlag){
        ui->buttonstart->setStyleSheet(QSS_PBTN_SELECTED_GREY);
    }
    else{
        ui->buttonstart->setStyleSheet(QSS_PBTN_NO_SELECTED);
    }

    ui->labelcarId->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[0]));
    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void speedcheckchange::setRowLabelText(int index, structSpeedCheck *speedData){
    QLabel * tempLabel1 = NULL;
    QLabel * tempLabel2 = NULL;
    QLabel * tempLabel3 = NULL;
    QLabel * tempLabel4 = NULL;
    QLabel * tempLabel5 = NULL;
    QLabel * tempLabel6 = NULL;
    QLabel * tempLabel7 = NULL;

    switch (index) {
    case 0:
        tempLabel1 = ui->labelinfo_2_2;
        tempLabel2 = ui->labelinfo_2_3;
        tempLabel3 = ui->labelinfo_2_4;
        tempLabel4 = ui->labelinfo_2_5;
        tempLabel5 = ui->labelinfo_2_6;
        tempLabel6 = ui->labelinfo_2_7;
        tempLabel7 = ui->labelinfo_2_8;
        break;
    case 1:
        tempLabel1 = ui->labelinfo_3_2;
        tempLabel2 = ui->labelinfo_3_3;
        tempLabel3 = ui->labelinfo_3_4;
        tempLabel4 = ui->labelinfo_3_5;
        tempLabel5 = ui->labelinfo_3_6;
        tempLabel6 = ui->labelinfo_3_7;
        tempLabel7 = ui->labelinfo_3_8;
        break;
    case 2:
        tempLabel1 = ui->labelinfo_4_2;
        tempLabel2 = ui->labelinfo_4_3;
        tempLabel3 = ui->labelinfo_4_4;
        tempLabel4 = ui->labelinfo_4_5;
        tempLabel5 = ui->labelinfo_4_6;
        tempLabel6 = ui->labelinfo_4_7;
        tempLabel7 = ui->labelinfo_4_8;
        break;
    case 3:
        tempLabel1 = ui->labelinfo_5_2;
        tempLabel2 = ui->labelinfo_5_3;
        tempLabel3 = ui->labelinfo_5_4;
        tempLabel4 = ui->labelinfo_5_5;
        tempLabel5 = ui->labelinfo_5_6;
        tempLabel6 = ui->labelinfo_5_7;
        tempLabel7 = ui->labelinfo_5_8;
        break;
    case 4:
        tempLabel1 = ui->labelinfo_6_2;
        tempLabel2 = ui->labelinfo_6_3;
        tempLabel3 = ui->labelinfo_6_4;
        tempLabel4 = ui->labelinfo_6_5;
        tempLabel5 = ui->labelinfo_6_6;
        tempLabel6 = ui->labelinfo_6_7;
        tempLabel7 = ui->labelinfo_6_8;
        break;
    case 5:
        tempLabel1 = ui->labelinfo_7_2;
        tempLabel2 = ui->labelinfo_7_3;
        tempLabel3 = ui->labelinfo_7_4;
        tempLabel4 = ui->labelinfo_7_5;
        tempLabel5 = ui->labelinfo_7_6;
        tempLabel6 = ui->labelinfo_7_7;
        tempLabel7 = ui->labelinfo_7_8;
        break;
    case 6:
        tempLabel1 = ui->labelinfo_8_2;
        tempLabel2 = ui->labelinfo_8_3;
        tempLabel3 = ui->labelinfo_8_4;
        tempLabel4 = ui->labelinfo_8_5;
        tempLabel5 = ui->labelinfo_8_6;
        tempLabel6 = ui->labelinfo_8_7;
        tempLabel7 = ui->labelinfo_8_8;
        break;
    case 7:
        tempLabel1 = ui->labelinfo_9_2;
        tempLabel2 = ui->labelinfo_9_3;
        tempLabel3 = ui->labelinfo_9_4;
        tempLabel4 = ui->labelinfo_9_5;
        tempLabel5 = ui->labelinfo_9_6;
        tempLabel6 = ui->labelinfo_9_7;
        tempLabel7 = ui->labelinfo_9_8;
        break;
    case 8:
        tempLabel1 = ui->labelinfo_10_2;
        tempLabel2 = ui->labelinfo_10_3;
        tempLabel3 = ui->labelinfo_10_4;
        tempLabel4 = ui->labelinfo_10_5;
        tempLabel5 = ui->labelinfo_10_6;
        tempLabel6 = ui->labelinfo_10_7;
        tempLabel7 = ui->labelinfo_10_8;
        break;
    case 9:
        tempLabel1 = ui->labelinfo_11_2;
        tempLabel2 = ui->labelinfo_11_3;
        tempLabel3 = ui->labelinfo_11_4;
        tempLabel4 = ui->labelinfo_11_5;
        tempLabel5 = ui->labelinfo_11_6;
        tempLabel6 = ui->labelinfo_11_7;
        tempLabel7 = ui->labelinfo_11_8;
        break;
    default:
        tempLabel1 = NULL;
        tempLabel2 = NULL;
        tempLabel3 = NULL;
        tempLabel4 = NULL;
        tempLabel5 = NULL;
        tempLabel6 = NULL;
        tempLabel7 = NULL;
        break;
    }
    if(NULL == tempLabel1 || NULL == tempLabel2 || NULL == tempLabel3 || NULL == tempLabel4 || NULL == tempLabel5 || NULL == tempLabel6 || NULL == tempLabel7){
        return ;
    }

    QString strText1 = speedData->strDate + " " + speedData->strTime;
    QString strText2 = (1 == speedData->iCheckType) ? "加速度1" : (2 == speedData->iCheckType) ? "加速度2" : (3 == speedData->iCheckType) ? "减速度" : "";
    QString strText3 = QString::number(speedData->iSpeedValue/100.0);
    QString strText4 = ((speedData->iSpeedValue > 0) ? "牵引" : "制动") + QString("%1%").arg(speedData->iPWMValue);
    QString strText5 = (3 != speedData->iCheckType) ? "" : (1 == speedData->iRebornFlag) ? "无" : (2 == speedData->iRebornFlag) ? "有" : "";
    QString strText6 = (3 != speedData->iCheckType) ? "" : QString("%1").arg(speedData->iBrakeSpeed);
    QString strText7 = (3 != speedData->iCheckType) ? "" : QString("%1").arg(speedData->iBrakeDistance);

    tempLabel1->setText(strText1);
    tempLabel2->setText(strText2);
    tempLabel3->setText(strText3);
    tempLabel4->setText(strText4);
    tempLabel5->setText(strText5);
    tempLabel6->setText(strText6);
    tempLabel7->setText(strText7);

    tempLabel1 = NULL;
    tempLabel2 = NULL;
    tempLabel3 = NULL;
    tempLabel4 = NULL;
    tempLabel5 = NULL;
    tempLabel6 = NULL;
    tempLabel7 = NULL;
}

void speedcheckchange::cleanScreenShow(){
    ui->labelinfo_2_2->setText("");
    ui->labelinfo_2_3->setText("");
    ui->labelinfo_2_4->setText("");
    ui->labelinfo_2_5->setText("");
    ui->labelinfo_2_6->setText("");
    ui->labelinfo_2_7->setText("");
    ui->labelinfo_2_8->setText("");

    ui->labelinfo_3_2->setText("");
    ui->labelinfo_3_3->setText("");
    ui->labelinfo_3_4->setText("");
    ui->labelinfo_3_5->setText("");
    ui->labelinfo_3_6->setText("");
    ui->labelinfo_3_7->setText("");
    ui->labelinfo_3_8->setText("");

    ui->labelinfo_4_2->setText("");
    ui->labelinfo_4_3->setText("");
    ui->labelinfo_4_4->setText("");
    ui->labelinfo_4_5->setText("");
    ui->labelinfo_4_6->setText("");
    ui->labelinfo_4_7->setText("");
    ui->labelinfo_4_8->setText("");

    ui->labelinfo_5_2->setText("");
    ui->labelinfo_5_3->setText("");
    ui->labelinfo_5_4->setText("");
    ui->labelinfo_5_5->setText("");
    ui->labelinfo_5_6->setText("");
    ui->labelinfo_5_7->setText("");
    ui->labelinfo_5_8->setText("");

    ui->labelinfo_6_2->setText("");
    ui->labelinfo_6_3->setText("");
    ui->labelinfo_6_4->setText("");
    ui->labelinfo_6_5->setText("");
    ui->labelinfo_6_6->setText("");
    ui->labelinfo_6_7->setText("");
    ui->labelinfo_6_8->setText("");

    ui->labelinfo_7_2->setText("");
    ui->labelinfo_7_3->setText("");
    ui->labelinfo_7_4->setText("");
    ui->labelinfo_7_5->setText("");
    ui->labelinfo_7_6->setText("");
    ui->labelinfo_7_7->setText("");
    ui->labelinfo_7_8->setText("");

    ui->labelinfo_8_2->setText("");
    ui->labelinfo_8_3->setText("");
    ui->labelinfo_8_4->setText("");
    ui->labelinfo_8_5->setText("");
    ui->labelinfo_8_6->setText("");
    ui->labelinfo_8_7->setText("");
    ui->labelinfo_8_8->setText("");

    ui->labelinfo_9_2->setText("");
    ui->labelinfo_9_3->setText("");
    ui->labelinfo_9_4->setText("");
    ui->labelinfo_9_5->setText("");
    ui->labelinfo_9_6->setText("");
    ui->labelinfo_9_7->setText("");
    ui->labelinfo_9_8->setText("");

    ui->labelinfo_10_2->setText("");
    ui->labelinfo_10_3->setText("");
    ui->labelinfo_10_4->setText("");
    ui->labelinfo_10_5->setText("");
    ui->labelinfo_10_6->setText("");
    ui->labelinfo_10_7->setText("");
    ui->labelinfo_10_8->setText("");

    ui->labelinfo_11_2->setText("");
    ui->labelinfo_11_3->setText("");
    ui->labelinfo_11_4->setText("");
    ui->labelinfo_11_5->setText("");
    ui->labelinfo_11_6->setText("");
    ui->labelinfo_11_7->setText("");
    ui->labelinfo_11_8->setText("");
}

void speedcheckchange::setScreenShow(){
    cleanScreenShow();
    ui->labelinfo_2_1->setText(QString("%1").arg(iShowIndex+1));
    ui->labelinfo_3_1->setText(QString("%1").arg(iShowIndex+2));
    ui->labelinfo_4_1->setText(QString("%1").arg(iShowIndex+3));
    ui->labelinfo_5_1->setText(QString("%1").arg(iShowIndex+4));
    ui->labelinfo_6_1->setText(QString("%1").arg(iShowIndex+5));
    ui->labelinfo_7_1->setText(QString("%1").arg(iShowIndex+6));
    ui->labelinfo_8_1->setText(QString("%1").arg(iShowIndex+7));
    ui->labelinfo_9_1->setText(QString("%1").arg(iShowIndex+8));
    ui->labelinfo_10_1->setText(QString("%1").arg(iShowIndex+9));
    ui->labelinfo_11_1->setText(QString("%1").arg(iShowIndex+10));

    int iShowCount = 0, iDataCount = mSpeedCheck->speedDataList.length();
    if(iDataCount <= iShowIndex + 10 ){
        iShowCount = iDataCount;
    }
    else{
        iShowCount = iShowIndex + 10;
    }
    for(int i = 0, j = iShowIndex; j < iShowCount; i++, j++){
        setRowLabelText(i, mSpeedCheck->speedDataList.at(j));
    }
}

void speedcheckchange::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void speedcheckchange::on_buttonstart_clicked(){
    if(!mSpeedCheck->bStartCheckFlag){
        buttonBeep();
        ui->buttonstart->setStyleSheet(QSS_PBTN_SELECTED_GREY);
        emit sig_PBtn_Start_Clicked();
    }
}

void speedcheckchange::on_buttonstop_clicked(){
    if(mSpeedCheck->bStartCheckFlag){
        buttonBeep();
        ui->buttonstart->setStyleSheet(QSS_PBTN_NO_SELECTED);
        emit sig_PBtn_Stop_Clicked();
        setScreenShow();
    }
}

void speedcheckchange::on_buttonchange_clicked(){
    if(timer5S->isActive()){
        timer5S->stop();
    }
    buttonBeep();
    mSpeedCheck->bNextScreenFlag = false;

    this->reject();
}

void speedcheckchange::on_buttonpreviouspage_clicked(){
    if(iShowIndex > 0){
        iShowIndex = iShowIndex - 10;
        setScreenShow();
        buttonBeep();
    }
}

void speedcheckchange::on_buttonnextpage_clicked(){
    if(iShowIndex+10 < mSpeedCheck->speedDataList.length()){
        iShowIndex = iShowIndex + 10;
        setScreenShow();
        buttonBeep();
    }
}

void speedcheckchange::on_buttonquit_clicked(){
    buttonBeep();
    this->reject();

    emit sig_Closed();
}

void speedcheckchange::slot_TimerOut_5S(){
    mSpeedCheck->bNextScreenFlag = false;

    this->reject();
}

void speedcheckchange::slot_SpeedCheckData_Update(){
    setScreenShow();
}
