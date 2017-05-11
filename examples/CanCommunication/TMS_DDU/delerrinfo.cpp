#include "delerrinfo.h"
#include "ui_delerrinfo.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

delerrinfo::delerrinfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::delerrinfo)
{
    ui->setupUi(this);

    iDelItemIndex = 0;

    Init();
    slot_CarIcon_Update();
    slot_CarCode_Update();

    slot_FaultRecordCount_Get_Update();

    connect(canSignalsSlots, SIGNAL(sig_Car_Code_Change()), this, SLOT(slot_CarCode_Update()));
    connect(canSignalsSlots, SIGNAL(sig_Car_Direction_Change()), this, SLOT(slot_Car_Direction_Update()));
    connect(canSignalsSlots, SIGNAL(sig_FaultRecordCount_Get_Update()), this, SLOT(slot_FaultRecordCount_Get_Update()));

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));

    canSignalsSlots->sendCarGetFaultInfoHMI(1);
}

delerrinfo::~delerrinfo(){
    delete ui;
}

void delerrinfo::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

//    ui->labelarrow->setScaledContents(true);
    ui->carimagelabel1->setScaledContents(true);
    ui->carimagelabel2->setScaledContents(true);
    ui->carimagelabel3->setScaledContents(true);
    ui->carimagelabel4->setScaledContents(true);
    ui->carimagelabel5->setScaledContents(true);
    ui->carimagelabel6->setScaledContents(true);

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void delerrinfo::setFalutRecordCount(CAR_ID carId, int iCount){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    QLabel * tempLabel = NULL;
    switch (realCarId) {
    case CAR_1:
        tempLabel = ui->labelinfo_2_2;
        break;
    case CAR_2:
        tempLabel = ui->labelinfo_2_3;
        break;
    case CAR_3:
        tempLabel = ui->labelinfo_2_4;
        break;
    case CAR_4:
        tempLabel = ui->labelinfo_2_5;
        break;
    case CAR_5:
        tempLabel = ui->labelinfo_2_6;
        break;
    case CAR_6:
        tempLabel = ui->labelinfo_2_7;
        break;
    default:
        break;
    }
    tempLabel->setText(QString("%1").arg(iCount));
    tempLabel = NULL;
}

void delerrinfo::setVVVF1FalutCount(CAR_ID carId, int iCount){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    QLabel * tempLabel = NULL;
    switch (realCarId) {
    case CAR_1:
        tempLabel = ui->labelinfo_3_2;
        break;
    case CAR_3:
        tempLabel = ui->labelinfo_3_4;
        break;
    case CAR_4:
        tempLabel = ui->labelinfo_3_5;
        break;
    case CAR_6:
        tempLabel = ui->labelinfo_3_7;
        break;
    default:
        break;
    }
    if(CAR_3 == carId){
        ui->labelinfo_3_4->setText("");
        ui->labelinfo_3_5->setText("");
    }
    tempLabel->setText(QString("%1").arg(iCount));
    tempLabel = NULL;
}

void delerrinfo::setVVVF2FalutCount(CAR_ID carId, int iCount){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    QLabel * tempLabel = NULL;
    switch (realCarId) {
    case CAR_1:
        tempLabel = ui->labelinfo_4_2;
        break;
    case CAR_3:
        tempLabel = ui->labelinfo_4_4;
        break;
    case CAR_4:
        tempLabel = ui->labelinfo_4_5;
        break;
    case CAR_6:
        tempLabel = ui->labelinfo_4_7;
        break;
    default:
        break;
    }
    if(CAR_3 == carId){
        ui->labelinfo_4_4->setText("");
        ui->labelinfo_4_5->setText("");
    }
    tempLabel->setText(QString("%1").arg(iCount));
    tempLabel = NULL;
}

