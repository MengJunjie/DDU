#include "errorrecord.h"
#include "ui_errorrecord.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

errorrecord::errorrecord(int iCarId, QString carName, int errCount, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::errorrecord)
{
    ui->setupUi(this);
    iErrCarId = iCarId;
    strCarName = carName;
    iShowIndex = 0;
    iShowLength = 0;
    iErrCount = errCount;

    Init();

    connect(canSignalsSlots, SIGNAL(sig_FaultRecordData_Get_Update()), this, SLOT(slot_FaultRecordData_Get_Update()));

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));
}

errorrecord::~errorrecord(){
    delete ui;
}

void errorrecord::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

    ui->labelcarId->setText(strCarName);

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void errorrecord::setRowLabelText(int index, structFaultRecordData *mFaultData){
    QLabel * tempLabel1 = NULL;
    QLabel * tempLabel2 = NULL;
    QLabel * tempLabel3 = NULL;
    QLabel * tempLabel4 = NULL;
    QLabel * tempLabel5 = NULL;
    QLabel * tempLabel6 = NULL;

    switch (index) {
    case 0:
        tempLabel1 = ui->labelinfo_2_2;
        tempLabel2 = ui->labelinfo_2_3;
        tempLabel3 = ui->labelinfo_2_4;
        tempLabel4 = ui->labelinfo_2_5;
        tempLabel5 = ui->labelinfo_2_6;
        tempLabel6 = ui->labelinfo_2_7;
        break;
    case 1:
        tempLabel1 = ui->labelinfo_3_2;
        tempLabel2 = ui->labelinfo_3_3;
        tempLabel3 = ui->labelinfo_3_4;
        tempLabel4 = ui->labelinfo_3_5;
        tempLabel5 = ui->labelinfo_3_6;
        tempLabel6 = ui->labelinfo_3_7;
        break;
    case 2:
        tempLabel1 = ui->labelinfo_4_2;
        tempLabel2 = ui->labelinfo_4_3;
        tempLabel3 = ui->labelinfo_4_4;
        tempLabel4 = ui->labelinfo_4_5;
        tempLabel5 = ui->labelinfo_4_6;
        tempLabel6 = ui->labelinfo_4_7;
        break;
    case 3:
        tempLabel1 = ui->labelinfo_5_2;
        tempLabel2 = ui->labelinfo_5_3;
        tempLabel3 = ui->labelinfo_5_4;
        tempLabel4 = ui->labelinfo_5_5;
        tempLabel5 = ui->labelinfo_5_6;
        tempLabel6 = ui->labelinfo_5_7;
        break;
    case 4:
        tempLabel1 = ui->labelinfo_6_2;
        tempLabel2 = ui->labelinfo_6_3;
        tempLabel3 = ui->labelinfo_6_4;
        tempLabel4 = ui->labelinfo_6_5;
        tempLabel5 = ui->labelinfo_6_6;
        tempLabel6 = ui->labelinfo_6_7;
        break;
    default:
        tempLabel1 = NULL;
        tempLabel2 = NULL;
        tempLabel3 = NULL;
        tempLabel4 = NULL;
        tempLabel5 = NULL;
        tempLabel6 = NULL;
        break;
    }
    if(NULL == tempLabel1 || NULL == tempLabel2 || NULL == tempLabel3 || NULL == tempLabel4 || NULL == tempLabel5 || NULL == tempLabel6){
        return ;
    }

    QString strText1 = mFaultData->strDate + "\n" + mFaultData->strTime;
    QString strStationName = canSignalsSlots->getCarStationName(mFaultData->iErrStationCode);
    QString strDistacne = QString("%1m").arg(mFaultData->iErrDistance);
    QString strText2 = strStationName + "\n" + strDistacne;
    QString strText3 = mFaultData->strErrDevName + "\n" + mFaultData->strErrCode;
    QString strText4 = "";
    switch (mFaultData->iTracBrakeLevel) {
    case 0:
        strText4 = "通讯异常";
        break;
    case 1:
        strText4 = "紧急制动";
        break;
    case 2:
        strText4 = QString("制动%1%").arg(mFaultData->iPWMValue);
        break;
    case 3:
        strText4 = "OFF";
        break;
    case 4:
        strText4 = QString("牵引%1%").arg(mFaultData->iPWMValue);
        break;
    default:
        strText4 = "";
        break;
    }
    QString strText5 = QString("%1\nKm/h").arg(mFaultData->iSpeedValue);
    QString strText6 = QString("%1\nV").arg(mFaultData->iVoltage);

    tempLabel1->setText(strText1);
    tempLabel2->setText(strText2);
    tempLabel3->setText(strText3);
    tempLabel4->setText(strText4);
    tempLabel5->setText(strText5);
    tempLabel6->setText(strText6);

    tempLabel1 = NULL;
    tempLabel2 = NULL;
    tempLabel3 = NULL;
    tempLabel4 = NULL;
    tempLabel5 = NULL;
    tempLabel6 = NULL;
}

