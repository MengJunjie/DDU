#include "setstation.h"
#include "ui_setstation.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

setstation::setstation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setstation)
{
    ui->setupUi(this);
    strStationName = "";
    iStationCount = canSignalsSlots->stationNameList.length();
    iStationFlag = 0;
    iStationId = 0;

    Init();
    setStationName();

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));
}

setstation::~setstation(){
    delete ui;
}

void setstation::Init(){
    this->setWindowFlags(Qt::FramelessWindowHint);

    pBtnList << ui->btnstation_1 << ui->btnstation_2 << ui->btnstation_3 << ui->btnstation_4 << ui->btnstation_5 << ui->btnstation_6 << ui->btnstation_7 << ui->btnstation_8;
    pBtnList << ui->btnstation_9 << ui->btnstation_10 << ui->btnstation_11 << ui->btnstation_12 << ui->btnstation_13 << ui->btnstation_14 << ui->btnstation_15 << ui->btnstation_16;
    pBtnList << ui->btnstation_17 << ui->btnstation_18 << ui->btnstation_19 << ui->btnstation_20;

    for(int i = 0; i < pBtnList.length(); i++){
        pBtnList.at(i)->setText("");
        pBtnList.at(i)->setProperty("pBtnSelected", false);
        connect(pBtnList.at(i), SIGNAL(clicked(bool)), this, SLOT(slot_StationBtn_Clicked()));
    }

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void setstation::setTitleName(int flag){
    QString strText = "";
    switch (flag) {
    case 0:
        strText = "当前站";
        iStationFlag = 0;
        break;
    case 1:
        strText = "站设置";
        iStationFlag = 1;
        setStationId(canSignalsSlots->iCurrStation);
        break;
    case 2:
        strText = "站设置";
        iStationFlag = 2;
        setStationId(canSignalsSlots->iEndStation);
        break;
    default:
        strText = "";
        break;
    }

    ui->title_2->setText(strText);
}

void setstation::setStationId(int stationId){
    iStationId = stationId;
    for(int i = 0; i < iStationCount; i++){
        if(canSignalsSlots->stationCodeList.at(i) == stationId){
            pBtnList.at(i)->setStyleSheet(QSS_PBTN_SELECTED_GREEN);
            break;
        }
    }
}

void setstation::setStationName(){
    for(int i = 0; i < iStationCount; i++){
        pBtnList.at(i)->setText(canSignalsSlots->stationNameList.at(i));
    }
}

void setstation::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void setstation::on_buttonback_clicked(){
    buttonBeep();
    this->reject();
}

void setstation::on_buttonok_clicked(){
    if("" != strStationName){
        emit sig_setStationId(iStationFlag, iStationId);

        buttonBeep();
        this->reject();
    }
}

void setstation::slot_StationBtn_Clicked(){
    QPushButton * tempBtn = dynamic_cast<QPushButton*>(sender());
    strStationName = tempBtn->text();
    if("" != strStationName){
        buttonBeep();

        for(int i = 0; i < iStationCount; i++){
            if(pBtnList.at(i) != tempBtn){
                pBtnList.at(i)->setStyleSheet(QSS_PBTN_NO_SELECTED);
            }
            else{
                tempBtn->setStyleSheet(QSS_PBTN_SELECTED_GREEN);
                iStationId = canSignalsSlots->stationCodeList.at(i);
            }
        }
    }
}
