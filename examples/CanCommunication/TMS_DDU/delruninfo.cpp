#include "delruninfo.h"
#include "ui_delruninfo.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

delruninfo::delruninfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::delruninfo)
{
    ui->setupUi(this);

    iDelItemIndex = 0;

    Init();

    connect(canSignalsSlots, SIGNAL(sig_RunRecordData_Get_Update()), this, SLOT(slot_RunRecordData_Get_Update()));

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));

    canSignalsSlots->sendCarGetFaultInfoHMI(2);
}

delruninfo::~delruninfo(){
    delete ui;
}

void delruninfo::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

    ui->labelinfo_2_2->setText("停止中");
    ui->labelinfo_3_2->setText("停止中");
    ui->labelinfo_4_2->setText("停止中");
    ui->labelinfo_5_2->setText("停止中");
    ui->labelinfo_6_2->setText("停止中");
    ui->labelinfo_7_2->setText("停止中");
    ui->labelinfo_8_2->setText("停止中");

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void delruninfo::cleanScreenShow(){
    ui->labelinfo_2_2->setText("");
    ui->labelinfo_2_3->setText("");
    ui->labelinfo_2_4->setText("");

    ui->labelinfo_3_2->setText("");
    ui->labelinfo_3_3->setText("");
    ui->labelinfo_3_4->setText("");

    ui->labelinfo_4_2->setText("");
    ui->labelinfo_4_3->setText("");
    ui->labelinfo_4_4->setText("");

    ui->labelinfo_5_2->setText("");
    ui->labelinfo_5_3->setText("");
    ui->labelinfo_5_4->setText("");

    ui->labelinfo_6_2->setText("");
    ui->labelinfo_6_3->setText("");
    ui->labelinfo_6_4->setText("");

    ui->labelinfo_7_2->setText("");
    ui->labelinfo_7_3->setText("");
    ui->labelinfo_7_4->setText("");

    ui->labelinfo_8_2->setText("");
    ui->labelinfo_8_3->setText("");
    ui->labelinfo_8_4->setText("");
}

void delruninfo::setScreenShow(){
    cleanScreenShow();

    if(0 == canSignalsSlots->carRunDataInfo.iErmComnState){
        ui->labelinfo_2_2->setText("记录中");
        ui->labelinfo_3_2->setText("记录中");
        ui->labelinfo_4_2->setText("记录中");
        ui->labelinfo_5_2->setText("记录中");
        ui->labelinfo_6_2->setText("记录中");
        ui->labelinfo_7_2->setText("记录中");
        ui->labelinfo_8_2->setText("记录中");
    }
    else{
        ui->labelinfo_2_2->setText("停止中");
        ui->labelinfo_3_2->setText("停止中");
        ui->labelinfo_4_2->setText("停止中");
        ui->labelinfo_5_2->setText("停止中");
        ui->labelinfo_6_2->setText("停止中");
        ui->labelinfo_7_2->setText("停止中");
        ui->labelinfo_8_2->setText("停止中");
    }
    ui->labelinfo_2_3->setText(canSignalsSlots->carRunDataInfo.strRunDataDate+" "+canSignalsSlots->carRunDataInfo.strRunDataTime);
    ui->labelinfo_2_4->setText(QString("%1小时").arg(canSignalsSlots->carRunDataInfo.u16RunData/10.0));
    ui->labelinfo_3_3->setText(canSignalsSlots->carRunDataInfo.strWalkDataDate+" "+canSignalsSlots->carRunDataInfo.strWalkDataTime);
    ui->labelinfo_3_4->setText(QString("%1日").arg(canSignalsSlots->carRunDataInfo.u16WalkData));
    ui->labelinfo_4_3->setText(canSignalsSlots->carRunDataInfo.strLoadRateDataDate+" "+canSignalsSlots->carRunDataInfo.strLoadRateDataTime);
    ui->labelinfo_4_4->setText(QString("%1站").arg(canSignalsSlots->carRunDataInfo.u16LoadRateData));
    ui->labelinfo_5_3->setText(canSignalsSlots->carRunDataInfo.strDistanceDataDate+" "+canSignalsSlots->carRunDataInfo.strDistanceDataTime);
    ui->labelinfo_5_4->setText(QString("%1km").arg(canSignalsSlots->carRunDataInfo.u32DistanceData/10.0));
    ui->labelinfo_6_3->setText(canSignalsSlots->carRunDataInfo.strWHMPDataDate+" "+canSignalsSlots->carRunDataInfo.strWHMPDataTime);
    ui->labelinfo_6_4->setText(QString("%1kwh").arg(canSignalsSlots->carRunDataInfo.u32WHMPData/10.0));
    ui->labelinfo_7_3->setText(canSignalsSlots->carRunDataInfo.strWHMBDataDate+" "+canSignalsSlots->carRunDataInfo.strWHMBDataTime);
    ui->labelinfo_7_4->setText(QString("%1kwh").arg(canSignalsSlots->carRunDataInfo.u32WHMBData/10.0));
    ui->labelinfo_8_3->setText(canSignalsSlots->carRunDataInfo.strACPWorkDataDate+" "+canSignalsSlots->carRunDataInfo.strACPWorkDataTime);
    ui->labelinfo_8_4->setText(QString("%1%").arg(canSignalsSlots->carRunDataInfo.u32ACPWorkData));
}

