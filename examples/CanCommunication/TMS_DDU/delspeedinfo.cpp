#include "delspeedinfo.h"
#include "ui_delspeedinfo.h"
#include "QStandardItemModel"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

delspeedinfo::delspeedinfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::delspeedinfo)
{
    ui->setupUi(this);

    iShowIndex = 0;
    iShowLength = 0;

    bShowTimeFlag = false;
    bGetAllFlag = false;

    timer_1S = new QTimer();
    connect(timer_1S, SIGNAL(timeout()), this, SLOT(slot_1S_Timer_Out_Show()));

    Init();

    connect(canSignalsSlots, SIGNAL(sig_SpeedRecordData_Get_Update(int)), this, SLOT(slot_SpeedRecordData_Get_Update(int)));

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));

    canSignalsSlots->bNowShowSpeedFlag = true;

    canSignalsSlots->sendCarGetFaultInfoHMI(3);
}

delspeedinfo::~delspeedinfo(){
    delete ui;
}

void delspeedinfo::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void delspeedinfo::setRowLabelText(int index, structSpeedCheck *speedData){
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
    QString strText4 = ((3 == speedData->iCheckType) ? "制动" : "牵引") + QString("%1%").arg(speedData->iPWMValue);
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

void delspeedinfo::cleanScreenShow(){
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

void delspeedinfo::setScreenShow(){
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

    int iShowCount = 0;
    if(iShowLength <= iShowIndex + 10 ){
        iShowCount = iShowLength;
    }
    else{
        iShowCount = iShowIndex + 10;
    }
    for(int i = 0, j = iShowIndex; j < iShowCount; i++, j++){
        setRowLabelText(i, canSignalsSlots->carSpeedDataList.at(j));
    }
}

void delspeedinfo::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void delspeedinfo::slot_1S_Timer_Out_Show(){
    int tempLength = canSignalsSlots->carSpeedDataList.length();
    qDebug()<<"HMI Get Speed Data Length == "<<tempLength;

    if(0 != tempLength && tempLength != iShowLength){
        iShowLength = tempLength;
        if(20 == iShowLength){
            timer_1S->stop();
            setScreenShow();
        }
    }
    else if(tempLength == iShowLength && 0 != tempLength){
        if(!bGetAllFlag){
            bGetAllFlag = true;
            setScreenShow();
        }
        else{
            timer_1S->stop();
        }
    }
    else{
        //do bothing
    }
//    setScreenShow();
}

void delspeedinfo::on_buttondelete_clicked(){
    if(iShowLength > 0){
        canSignalsSlots->sendCarDeleteRecordDataHMI(3, 0);
        buttonBeep();
    }
}

void delspeedinfo::on_buttonpreviouspage_clicked(){
    if(iShowIndex > 0){
        iShowIndex = iShowIndex - 10;
        setScreenShow();
        buttonBeep();
    }
}

void delspeedinfo::on_buttonnextpage_clicked(){
    if(iShowIndex+10 < iShowLength){
        iShowIndex = iShowIndex + 10;
        setScreenShow();
        buttonBeep();
    }
}

void delspeedinfo::on_buttonquit_clicked(){
    canSignalsSlots->bNowShowSpeedFlag = false;
    buttonBeep();
    this->reject();
}

void delspeedinfo::slot_SpeedRecordData_Get_Update(int flag){
    if(0 == flag){
        cleanScreenShow();
        bShowTimeFlag = false;
    }
    else if(1 == flag){
        if(!bShowTimeFlag){
            bShowTimeFlag = true;
            timer_1S->start(1000);
        }
//        iShowLength = canSignalsSlots->carSpeedDataList.length();
//        setScreenShow();
    }
    else{
        //do nothing
    }
}
