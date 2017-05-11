#include "reparismenu.h"
#include "ui_reparismenu.h"
#include "../airpumpcheck.h"
#include "../doorcheck.h"
#include "../brakecheck.h"
#include "../viewerrlist1.h"
#include "../runinfo.h"
#include "../speedcheck.h"
#include "../sivcheck.h"
#include "../password.h"
#include "../networkstation.h"
#include "../setcarid.h"
#include "../ioscreen.h"

#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

reparismenu::reparismenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::reparismenu)
{
    ui->setupUi(this);
    curOperation = REPARI_NULL;

    bPBtnTempLeftClicked = false;

    pBtnList.clear();
    pBtnList << ui->buttondoorcheck << ui->buttonaircheck << ui->buttonSIVcheck <<  ui->buttonbraketest << ui->buttontimeset << ui->buttonparameterset;
    pBtnList << ui->buttonerrorlog << ui->buttondeleteerrorlog << ui->buttonrunlog << ui->buttondeleterunlog << ui->buttonspeedtest << ui->buttondeletespeedtest;
    pBtnList << ui->buttonIOScreen;

    Init();

    connect(ui->button_temp1, SIGNAL(clicked(bool)), this, SLOT(slot_OtherTempPBtn_Clicked()));
    connect(ui->button_temp2, SIGNAL(clicked(bool)), this, SLOT(slot_OtherTempPBtn_Clicked()));
    connect(ui->button_temp5, SIGNAL(clicked(bool)), this, SLOT(slot_OtherTempPBtn_Clicked()));
    connect(ui->button_temp6, SIGNAL(clicked(bool)), this, SLOT(slot_OtherTempPBtn_Clicked()));
//    connect(ui->button_temp7, SIGNAL(clicked(bool)), this, SLOT(slot_OtherTempPBtn_Clicked()));
    connect(ui->button_temp8, SIGNAL(clicked(bool)), this, SLOT(slot_OtherTempPBtn_Clicked()));
    connect(ui->button_temp9, SIGNAL(clicked(bool)), this, SLOT(slot_OtherTempPBtn_Clicked()));
    connect(ui->button_temp10, SIGNAL(clicked(bool)), this, SLOT(slot_OtherTempPBtn_Clicked()));
    connect(ui->button_temp11, SIGNAL(clicked(bool)), this, SLOT(slot_OtherTempPBtn_Clicked()));
    connect(ui->buttontemp1, SIGNAL(clicked(bool)), this, SLOT(slot_OtherTempPBtn_Clicked()));
    connect(ui->buttontemp2, SIGNAL(clicked(bool)), this, SLOT(slot_OtherTempPBtn_Clicked()));
    connect(ui->buttontemp3, SIGNAL(clicked(bool)), this, SLOT(slot_OtherTempPBtn_Clicked()));
    connect(ui->buttontemp4, SIGNAL(clicked(bool)), this, SLOT(slot_OtherTempPBtn_Clicked()));

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));
}

reparismenu::~reparismenu(){
    delete ui;
}

void reparismenu::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

    ui->buttontempleft->setStyleSheet("border: none");
    ui->buttontempright->setStyleSheet("border: none");

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));

    m_BlinkTimer = new QTimer();
    connect(m_BlinkTimer, SIGNAL(timeout()), this, SLOT(slot_Blink_TimeOut()));

//    m_3STimer = new QTimer();
//    connect(m_3STimer, SIGNAL(timeout()), this, SLOT(slot_3STimeOut()));
}

void reparismenu::allClickedFlagInit(){
    for(int i = 0; i < 2; i++){
        bClickedFlag[i] = false;
    }
}

void reparismenu::clearBtnFocus(){
    for(int i = 0; i < pBtnList.length(); i++){
        pBtnList.at(i)->setStyleSheet(QSS_PBTN_NO_SELECTED);
    }
}

void reparismenu::stopBlinkTimer(){
    if(m_BlinkTimer->isActive()){
        m_BlinkTimer->stop();
    }
}

void reparismenu::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void reparismenu::on_buttondoorcheck_clicked(){
    buttonBeep();
    allClickedFlagInit();
    stopBlinkTimer();
    clearBtnFocus();
    ui->buttondoorcheck->setStyleSheet(QSS_PBTN_SELECTED_GREY);
    curOperation = REPARI_DOORCHECK;
    ui->labelmessage->setText("");
    m_BlinkTimer->start(BLINK_TIMEOUT);
}

void reparismenu::on_buttonaircheck_clicked(){
    buttonBeep();
    allClickedFlagInit();
    stopBlinkTimer();
    clearBtnFocus();
    ui->buttonaircheck->setStyleSheet(QSS_PBTN_SELECTED_GREY);
    curOperation = REPARI_AIRPUMPCHECK;
    ui->labelmessage->setText("");
    m_BlinkTimer->start(BLINK_TIMEOUT);
}