void delerrinfo::setSIVFalutCount(CAR_ID carId, int iCount){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    QLabel * tempLabel = NULL;
    switch (realCarId) {
    case CAR_2:
        tempLabel = ui->labelinfo_5_3;
        break;
    case CAR_3:
        tempLabel = ui->labelinfo_5_4;
        break;
    case CAR_4:
        tempLabel = ui->labelinfo_5_5;
        break;
    case CAR_5:
        tempLabel = ui->labelinfo_5_6;
        break;
    default:
        break;
    }
    if(CAR_2 == carId){
        ui->labelinfo_5_3->setText("");
        ui->labelinfo_5_6->setText("");
    }
    if(CAR_4 == carId){
        ui->labelinfo_5_4->setText("");
        ui->labelinfo_5_5->setText("");
    }
    tempLabel->setText(QString("%1").arg(iCount));
    tempLabel = NULL;
}

void delerrinfo::setBrakeFalutCount(CAR_ID carId, int iCount){
    CAR_ID realCarId = CAR_NULL;
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        realCarId = CAR_ID(7 - carId);
    }
    else{
        realCarId = carId;
    }
    QLabel * tempLabel = NULL;
    switch (realCarId) {
    case CAR_1:
        tempLabel = ui->labelinfo_6_2;
        break;
    case CAR_2:
        tempLabel = ui->labelinfo_6_3;
        break;
    case CAR_3:
        tempLabel = ui->labelinfo_6_4;
        break;
    case CAR_4:
        tempLabel = ui->labelinfo_6_5;
        break;
    case CAR_5:
        tempLabel = ui->labelinfo_6_6;
        break;
    case CAR_6:
        tempLabel = ui->labelinfo_6_7;
        break;
    default:
        break;
    }
    tempLabel->setText(QString("%1").arg(iCount));
    tempLabel = NULL;
}

void delerrinfo::allPBtnFocusClear(){
    ui->btnerrorrecord->setStyleSheet(QSS_PBTN_NO_SELECTED);
    ui->btnvvvf1->setStyleSheet(QSS_PBTN_NO_SELECTED);
    ui->btnvvvf2->setStyleSheet(QSS_PBTN_NO_SELECTED);
    ui->btnsiv->setStyleSheet(QSS_PBTN_NO_SELECTED);
    ui->btnbrake->setStyleSheet(QSS_PBTN_NO_SELECTED);
    iDelItemIndex = 0;
}

void delerrinfo::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void delerrinfo::on_btnerrorrecord_clicked(){
    if(1 != iDelItemIndex){
        allPBtnFocusClear();
        ui->btnerrorrecord->setStyleSheet(QSS_PBTN_SELECTED_GREY);
        iDelItemIndex = 1;
        buttonBeep();
    }
}

void delerrinfo::on_btnvvvf1_clicked(){
    if(2 != iDelItemIndex){
        allPBtnFocusClear();
        ui->btnvvvf1->setStyleSheet(QSS_PBTN_SELECTED_GREY);
        iDelItemIndex = 2;
        buttonBeep();
    }
}

void delerrinfo::on_btnvvvf2_clicked(){
    if(3 != iDelItemIndex){
        allPBtnFocusClear();
        ui->btnvvvf2->setStyleSheet(QSS_PBTN_SELECTED_GREY);
        iDelItemIndex = 3;
        buttonBeep();
    }
}

void delerrinfo::on_btnsiv_clicked(){
    if(4 != iDelItemIndex){
        allPBtnFocusClear();
        ui->btnsiv->setStyleSheet(QSS_PBTN_SELECTED_GREY);
        iDelItemIndex = 4;
        buttonBeep();
    }
}

void delerrinfo::on_btnbrake_clicked(){
    if(5 != iDelItemIndex){
        allPBtnFocusClear();
        ui->btnbrake->setStyleSheet(QSS_PBTN_SELECTED_GREY);
        iDelItemIndex = 5;
        buttonBeep();
    }
}

void delerrinfo::on_buttondelete_clicked(){
    if(0 != iDelItemIndex){
        canSignalsSlots->sendCarDeleteRecordDataHMI(1, iDelItemIndex);
        buttonBeep();
    }
}

