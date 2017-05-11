#include "errorvvvfsiv.h"
#include "ui_errorvvvfsiv.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

errorvvvfsiv::errorvvvfsiv(BREAKDOWN_TYPE type, int iCarId, QString strCarId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::errorvvvfsiv)
{
    ui->setupUi(this);
    iErrCarId = iCarId;
    iErrType = type;
    iShowErrIndex = 0;
    iErrListLength = 0;
    QString strText = "";
    switch (type) {
    case BD_VVVF1:
        strText = "VVVF1监控记录";
        break;
    case BD_VVVF2:
        strText = "VVVF2监控记录";
        break;
    case BD_SIV:
        strText = "SIV监控记录";
        break;
    default:
        strText = "";
        break;
    }

    ui->labelrevisetype->setText(strText);
    ui->labelcarId->setText(strCarId);
    Init();
    slot_FaultRecordData_Get_Update();

    connect(canSignalsSlots, SIGNAL(sig_FaultRecordData_Get_Update()), this, SLOT(slot_FaultRecordData_Get_Update()));

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));
}

errorvvvfsiv::~errorvvvfsiv(){
    delete ui;
}

void errorvvvfsiv::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void errorvvvfsiv::setErrorInfo(int iIndex, QString strDate, QString strTime, QString strCode){
    QLabel * tempLabel1 = NULL;
    QLabel * tempLabel2 = NULL;
    switch (iIndex) {
    case 0:
        tempLabel1 = ui->labelinfo_2_2;
        tempLabel2 = ui->labelinfo_2_3;
        break;
    case 1:
        tempLabel1 = ui->labelinfo_3_2;
        tempLabel2 = ui->labelinfo_3_3;
        break;
    case 2:
        tempLabel1 = ui->labelinfo_4_2;
        tempLabel2 = ui->labelinfo_4_3;
        break;
    case 3:
        tempLabel1 = ui->labelinfo_5_2;
        tempLabel2 = ui->labelinfo_5_3;
        break;
    default:
        tempLabel1 = NULL;
        tempLabel2 = NULL;
        break;
    }
    if(NULL == tempLabel1 || NULL == tempLabel2){
        return ;
    }
    tempLabel1->setText(strDate+" "+strTime);
    tempLabel2->setText(strCode);
    tempLabel1 = NULL;
    tempLabel2 = NULL;
}

void errorvvvfsiv::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void errorvvvfsiv::on_buttonpreviouspage_clicked(){
    if(4 == iShowErrIndex){
        iShowErrIndex = 0;
        ui->labelinfo_2_1->setText("1");
        ui->labelinfo_3_1->setText("2");
        ui->labelinfo_4_1->setText("3");
        ui->labelinfo_5_1->setText("4");
        ui->labelinfo_2_2->setText("");
        ui->labelinfo_2_3->setText("");
        ui->labelinfo_3_2->setText("");
        ui->labelinfo_3_3->setText("");
        ui->labelinfo_4_2->setText("");
        ui->labelinfo_4_3->setText("");
        ui->labelinfo_5_2->setText("");
        ui->labelinfo_5_3->setText("");
        buttonBeep();
        slot_FaultRecordData_Get_Update();
    }
}

void errorvvvfsiv::on_buttonnextpage_clicked(){
    if(0 == iShowErrIndex && iErrListLength > 4){
        iShowErrIndex = 4;
        ui->labelinfo_2_1->setText("5");
        ui->labelinfo_3_1->setText("6");
        ui->labelinfo_4_1->setText("7");
        ui->labelinfo_5_1->setText("8");
        ui->labelinfo_2_2->setText("");
        ui->labelinfo_2_3->setText("");
        ui->labelinfo_3_2->setText("");
        ui->labelinfo_3_3->setText("");
        ui->labelinfo_4_2->setText("");
        ui->labelinfo_4_3->setText("");
        ui->labelinfo_5_2->setText("");
        ui->labelinfo_5_3->setText("");
        buttonBeep();
        slot_FaultRecordData_Get_Update();
    }
}

void errorvvvfsiv::on_buttonquit_clicked(){
    canSignalsSlots->bNowShowSubDevErrFlag = false;
    buttonBeep();
    this->reject();
}