void reparismenu::on_buttonSIVcheck_clicked(){
    buttonBeep();
    allClickedFlagInit();
    stopBlinkTimer();
    clearBtnFocus();
    ui->buttonSIVcheck->setStyleSheet(QSS_PBTN_SELECTED_GREY);
    curOperation = REPARI_SIVCHECK;
    ui->labelmessage->setText("");
    m_BlinkTimer->start(BLINK_TIMEOUT);
}

void reparismenu::on_buttonbraketest_clicked(){
    buttonBeep();
    allClickedFlagInit();
    stopBlinkTimer();
    clearBtnFocus();
    ui->buttonbraketest->setStyleSheet(QSS_PBTN_SELECTED_GREY);
    curOperation = REPARI_BRAKECHECK;
    ui->labelmessage->setText("");
    m_BlinkTimer->start(BLINK_TIMEOUT);
}

void reparismenu::on_buttontimeset_clicked(){
    allClickedFlagInit();
//    if(nowCtrlHMIFlag){
    buttonBeep();
    stopBlinkTimer();
    clearBtnFocus();
    ui->buttontimeset->setStyleSheet(QSS_PBTN_SELECTED_GREY);
    curOperation = REPARI_SETDATETIME;
    ui->labelmessage->setText("");
    m_BlinkTimer->start(BLINK_TIMEOUT);
//    }
//    else{
//        ui->labelmessage->setText("只有头车司机室显示屏才能进行词操作。");
//    }
}

void reparismenu::on_buttonparameterset_clicked(){
    allClickedFlagInit();
//    if(nowCtrlHMIFlag){
    buttonBeep();
    stopBlinkTimer();
    clearBtnFocus();
    ui->buttonparameterset->setStyleSheet(QSS_PBTN_SELECTED_GREY);
    curOperation = REPARI_SETPARA;
    ui->labelmessage->setText("");
    m_BlinkTimer->start(BLINK_TIMEOUT);
//    }
//    else{
//        ui->labelmessage->setText("只有头车司机室显示屏才能进行词操作。");
//    }
}

void reparismenu::on_buttonerrorlog_clicked(){
    buttonBeep();
    allClickedFlagInit();
    stopBlinkTimer();
    clearBtnFocus();
    ui->buttonerrorlog->setStyleSheet(QSS_PBTN_SELECTED_GREY);
    curOperation = REPARI_ERRORINFO;
    ui->labelmessage->setText("");
    m_BlinkTimer->start(BLINK_TIMEOUT);
}

void reparismenu::on_buttondeleteerrorlog_clicked(){
    allClickedFlagInit();
//    if(nowCtrlHMIFlag){
    buttonBeep();
    stopBlinkTimer();
    clearBtnFocus();
    ui->buttondeleteerrorlog->setStyleSheet(QSS_PBTN_SELECTED_GREY);
    curOperation = REPARI_DEL_ERRORINFO;
    ui->labelmessage->setText("");
    m_BlinkTimer->start(BLINK_TIMEOUT);
//    }
//    else{
//        ui->labelmessage->setText("只有头车司机室显示屏才能进行词操作。");
//    }
}

void reparismenu::on_buttonrunlog_clicked(){
    buttonBeep();
    allClickedFlagInit();
    stopBlinkTimer();
    clearBtnFocus();
    ui->buttonrunlog->setStyleSheet(QSS_PBTN_SELECTED_GREY);
    curOperation = REPARI_RUNINFO;
    ui->labelmessage->setText("");
    m_BlinkTimer->start(BLINK_TIMEOUT);
}

void reparismenu::on_buttondeleterunlog_clicked(){
    allClickedFlagInit();
//    if(nowCtrlHMIFlag){
    buttonBeep();
    stopBlinkTimer();
    clearBtnFocus();
    ui->buttondeleterunlog->setStyleSheet(QSS_PBTN_SELECTED_GREY);
    curOperation = REPARI_DEL_RUNINFO;
    ui->labelmessage->setText("");
    m_BlinkTimer->start(BLINK_TIMEOUT);
//    }
//    else{
//        ui->labelmessage->setText("只有头车司机室显示屏才能进行词操作。");
//    }
}

void reparismenu::on_buttonspeedtest_clicked(){
    buttonBeep();
    allClickedFlagInit();
    stopBlinkTimer();
    clearBtnFocus();
    ui->buttonspeedtest->setStyleSheet(QSS_PBTN_SELECTED_GREY);
    curOperation = REPARI_SPEEDINFO;
    ui->labelmessage->setText("");
    m_BlinkTimer->start(BLINK_TIMEOUT);
}

void reparismenu::on_buttondeletespeedtest_clicked(){
    allClickedFlagInit();
//    if(nowCtrlHMIFlag){
    buttonBeep();
    stopBlinkTimer();
    clearBtnFocus();
    ui->buttondeletespeedtest->setStyleSheet(QSS_PBTN_SELECTED_GREY);
    curOperation = REPARI_DEL_SPEEDINFO;
    m_BlinkTimer->start(BLINK_TIMEOUT);
//    }
//    else{
//        ui->labelmessage->setText("只有头车司机室显示屏才能进行词操作。");
//    }
}

