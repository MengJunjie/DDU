#include "networkstation.h"
#include "ui_networkstation.h"
#include "../can1operation.h"
#include "../can2operation.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

networkstation::networkstation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::networkstation)
{
    ui->setupUi(this);

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));

    Init();

    slot_SetCanWorking();
    slot_SetCCUWorking(1);
}

networkstation::~networkstation()
{
    delete ui;
}

void networkstation::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

    ui->labelinfo->setStyleSheet("border:none");

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));

    ui->labelCAN1->setStyleSheet(QSS_BG_BR_DARK_BROWN_WD_BLACK);
    ui->labelCAN2->setStyleSheet(QSS_BG_PURPLE_BR_PURPLE_WD_BLACK);

    can1DevNodeList.clear();
    can1DevNodeList <<  ui->labelMDU_1_1 <<  ui->labelMDU_2_1<<  ui->labelMDU_3_1<<  ui->labelMDU_4_1<<  ui->labelMDU_5_1<<  ui->labelMDU_6_1 << ui->labelHMI_1_1 << ui->labelHMI_2_1 << ui->labelCCU_1_1 << ui->labelCCU_2_1 << ui->labelERM_1_1 << ui->label_R1;
    can2DevNodeList.clear();
    can2DevNodeList <<  ui->labelMDU_1_2 <<  ui->labelMDU_2_2<<  ui->labelMDU_3_2<<  ui->labelMDU_4_2<<  ui->labelMDU_5_2<<  ui->labelMDU_6_2 << ui->labelHMI_1_2 << ui->labelHMI_2_2 << ui->labelCCU_1_2 << ui->labelCCU_2_2 << ui->labelERM_2_1 << ui->label_R2;
}

void  networkstation::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void networkstation::on_buttonback_clicked(){
    this->reject();
}

void networkstation::slot_SetCanState(int canId, CAN_STATE canState){
    QLabel * tempLabel = NULL;
    switch (canId) {
    case 1:
        tempLabel = ui->labelCAN1_1;
        break;
    case 2:
        tempLabel = ui->labelCAN2_1;
        break;
    default:
        tempLabel = NULL;
        break;
    }

    if(NULL == tempLabel){
        return ;
    }

    if(CAN_PREOPERATIONAL == canState){
        tempLabel->setStyleSheet(QSS_BG_YELLOW_BR_YELLOW_WD_BLACK);
    }
    else{
        if(ui->labelCAN1_1 == tempLabel){
            tempLabel->setStyleSheet(QSS_BG_BR_DARK_GREEN_WD_BLACK);
        }
        else{
            tempLabel->setStyleSheet(QSS_BG_BLUE_BR_BLUE_WD_BLACK);
        }
    }
    tempLabel = NULL;
}

void networkstation::slot_SetCanWorking(){
    QLabel * tempLabel = NULL;

    if(CAN_1 == canWorkFlag){
        ui->labelCAN1_1->setStyleSheet(QSS_BG_BR_DARK_GREEN_WD_BLACK);
//        ui->labelCAN2_1->setStyleSheet(QSS_BG_YELLOW_BR_YELLOW_WD_BLACK);

        for(int i = 0; i < can1DevNodeList.length(); i++){
            tempLabel = can1DevNodeList.at(i);
            tempLabel->setStyleSheet(QSS_BG_DARK_GREEN_BR_WHITE_WD_WHITE);
        }
    }
    else if(CAN_2 == canWorkFlag){
//        ui->labelCAN1_1->setStyleSheet(QSS_BG_YELLOW_BR_YELLOW_WD_BLACK);
        ui->labelCAN2_1->setStyleSheet(QSS_BG_BR_DARK_GREEN_WD_BLACK);

        for(int i = 0; i < can2DevNodeList.length(); i++){
            tempLabel = can2DevNodeList.at(i);
            tempLabel->setStyleSheet(QSS_BG_DARK_GREEN_BR_WHITE_WD_WHITE);
        }
    }
    else{
        ui->labelCAN1_1->setStyleSheet(QSS_BG_YELLOW_BR_YELLOW_WD_BLACK);
        ui->labelCAN2_1->setStyleSheet(QSS_BG_YELLOW_BR_YELLOW_WD_BLACK);

        for(int i = 0; i < can1DevNodeList.length(); i++){
            tempLabel = can1DevNodeList.at(i);
            tempLabel->setStyleSheet(QSS_BG_YELLOW_BR_WHITE_WD_BLACK);
        }

        for(int i = 0; i < can2DevNodeList.length(); i++){
            tempLabel = can2DevNodeList.at(i);
            tempLabel->setStyleSheet(QSS_BG_YELLOW_BR_WHITE_WD_BLACK);
        }
    }

    tempLabel = NULL;
}

void networkstation::slot_SetCCUWorking(int ccuId){
    if(1 == ccuId){
        ui->labelCCU_1->setStyleSheet(QSS_BG_DARK_GREEN_BR_WHITE_WD_WHITE);
        ui->labelCCU_2->setStyleSheet(QSS_BG_BLUE_BR_WHITE_WD_WHITE);
    }
    else{
        ui->labelCCU_1->setStyleSheet(QSS_BG_BLUE_BR_WHITE_WD_WHITE);
        ui->labelCCU_2->setStyleSheet(QSS_BG_DARK_GREEN_BR_WHITE_WD_WHITE);
    }
}

void networkstation::slot_SetR1R2ComnErr(int rrId){
    if(1 == rrId){
        ui->label_R1->setStyleSheet(QSS_BG_YELLOW_BR_WHITE_WD_BLACK);
    }
    else{
        ui->label_R2->setStyleSheet(QSS_BG_YELLOW_BR_WHITE_WD_BLACK);
    }
}
