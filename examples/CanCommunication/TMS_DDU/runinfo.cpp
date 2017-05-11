#include "runinfo.h"
#include "ui_runinfo.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

runinfo::runinfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::runinfo)
{
    ui->setupUi(this);

    Init();

    connect(canSignalsSlots, SIGNAL(sig_RunRecordData_Get_Update()), this, SLOT(slot_RunRecordData_Get_Update()));

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));

    canSignalsSlots->sendCarGetFaultInfoHMI(2);
}

runinfo::~runinfo(){
    delete ui;
}

void runinfo::Init(){
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

void runinfo::cleanScreenShow(){
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

void runinfo::setScreenShow(){
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

void runinfo::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void runinfo::on_buttonquit_clicked(){
    buttonBeep();
    this->reject();
}

void runinfo::slot_RunRecordData_Get_Update(){
    setScreenShow();
}
