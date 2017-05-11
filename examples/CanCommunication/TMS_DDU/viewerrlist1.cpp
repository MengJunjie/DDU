#include "viewerrlist1.h"
#include "ui_viewerrlist1.h"
#include "errorrecord.h"
#include "viewerrlist2.h"
#include "errorvvvfsiv.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

viewerrlist1::viewerrlist1(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::viewerrlist1)
{
    ui->setupUi(this);
    labelCurrent = NULL;

    Init();
    slot_CarIcon_Update();
    slot_CarCode_Update();

    slot_FaultRecordCount_Get_Update();

    connect(ui->labelinfo_2_2, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_2_3, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_2_4, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_2_5, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_2_6, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_2_7, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));

    connect(ui->labelinfo_3_2, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_3_4, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_3_5, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_3_7, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));

    connect(ui->labelinfo_4_2, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_4_4, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_4_5, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_4_7, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));

    connect(ui->labelinfo_5_3, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_5_4, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_5_5, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_5_6, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));

    connect(ui->labelinfo_6_2, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_6_3, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_6_4, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_6_5, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_6_6, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_6_7, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));

    connect(canSignalsSlots, SIGNAL(sig_Car_Code_Change()), this, SLOT(slot_CarCode_Update()));
    connect(canSignalsSlots, SIGNAL(sig_Car_Direction_Change()), this, SLOT(slot_Car_Direction_Update()));
    connect(canSignalsSlots, SIGNAL(sig_FaultRecordCount_Get_Update()), this, SLOT(slot_FaultRecordCount_Get_Update()));

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));

    canSignalsSlots->sendCarGetFaultInfoHMI(1);
}

viewerrlist1::~viewerrlist1(){
    delete ui;
}

void viewerrlist1::Init(){
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

void viewerrlist1::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void viewerrlist1::setFalutRecordCount(CAR_ID carId, int iCount){
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

void viewerrlist1::setVVVF1FalutCount(CAR_ID carId, int iCount){
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

void viewerrlist1::setVVVF2FalutCount(CAR_ID carId, int iCount){
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

void viewerrlist1::setSIVFalutCount(CAR_ID carId, int iCount){
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

void viewerrlist1::setBrakeFalutCount(CAR_ID carId, int iCount){
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

void viewerrlist1::on_buttonshow_clicked(){
    if(NULL == labelCurrent){
        return ;
    }
    buttonBeep();
    QString strCarName = "";
    int iCarId = 0, iCount = 0;

    QString strObgectName = labelCurrent->objectName();
    int itemId = strObgectName.section("_", 1, 1).toInt();
    int carId = strObgectName.section("_", 2, 2).toInt();
    int errCount = labelCurrent->text().toInt();

    switch (carId) {
    case 2:
        strCarName = ui->carlabel1->text();
        iCarId = ui->labelinfo_1_2->text().toInt();
        break;
    case 3:
        strCarName = ui->carlabel2->text();
        iCarId = ui->labelinfo_1_3->text().toInt();
        break;
    case 4:
        strCarName = ui->carlabel3->text();
        iCarId = ui->labelinfo_1_4->text().toInt();
        break;
    case 5:
        strCarName = ui->carlabel4->text();
        iCarId = ui->labelinfo_1_5->text().toInt();
        break;
    case 6:
        strCarName = ui->carlabel5->text();
        iCarId = ui->labelinfo_1_6->text().toInt();
        break;
    case 7:
        strCarName = ui->carlabel6->text();
        iCarId = ui->labelinfo_1_7->text().toInt();
        break;
    default:
        strCarName = "";
        break;
    }

    iCount = labelCurrent->text().toInt();
    if(iCount > 0){
        canSignalsSlots->sendCarGetFaultDetailDataHMI(itemId-1, iCarId);
    }
//    if("" == strValue || "0" == strValue){
//        return ;
//    }

    labelCurrent->setStyleSheet(QSS_BG_BLACK_BR_WHITE_WD_WHITE);
    labelCurrent = NULL;

//    qDebug()<<"aa == "<<strShowItem<<"  bb == "<<strCarName<<"  cc == "<<strValue;
    if(2 == itemId){
        canSignalsSlots->bNowShowERMFaultFlag = true;
        errorrecord * m_errorrecord = new errorrecord(iCarId, strCarName, errCount);
        m_errorrecord->setModal(true);
        m_errorrecord->show();
    }
    else if(3 == itemId){
        canSignalsSlots->bNowShowSubDevErrFlag = true;
        errorvvvfsiv * m_errorvvvf1 = new errorvvvfsiv(BD_VVVF1, iCarId, strCarName);
        m_errorvvvf1->setModal(true);
        m_errorvvvf1->show();
    }
    else if(4 == itemId){
        canSignalsSlots->bNowShowSubDevErrFlag = true;
        errorvvvfsiv * m_errorvvvf2 = new errorvvvfsiv(BD_VVVF2, iCarId, strCarName);
        m_errorvvvf2->setModal(true);
        m_errorvvvf2->show();
    }
    else if(5 == itemId){
        canSignalsSlots->bNowShowSubDevErrFlag = true;
        errorvvvfsiv * m_errorsiv = new errorvvvfsiv(BD_SIV, iCarId, strCarName);
        m_errorsiv->setModal(true);
        m_errorsiv->show();
    }
    else if(6 == itemId){
        canSignalsSlots->bNowShowSubDevErrFlag = true;
        viewerrlist2 * m_errorbrake = new viewerrlist2(iCarId, strCarName);
        m_errorbrake->setModal(true);
        m_errorbrake->show();
    }
    else{
        //do nothing
    }
}

void viewerrlist1::on_buttonquit_clicked(){
    buttonBeep();
    this->reject();
}

void viewerrlist1::slot_labelinfo_clicked(){
//    qDebug()<<"@@@@@@@@@@@@@@@@@@@@@@@@@@";
    QLabel * tempLabel = dynamic_cast<QLabel*>(sender());
    if("" == tempLabel->text()){
        return;
    }
    buttonBeep();
    if(NULL == labelCurrent){
        labelCurrent = tempLabel;
        labelCurrent->setStyleSheet(QSS_BG_BLUE_BR_WHITE_WD_BLACK);
    }
    else if(labelCurrent != tempLabel){
        labelCurrent->setStyleSheet(QSS_BG_BLACK_BR_WHITE_WD_WHITE);

        labelCurrent = tempLabel;
        labelCurrent->setStyleSheet(QSS_BG_BLUE_BR_WHITE_WD_BLACK);
    }
    else{
        // do nothing
    }
}

void viewerrlist1::slot_CarIcon_Update(){
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

void viewerrlist1::slot_CarCode_Update(){
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

void viewerrlist1::slot_Car_Direction_Update(){
    slot_CarIcon_Update();
    slot_CarCode_Update();

    slot_FaultRecordCount_Get_Update();

    if(NULL != labelCurrent){
        labelCurrent->setStyleSheet(QSS_BG_BLACK_BR_WHITE_WD_WHITE);
        labelCurrent = NULL;
    }
}

void viewerrlist1::slot_FaultRecordCount_Get_Update(){
    for(int i = 0; i < 6; i++){
//        qDebug()<<"BCU BCU  BCU   BCU   BCU   BCU   BCU Fault  CAr Id  ==  "<<i+1 <<"  Count  ==  "<<canSignalsSlots->m_BCUFaultCount[i];
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