void errorvvvfsiv::slot_FaultRecordData_Get_Update(){
    switch (iErrType) {
    case BD_VVVF1:
        switch (iErrCarId) {
        case 1:
            iErrListLength = canSignalsSlots->car1VVVF1ErrList.length();
            if(0 == iShowErrIndex){
                if(iErrListLength <= 4){
                    for(int i = 0; i < canSignalsSlots->car1VVVF1ErrList.length(); i++){
                        setErrorInfo(i, canSignalsSlots->car1VVVF1ErrList.at(i)->strDate, canSignalsSlots->car1VVVF1ErrList.at(i)->strTime, canSignalsSlots->car1VVVF1ErrList.at(i)->strErrCode);
                    }
                }
                else{
                    for(int i = 0; i < 4; i++){
                        setErrorInfo(i, canSignalsSlots->car1VVVF1ErrList.at(i)->strDate, canSignalsSlots->car1VVVF1ErrList.at(i)->strTime, canSignalsSlots->car1VVVF1ErrList.at(i)->strErrCode);
                    }
                }
            }
            else{
                for(int i = 4; i < canSignalsSlots->car1VVVF1ErrList.length(); i++){
                    setErrorInfo(i-4, canSignalsSlots->car1VVVF1ErrList.at(i)->strDate, canSignalsSlots->car1VVVF1ErrList.at(i)->strTime, canSignalsSlots->car1VVVF1ErrList.at(i)->strErrCode);
                }
            }
            break;
        case 3:
            iErrListLength = canSignalsSlots->car3VVVF1ErrList.length();
            if(0 == iShowErrIndex){
                if(iErrListLength <= 4){
                    for(int i = 0; i < canSignalsSlots->car3VVVF1ErrList.length(); i++){
                        setErrorInfo(i, canSignalsSlots->car3VVVF1ErrList.at(i)->strDate, canSignalsSlots->car3VVVF1ErrList.at(i)->strTime, canSignalsSlots->car3VVVF1ErrList.at(i)->strErrCode);
                    }
                }
                else{
                    for(int i = 0; i < 4; i++){
                        setErrorInfo(i, canSignalsSlots->car3VVVF1ErrList.at(i)->strDate, canSignalsSlots->car3VVVF1ErrList.at(i)->strTime, canSignalsSlots->car3VVVF1ErrList.at(i)->strErrCode);
                    }
                }
            }
            else{
                for(int i = 4; i < canSignalsSlots->car3VVVF1ErrList.length(); i++){
                    setErrorInfo(i-4, canSignalsSlots->car3VVVF1ErrList.at(i)->strDate, canSignalsSlots->car3VVVF1ErrList.at(i)->strTime, canSignalsSlots->car3VVVF1ErrList.at(i)->strErrCode);
                }
            }
            break;
        case 6:
            iErrListLength = canSignalsSlots->car6VVVF1ErrList.length();
            if(0 == iShowErrIndex){
                if(iErrListLength <= 4){
                    for(int i = 0; i < canSignalsSlots->car6VVVF1ErrList.length(); i++){
                        setErrorInfo(i, canSignalsSlots->car6VVVF1ErrList.at(i)->strDate, canSignalsSlots->car6VVVF1ErrList.at(i)->strTime, canSignalsSlots->car6VVVF1ErrList.at(i)->strErrCode);
                    }
                }
                else{
                    for(int i = 0; i < 4; i++){
                        setErrorInfo(i, canSignalsSlots->car6VVVF1ErrList.at(i)->strDate, canSignalsSlots->car6VVVF1ErrList.at(i)->strTime, canSignalsSlots->car6VVVF1ErrList.at(i)->strErrCode);
                    }
                }
            }
            else{
                for(int i = 4; i < canSignalsSlots->car6VVVF1ErrList.length(); i++){
                    setErrorInfo(i-4, canSignalsSlots->car6VVVF1ErrList.at(i)->strDate, canSignalsSlots->car6VVVF1ErrList.at(i)->strTime, canSignalsSlots->car6VVVF1ErrList.at(i)->strErrCode);
                }
            }
            break;
        default:
            break;
        }
        break;
    case BD_VVVF2:
        switch (iErrCarId) {
        case 1:
            iErrListLength = canSignalsSlots->car1VVVF2ErrList.length();
            if(0 == iShowErrIndex){
                if(iErrListLength <= 4){
                    for(int i = 0; i < canSignalsSlots->car1VVVF2ErrList.length(); i++){
                        setErrorInfo(i, canSignalsSlots->car1VVVF2ErrList.at(i)->strDate, canSignalsSlots->car1VVVF2ErrList.at(i)->strTime, canSignalsSlots->car1VVVF2ErrList.at(i)->strErrCode);
                    }
                }
                else{
                    for(int i = 0; i < 4; i++){
                        setErrorInfo(i, canSignalsSlots->car1VVVF2ErrList.at(i)->strDate, canSignalsSlots->car1VVVF2ErrList.at(i)->strTime, canSignalsSlots->car1VVVF2ErrList.at(i)->strErrCode);
                    }
                }
            }
            else{
                for(int i = 4; i < canSignalsSlots->car1VVVF2ErrList.length(); i++){
                    setErrorInfo(i-4, canSignalsSlots->car1VVVF2ErrList.at(i)->strDate, canSignalsSlots->car1VVVF2ErrList.at(i)->strTime, canSignalsSlots->car1VVVF2ErrList.at(i)->strErrCode);
                }
            }
            break;
        case 3:
            iErrListLength = canSignalsSlots->car3VVVF2ErrList.length();
            if(0 == iShowErrIndex){
                if(iErrListLength <= 4){
                    for(int i = 0; i < canSignalsSlots->car3VVVF2ErrList.length(); i++){
                        setErrorInfo(i, canSignalsSlots->car3VVVF2ErrList.at(i)->strDate, canSignalsSlots->car3VVVF2ErrList.at(i)->strTime, canSignalsSlots->car3VVVF2ErrList.at(i)->strErrCode);
                    }
                }
                else{
                    for(int i = 0; i < 4; i++){
                        setErrorInfo(i, canSignalsSlots->car3VVVF2ErrList.at(i)->strDate, canSignalsSlots->car3VVVF2ErrList.at(i)->strTime, canSignalsSlots->car3VVVF2ErrList.at(i)->strErrCode);
                    }
                }
            }
            else{
                for(int i = 4; i < canSignalsSlots->car3VVVF2ErrList.length(); i++){
                    setErrorInfo(i-4, canSignalsSlots->car3VVVF2ErrList.at(i)->strDate, canSignalsSlots->car3VVVF2ErrList.at(i)->strTime, canSignalsSlots->car3VVVF2ErrList.at(i)->strErrCode);
                }
            }
            break;
        case 6:
            iErrListLength = canSignalsSlots->car6VVVF2ErrList.length();
            if(0 == iShowErrIndex){
                if(iErrListLength <= 4){
                    for(int i = 0; i < canSignalsSlots->car6VVVF2ErrList.length(); i++){
                        setErrorInfo(i, canSignalsSlots->car6VVVF2ErrList.at(i)->strDate, canSignalsSlots->car6VVVF2ErrList.at(i)->strTime, canSignalsSlots->car6VVVF2ErrList.at(i)->strErrCode);
                    }
                }
                else{
                    for(int i = 0; i < 4; i++){
                        setErrorInfo(i, canSignalsSlots->car6VVVF2ErrList.at(i)->strDate, canSignalsSlots->car6VVVF2ErrList.at(i)->strTime, canSignalsSlots->car6VVVF2ErrList.at(i)->strErrCode);
                    }
                }
            }
            else{
                for(int i = 4; i < canSignalsSlots->car6VVVF2ErrList.length(); i++){
                    setErrorInfo(i-4, canSignalsSlots->car6VVVF2ErrList.at(i)->strDate, canSignalsSlots->car6VVVF2ErrList.at(i)->strTime, canSignalsSlots->car6VVVF2ErrList.at(i)->strErrCode);
                }
            }
            break;
        default:
            break;
        }
        break;
    case BD_SIV:
        switch (iErrCarId) {
        case 2:
            iErrListLength = canSignalsSlots->car2SIVErrList.length();
            if(0 == iShowErrIndex){
                if(iErrListLength <= 4){
                    for(int i = 0; i < canSignalsSlots->car2SIVErrList.length(); i++){
                        setErrorInfo(i, canSignalsSlots->car2SIVErrList.at(i)->strDate, canSignalsSlots->car2SIVErrList.at(i)->strTime, canSignalsSlots->car2SIVErrList.at(i)->strErrCode);
                    }
                }
                else{
                    for(int i = 0; i < 4; i++){
                        setErrorInfo(i, canSignalsSlots->car2SIVErrList.at(i)->strDate, canSignalsSlots->car2SIVErrList.at(i)->strTime, canSignalsSlots->car2SIVErrList.at(i)->strErrCode);
                    }
                }
            }
            else{
                for(int i = 4; i < canSignalsSlots->car2SIVErrList.length(); i++){
                    setErrorInfo(i-4, canSignalsSlots->car2SIVErrList.at(i)->strDate, canSignalsSlots->car2SIVErrList.at(i)->strTime, canSignalsSlots->car2SIVErrList.at(i)->strErrCode);
                }
            }
            break;
        case 4:
            iErrListLength = canSignalsSlots->car4SIVErrList.length();
            if(0 == iShowErrIndex){
                if(iErrListLength <= 4){
                    for(int i = 0; i < canSignalsSlots->car4SIVErrList.length(); i++){
                        setErrorInfo(i, canSignalsSlots->car4SIVErrList.at(i)->strDate, canSignalsSlots->car4SIVErrList.at(i)->strTime, canSignalsSlots->car4SIVErrList.at(i)->strErrCode);
                    }
                }
                else{
                    for(int i = 0; i < 4; i++){
                        setErrorInfo(i, canSignalsSlots->car4SIVErrList.at(i)->strDate, canSignalsSlots->car4SIVErrList.at(i)->strTime, canSignalsSlots->car4SIVErrList.at(i)->strErrCode);
                    }
                }
            }
            else{
                for(int i = 4; i < canSignalsSlots->car4SIVErrList.length(); i++){
                    setErrorInfo(i-4, canSignalsSlots->car4SIVErrList.at(i)->strDate, canSignalsSlots->car4SIVErrList.at(i)->strTime, canSignalsSlots->car4SIVErrList.at(i)->strErrCode);
                }
            }
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}