void delruninfo::allPBtnFocusClear(){
    ui->btnrundata->setStyleSheet(QSS_PBTN_NO_SELECTED);
    ui->btnworkdata->setStyleSheet(QSS_PBTN_NO_SELECTED);
    ui->btnridingrate->setStyleSheet(QSS_PBTN_NO_SELECTED);
    ui->btntraveldistance->setStyleSheet(QSS_PBTN_NO_SELECTED);
    ui->btndragelec->setStyleSheet(QSS_PBTN_NO_SELECTED);
    ui->btnreviveelec->setStyleSheet(QSS_PBTN_NO_SELECTED);
    ui->btnairpump->setStyleSheet(QSS_PBTN_NO_SELECTED);
    iDelItemIndex = 0;
}

void delruninfo::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void delruninfo::on_btnrundata_clicked(){
    if(1 != iDelItemIndex){
        allPBtnFocusClear();
        ui->btnrundata->setStyleSheet(QSS_PBTN_SELECTED_GREY);
        iDelItemIndex = 1;
        buttonBeep();
    }
}

void delruninfo::on_btnworkdata_clicked(){
    if(2 != iDelItemIndex){
        allPBtnFocusClear();
        ui->btnworkdata->setStyleSheet(QSS_PBTN_SELECTED_GREY);
        iDelItemIndex = 2;
        buttonBeep();
    }
}

void delruninfo::on_btnridingrate_clicked(){
    if(3 != iDelItemIndex){
        allPBtnFocusClear();
        ui->btnridingrate->setStyleSheet(QSS_PBTN_SELECTED_GREY);
        iDelItemIndex = 3;
        buttonBeep();
    }
}

void delruninfo::on_btntraveldistance_clicked(){
    if(4 != iDelItemIndex){
        allPBtnFocusClear();
        ui->btntraveldistance->setStyleSheet(QSS_PBTN_SELECTED_GREY);
        iDelItemIndex = 4;
        buttonBeep();
    }
}

void delruninfo::on_btndragelec_clicked(){
    if(5 != iDelItemIndex){
        allPBtnFocusClear();
        ui->btndragelec->setStyleSheet(QSS_PBTN_SELECTED_GREY);
        iDelItemIndex = 5;
        buttonBeep();
    }
}

void delruninfo::on_btnreviveelec_clicked(){
    if(6 != iDelItemIndex){
        allPBtnFocusClear();
        ui->btnreviveelec->setStyleSheet(QSS_PBTN_SELECTED_GREY);
        iDelItemIndex = 6;
        buttonBeep();
    }
}

void delruninfo::on_btnairpump_clicked(){
    if(7 != iDelItemIndex){
        allPBtnFocusClear();
        ui->btnairpump->setStyleSheet(QSS_PBTN_SELECTED_GREY);
        iDelItemIndex = 7;
        buttonBeep();
    }
}

void delruninfo::on_buttondelete_clicked(){
    if(0 != iDelItemIndex){
        canSignalsSlots->sendCarDeleteRecordDataHMI(2, iDelItemIndex);
        buttonBeep();
    }
}

void delruninfo::on_buttonquit_clicked(){
    buttonBeep();
    this->reject();
}

void delruninfo::slot_RunRecordData_Get_Update(){
    setScreenShow();
}
