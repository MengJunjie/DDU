#include "viewerrlist2.h"
#include "ui_viewerrlist2.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

viewerrlist2::viewerrlist2(int iCarId, QString strCarId, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::viewerrlist2)
{
    ui->setupUi(this);
    iErrCarId = iCarId;
    iShowErrIndex = 0;
    iErrListLength = 0;

    ui->labelcarId->setText(strCarId);

    Init();
    slot_FaultRecordData_Get_Update();

    connect(canSignalsSlots, SIGNAL(sig_FaultRecordData_Get_Update()), this, SLOT(slot_FaultRecordData_Get_Update()));

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));
}

viewerrlist2::~viewerrlist2(){
    delete ui;
}

void  viewerrlist2::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void viewerrlist2::setErrorInfo(int iIndex, QString strDate, QString strTime, QString strCode){
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
    case 4:
        tempLabel1 = ui->labelinfo_6_2;
        tempLabel2 = ui->labelinfo_6_3;
        break;
    case 5:
        tempLabel1 = ui->labelinfo_7_2;
        tempLabel2 = ui->labelinfo_7_3;
        break;
    case 6:
        tempLabel1 = ui->labelinfo_8_2;
        tempLabel2 = ui->labelinfo_8_3;
        break;
    case 7:
        tempLabel1 = ui->labelinfo_9_2;
        tempLabel2 = ui->labelinfo_9_3;
        break;
    case 8:
        tempLabel1 = ui->labelinfo_10_2;
        tempLabel2 = ui->labelinfo_10_3;
        break;
    case 9:
        tempLabel1 = ui->labelinfo_11_2;
        tempLabel2 = ui->labelinfo_11_3;
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

void viewerrlist2::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void viewerrlist2::on_buttonpreviouspage_clicked(){
    if(10 == iShowErrIndex){
        iShowErrIndex = 0;
        buttonBeep();
        slot_FaultRecordData_Get_Update();
    }
}

void viewerrlist2::on_buttonnextpage_clicked(){
    if(0 == iShowErrIndex && iErrListLength > 10){
        iShowErrIndex = 10;
        buttonBeep();
        slot_FaultRecordData_Get_Update();
    }
}

void viewerrlist2::on_buttonquit_clicked(){
    canSignalsSlots->bNowShowSubDevErrFlag = false;
    buttonBeep();
    this->reject();
}

void viewerrlist2::slot_FaultRecordData_Get_Update(){
    switch (iErrCarId) {
    case 1:
        iErrListLength = canSignalsSlots->car1BCUErrList.length();
        if(0 == iShowErrIndex){
            if(iErrListLength <= 10){
                for(int i = 0; i < canSignalsSlots->car1BCUErrList.length(); i++){
                    setErrorInfo(i, canSignalsSlots->car1BCUErrList.at(i)->strDate, canSignalsSlots->car1BCUErrList.at(i)->strTime, canSignalsSlots->car1BCUErrList.at(i)->strErrCode);
                }
            }
            else{
                for(int i = 0; i < 10; i++){
                    setErrorInfo(i, canSignalsSlots->car1BCUErrList.at(i)->strDate, canSignalsSlots->car1BCUErrList.at(i)->strTime, canSignalsSlots->car1BCUErrList.at(i)->strErrCode);
                }
            }
        }
        else{
            for(int i = 10; i < canSignalsSlots->car1BCUErrList.length(); i++){
                setErrorInfo(i-10, canSignalsSlots->car1BCUErrList.at(i)->strDate, canSignalsSlots->car1BCUErrList.at(i)->strTime, canSignalsSlots->car1BCUErrList.at(i)->strErrCode);
            }
        }
        break;
    case 2:
        iErrListLength = canSignalsSlots->car2BCUErrList.length();
        if(0 == iShowErrIndex){
            if(iErrListLength <= 10){
                for(int i = 0; i < canSignalsSlots->car2BCUErrList.length(); i++){
                    setErrorInfo(i, canSignalsSlots->car2BCUErrList.at(i)->strDate, canSignalsSlots->car2BCUErrList.at(i)->strTime, canSignalsSlots->car2BCUErrList.at(i)->strErrCode);
                }
            }
            else{
                for(int i = 0; i < 10; i++){
                    setErrorInfo(i, canSignalsSlots->car2BCUErrList.at(i)->strDate, canSignalsSlots->car2BCUErrList.at(i)->strTime, canSignalsSlots->car2BCUErrList.at(i)->strErrCode);
                }
            }
        }
        else{
            for(int i = 10; i < canSignalsSlots->car2BCUErrList.length(); i++){
                setErrorInfo(i-10, canSignalsSlots->car2BCUErrList.at(i)->strDate, canSignalsSlots->car2BCUErrList.at(i)->strTime, canSignalsSlots->car2BCUErrList.at(i)->strErrCode);
            }
        }
        break;
    case 3:
        iErrListLength = canSignalsSlots->car3BCUErrList.length();
        if(0 == iShowErrIndex){
            if(iErrListLength <= 10){
                for(int i = 0; i < canSignalsSlots->car3BCUErrList.length(); i++){
                    setErrorInfo(i, canSignalsSlots->car3BCUErrList.at(i)->strDate, canSignalsSlots->car3BCUErrList.at(i)->strTime, canSignalsSlots->car3BCUErrList.at(i)->strErrCode);
                }
            }
            else{
                for(int i = 0; i < 10; i++){
                    setErrorInfo(i, canSignalsSlots->car3BCUErrList.at(i)->strDate, canSignalsSlots->car3BCUErrList.at(i)->strTime, canSignalsSlots->car3BCUErrList.at(i)->strErrCode);
                }
            }
        }
        else{
            for(int i = 10; i < canSignalsSlots->car3BCUErrList.length(); i++){
                setErrorInfo(i-10, canSignalsSlots->car3BCUErrList.at(i)->strDate, canSignalsSlots->car3BCUErrList.at(i)->strTime, canSignalsSlots->car3BCUErrList.at(i)->strErrCode);
            }
        }
        break;
    case 4:
        iErrListLength = canSignalsSlots->car4BCUErrList.length();
        if(0 == iShowErrIndex){
            if(iErrListLength <= 10){
                for(int i = 0; i < canSignalsSlots->car4BCUErrList.length(); i++){
                    setErrorInfo(i, canSignalsSlots->car4BCUErrList.at(i)->strDate, canSignalsSlots->car4BCUErrList.at(i)->strTime, canSignalsSlots->car4BCUErrList.at(i)->strErrCode);
                }
            }
            else{
                for(int i = 0; i < 10; i++){
                    setErrorInfo(i, canSignalsSlots->car4BCUErrList.at(i)->strDate, canSignalsSlots->car4BCUErrList.at(i)->strTime, canSignalsSlots->car4BCUErrList.at(i)->strErrCode);
                }
            }
        }
        else{
            for(int i = 10; i < canSignalsSlots->car4BCUErrList.length(); i++){
                setErrorInfo(i-10, canSignalsSlots->car4BCUErrList.at(i)->strDate, canSignalsSlots->car4BCUErrList.at(i)->strTime, canSignalsSlots->car4BCUErrList.at(i)->strErrCode);
            }
        }
        break;
    case 5:
        iErrListLength = canSignalsSlots->car5BCUErrList.length();
        if(0 == iShowErrIndex){
            if(iErrListLength <= 10){
                for(int i = 0; i < canSignalsSlots->car5BCUErrList.length(); i++){
                    setErrorInfo(i, canSignalsSlots->car5BCUErrList.at(i)->strDate, canSignalsSlots->car5BCUErrList.at(i)->strTime, canSignalsSlots->car5BCUErrList.at(i)->strErrCode);
                }
            }
            else{
                for(int i = 0; i < 10; i++){
                    setErrorInfo(i, canSignalsSlots->car5BCUErrList.at(i)->strDate, canSignalsSlots->car5BCUErrList.at(i)->strTime, canSignalsSlots->car5BCUErrList.at(i)->strErrCode);
                }
            }
        }
        else{
            for(int i = 10; i < canSignalsSlots->car5BCUErrList.length(); i++){
                setErrorInfo(i-10, canSignalsSlots->car5BCUErrList.at(i)->strDate, canSignalsSlots->car5BCUErrList.at(i)->strTime, canSignalsSlots->car5BCUErrList.at(i)->strErrCode);
            }
        }
        break;
    case 6:
        iErrListLength = canSignalsSlots->car6BCUErrList.length();
        if(0 == iShowErrIndex){
            if(iErrListLength <= 10){
                for(int i = 0; i < canSignalsSlots->car6BCUErrList.length(); i++){
                    setErrorInfo(i, canSignalsSlots->car6BCUErrList.at(i)->strDate, canSignalsSlots->car6BCUErrList.at(i)->strTime, canSignalsSlots->car6BCUErrList.at(i)->strErrCode);
                }
            }
            else{
                for(int i = 0; i < 10; i++){
                    setErrorInfo(i, canSignalsSlots->car6BCUErrList.at(i)->strDate, canSignalsSlots->car6BCUErrList.at(i)->strTime, canSignalsSlots->car6BCUErrList.at(i)->strErrCode);
                }
            }
        }
        else{
            for(int i = 10; i < canSignalsSlots->car6BCUErrList.length(); i++){
                setErrorInfo(i-10, canSignalsSlots->car6BCUErrList.at(i)->strDate, canSignalsSlots->car6BCUErrList.at(i)->strTime, canSignalsSlots->car6BCUErrList.at(i)->strErrCode);
            }
        }
        break;
    default:
        break;
    }
}