void delerrinfo::on_buttonquit_clicked(){
    buttonBeep();
    this->reject();
}

void delerrinfo::slot_CarIcon_Update(){
    if(CAR_NO_HEADER != carHeaderFlag){
        ui->labelarrow->setPixmap(ARROW_LEFT);
    }
    else{
        ui->labelarrow->setPixmap(NO_QPIXMAP);
    }
    ui->carimagelabel1->setPixmap(MC1_CAR_BMP);
    ui->carimagelabel2->setPixmap(T_CAR_BMP);
    ui->carimagelabel5->setPixmap(T_CAR_BMP);
    ui->carimagelabel6->setPixmap(MC2_CAR_BMP);

    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        ui->carimagelabel3->setPixmap(T_CAR_BMP);
        ui->carimagelabel4->setPixmap(M_CAR_BMP);

        ui->labelinfo_1_2->setText(CAR_6_ID);
        ui->labelinfo_1_3->setText(CAR_5_ID);
        ui->labelinfo_1_4->setText(CAR_4_ID);
        ui->labelinfo_1_5->setText(CAR_3_ID);
        ui->labelinfo_1_6->setText(CAR_2_ID);
        ui->labelinfo_1_7->setText(CAR_1_ID);
    }
    else{
        ui->carimagelabel3->setPixmap(M_CAR_BMP);
        ui->carimagelabel4->setPixmap(T_CAR_BMP);

        ui->labelinfo_1_2->setText(CAR_1_ID);
        ui->labelinfo_1_3->setText(CAR_2_ID);
        ui->labelinfo_1_4->setText(CAR_3_ID);
        ui->labelinfo_1_5->setText(CAR_4_ID);
        ui->labelinfo_1_6->setText(CAR_5_ID);
        ui->labelinfo_1_7->setText(CAR_6_ID);
    }
}

void delerrinfo::slot_CarCode_Update(){
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        ui->carlabel1->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[5]));
        ui->carlabel2->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[4]));
        ui->carlabel3->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[3]));
        ui->carlabel4->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[2]));
        ui->carlabel5->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[1]));
        ui->carlabel6->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[0]));
    }
    else{
        ui->carlabel1->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[0]));
        ui->carlabel2->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[1]));
        ui->carlabel3->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[2]));
        ui->carlabel4->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[3]));
        ui->carlabel5->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[4]));
        ui->carlabel6->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[5]));
    }
}

void delerrinfo::slot_Car_Direction_Update(){
    slot_CarIcon_Update();
    slot_CarCode_Update();

    slot_FaultRecordCount_Get_Update();
}

void delerrinfo::slot_FaultRecordCount_Get_Update(){
    for(int i = 0; i < 6; i++){
        setFalutRecordCount(CAR_ID(i+1), canSignalsSlots->m_FaultRecordCount[i]);
        setBrakeFalutCount(CAR_ID(i+1), canSignalsSlots->m_BCUFaultCount[i]);
    }
    setVVVF1FalutCount(CAR_1, canSignalsSlots->m_VVVF1FaultCount[0]);
    setVVVF1FalutCount(CAR_3, canSignalsSlots->m_VVVF1FaultCount[1]);
    setVVVF1FalutCount(CAR_6, canSignalsSlots->m_VVVF1FaultCount[2]);
    setVVVF2FalutCount(CAR_1, canSignalsSlots->m_VVVF2FaultCount[0]);
    setVVVF2FalutCount(CAR_3, canSignalsSlots->m_VVVF2FaultCount[1]);
    setVVVF2FalutCount(CAR_6, canSignalsSlots->m_VVVF2FaultCount[2]);
    setSIVFalutCount(CAR_2, canSignalsSlots->m_SIVFaultCount[0]);
    setSIVFalutCount(CAR_4, canSignalsSlots->m_SIVFaultCount[1]);
}