void errorrecord::cleanScreenShow(){
    ui->labelinfo_2_2->setText("");
    ui->labelinfo_2_3->setText("");
    ui->labelinfo_2_4->setText("");
    ui->labelinfo_2_5->setText("");
    ui->labelinfo_2_6->setText("");
    ui->labelinfo_2_7->setText("");

    ui->labelinfo_3_2->setText("");
    ui->labelinfo_3_3->setText("");
    ui->labelinfo_3_4->setText("");
    ui->labelinfo_3_5->setText("");
    ui->labelinfo_3_6->setText("");
    ui->labelinfo_3_7->setText("");

    ui->labelinfo_4_2->setText("");
    ui->labelinfo_4_3->setText("");
    ui->labelinfo_4_4->setText("");
    ui->labelinfo_4_5->setText("");
    ui->labelinfo_4_6->setText("");
    ui->labelinfo_4_7->setText("");

    ui->labelinfo_5_2->setText("");
    ui->labelinfo_5_3->setText("");
    ui->labelinfo_5_4->setText("");
    ui->labelinfo_5_5->setText("");
    ui->labelinfo_5_6->setText("");
    ui->labelinfo_5_7->setText("");

    ui->labelinfo_6_2->setText("");
    ui->labelinfo_6_3->setText("");
    ui->labelinfo_6_4->setText("");
    ui->labelinfo_6_5->setText("");
    ui->labelinfo_6_6->setText("");
    ui->labelinfo_6_7->setText("");
}

void errorrecord::setScreenShow(){
    int iShowCount = 0;
    if(iShowLength <= iShowIndex + 5 ){
        iShowCount = iShowLength;
    }
    else{
        iShowCount = iShowIndex + 5;
    }

    cleanScreenShow();
    ui->labelinfo_2_1->setText(QString("%1").arg(iShowIndex+1));
    ui->labelinfo_3_1->setText(QString("%1").arg(iShowIndex+2));
    ui->labelinfo_4_1->setText(QString("%1").arg(iShowIndex+3));
    ui->labelinfo_5_1->setText(QString("%1").arg(iShowIndex+4));
    ui->labelinfo_6_1->setText(QString("%1").arg(iShowIndex+5));

    switch (CAR_ID(iErrCarId)) {
    case CAR_1:
        for(int i = 0, j = iShowIndex; j < iShowCount; i++, j++){
            setRowLabelText(i, canSignalsSlots->car1FaultList.at(j));
        }
        break;
    case CAR_2:
        for(int i = 0, j = iShowIndex; j < iShowCount; i++, j++){
            setRowLabelText(i, canSignalsSlots->car2FaultList.at(j));
        }
        break;
    case CAR_3:
        for(int i = 0, j = iShowIndex; j < iShowCount; i++, j++){
            setRowLabelText(i, canSignalsSlots->car3FaultList.at(j));
        }
        break;
    case CAR_4:
        for(int i = 0, j = iShowIndex; j < iShowCount; i++, j++){
            setRowLabelText(i, canSignalsSlots->car4FaultList.at(j));
        }
        break;
    case CAR_5:
        for(int i = 0, j = iShowIndex; j < iShowCount; i++, j++){
            setRowLabelText(i, canSignalsSlots->car5FaultList.at(j));
        }
        break;
    case CAR_6:
        for(int i = 0, j = iShowIndex; j < iShowCount; i++, j++){
            setRowLabelText(i, canSignalsSlots->car6FaultList.at(j));
        }
        break;
    default:
        break;
    }
}

void errorrecord::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}
void errorrecord::on_buttonpreviouspage_clicked(){
    if(iShowIndex > 0){
        iShowIndex = iShowIndex - 5;
        setScreenShow();
        buttonBeep();
    }
}

void errorrecord::on_buttonnextpage_clicked(){
    if(iShowIndex+5 < iShowLength){
        iShowIndex = iShowIndex + 5;
        setScreenShow();
        buttonBeep();
    }
}

void errorrecord::on_buttonquit_clicked(){
    canSignalsSlots->bNowShowERMFaultFlag = false;
    buttonBeep();
    this->reject();
}

void errorrecord::slot_FaultRecordData_Get_Update(){
    switch (CAR_ID(iErrCarId)) {
    case CAR_1:
        iShowLength = canSignalsSlots->car1FaultList.length();
        break;
    case CAR_2:
        iShowLength = canSignalsSlots->car2FaultList.length();
        break;
    case CAR_3:
        iShowLength = canSignalsSlots->car3FaultList.length();
        break;
    case CAR_4:
        iShowLength = canSignalsSlots->car4FaultList.length();
        break;
    case CAR_5:
        iShowLength = canSignalsSlots->car5FaultList.length();
        break;
    case CAR_6:
        iShowLength = canSignalsSlots->car6FaultList.length();
        break;
    default:
        break;
    }

    if(iShowLength == iErrCount){
        qDebug()<<"AAAAAAAAAAAAAAAAaa   Show Fault INfo  in the Screen AAAAAAAa   Err Length == "<<iShowLength;
        setScreenShow();
    }
//    if(iShowIndex + 5 < iShowLength){
//        return ;
//    }

//    setScreenShow();
}