void reparismenu::on_buttonlinkerreadout_clicked(){
    allClickedFlagInit();
//    stopBlinkTimer();
//    curOperation = REPARI_LINKER_GET;

//    m_BlinkTimer->start(BLINK_TIMEOUT);
}

void reparismenu::on_buttonpasswordset_clicked(){
    allClickedFlagInit();
//    stopBlinkTimer();
//    curOperation = REPARI_SETPASSWORD;

//    m_BlinkTimer->start(BLINK_TIMEOUT);
}

void reparismenu::on_buttonmenu_clicked(){
    buttonBeep();
    allClickedFlagInit();
    stopBlinkTimer();
    this->reject();
}

void reparismenu::on_buttonsure_clicked(){
    allClickedFlagInit();
    stopBlinkTimer();
    ui->buttonsure->setText(QString::fromUtf8("确   认"));

    QDialog * m_Dialog = NULL;
    switch (curOperation) {
    case REPARI_NULL:
        break;
    case REPARI_DOORCHECK:
        m_Dialog = new doorcheck();
        break;
    case REPARI_AIRPUMPCHECK:
        m_Dialog = new airpumpcheck();
        break;
    case REPARI_SIVCHECK:
        m_Dialog = new sivcheck();
        break;
    case REPARI_BRAKECHECK:
        m_Dialog = new brakecheck();
        break;
    case REPARI_SETDATETIME:
        m_Dialog = new password(SET_DATETIME);
        break;
    case REPARI_SETPARA:
        m_Dialog = new password(SET_PARA);
        break;
    case REPARI_ERRORINFO:
        m_Dialog = new viewerrlist1();
        break;
    case REPARI_DEL_ERRORINFO:
        m_Dialog = new password(DEL_ERRINFO);
        break;
    case REPARI_RUNINFO:
        m_Dialog = new runinfo();
        break;
    case REPARI_DEL_RUNINFO:
        m_Dialog = new password(DEL_RUNINFO);
        break;
    case REPARI_SPEEDINFO:
        m_Dialog = new speedcheck();
        break;
    case REPARI_DEL_SPEEDINFO:
        m_Dialog = new password(DEL_SPEEDINFO);
        break;
    case REPARI_LINKER_GET:
        m_Dialog = new password(LINKER_READOUT);
        break;
    case REPARI_SETPASSWORD:
        m_Dialog = new password(SET_PASSWORD);
        break;
    case REPARI_IOSCREEN:
        m_Dialog = new IOScreen();
        break;
    default:
        m_Dialog = NULL;
        break;
    }

    if(NULL == m_Dialog){
        return ;
    }

    buttonBeep();
    clearBtnFocus();
    curOperation = REPARI_NULL;
    m_Dialog->setModal(true);
    m_Dialog->show();
}

void reparismenu::on_buttonIOScreen_clicked(){
    buttonBeep();
    allClickedFlagInit();
    stopBlinkTimer();
    clearBtnFocus();
    ui->buttonIOScreen->setStyleSheet(QSS_PBTN_SELECTED_GREY);
    curOperation = REPARI_IOSCREEN;
    ui->labelmessage->setText("");
    m_BlinkTimer->start(BLINK_TIMEOUT);
}

void reparismenu::on_buttontempleft_clicked(){
//    allClickedFlagInit();
//    m_3STimer->stop();

//    bPBtnTempLeftClicked = true;
//    m_3STimer->start(3000);
}

void reparismenu::on_buttontempright_clicked(){
//    allClickedFlagInit();
//    m_3STimer->stop();

//    if(bPBtnTempLeftClicked){
//        bPBtnTempLeftClicked = false;

//        networkstation * networkStation = new networkstation();
//        networkStation->setModal(true);
//        networkStation->show();
//    }
}

void reparismenu::on_button_temp3_clicked(){
    allClickedFlagInit();
    bClickedFlag[0] = true;
}

void reparismenu::on_button_temp4_clicked(){
    if(bClickedFlag[0] && !bClickedFlag[1]){
        bClickedFlag[1] = true;
    }
    else{
        allClickedFlagInit();
    }
}

void reparismenu::on_button_temp5_clicked(){
    if(bClickedFlag[1]){
        setCarId * m_SetCarId = new setCarId();
        m_SetCarId->setModal(true);
        m_SetCarId->show();
    }
    allClickedFlagInit();
}

void reparismenu::slot_OtherTempPBtn_Clicked(){
    allClickedFlagInit();
}

void reparismenu::slot_Blink_TimeOut(){
    if("" == ui->buttonsure->text()){
        ui->buttonsure->setText(QString::fromUtf8("确   认"));
    }
    else{
        ui->buttonsure->setText("");
    }
}

void reparismenu::slot_3STimeOut(){
//    m_3STimer->stop();
    bPBtnTempLeftClicked = false;
}
