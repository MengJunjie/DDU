#include "setpara.h"
#include "ui_setpara.h"
#include "setaspara.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

setpara::setpara(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setpara)
{
    ui->setupUi(this);

    labelCurrent = NULL;

    Init();
    slot_CarIcon_Update();
    slot_CarCode_Update();
    allDataInit();

    connect(ui->labelinfo_2_2, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_2_3, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_2_4, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_2_5, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_2_6, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_2_7, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));

    connect(ui->labelinfo_3_2, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_3_3, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_3_4, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_3_5, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_3_6, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_3_7, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));

    connect(ui->labelinfo_4_2, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_4_3, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_4_4, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_4_5, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_4_6, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_4_7, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));

    connect(ui->labelinfo_5_2, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_5_3, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_5_4, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_5_5, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_5_6, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));
    connect(ui->labelinfo_5_7, SIGNAL(clicked()), this, SLOT(slot_labelinfo_clicked()));

    connect(canSignalsSlots, SIGNAL(sig_Car_Code_Change()), this, SLOT(slot_CarCode_Update()));
    connect(canSignalsSlots, SIGNAL(sig_Car_Direction_Change()), this, SLOT(slot_Car_Direction_Update()));
    connect(canSignalsSlots, SIGNAL(sig_Car_ParaData_Update(int)), this, SLOT(slot_Car_ParaData_Update(int)));

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));
}

setpara::~setpara(){
    delete ui;
}

void setpara::Init(){
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

void setpara::allDataInit(){
    slot_Car_ParaData_Update(1);
    slot_Car_ParaData_Update(2);
    slot_Car_ParaData_Update(3);
    slot_Car_ParaData_Update(4);
}

void setpara::setValue(SETPARA_ITEM item, CAR_ID carId, int iValue){
    QLabel * tempLabel = NULL;
    switch (item) {
    case PARA_CAR_ID:
        switch (carId) {
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
            tempLabel = NULL;
            break;
        }
        break;
    case PARA_WHEEL_MM:
        switch (carId) {
        case CAR_1:
            tempLabel = ui->labelinfo_3_2;
            break;
        case CAR_2:
            tempLabel = ui->labelinfo_3_3;
            break;
        case CAR_3:
            tempLabel = ui->labelinfo_3_4;
            break;
        case CAR_4:
            tempLabel = ui->labelinfo_3_5;
            break;
        case CAR_5:
            tempLabel = ui->labelinfo_3_6;
            break;
        case CAR_6:
            tempLabel = ui->labelinfo_3_7;
            break;
        default:
            tempLabel = NULL;
            break;
        }
        break;
    case PARA_AS_FULL:
        switch (carId) {
        case CAR_1:
            tempLabel = ui->labelinfo_4_2;
            break;
        case CAR_2:
            tempLabel = ui->labelinfo_4_3;
            break;
        case CAR_3:
            tempLabel = ui->labelinfo_4_4;
            break;
        case CAR_4:
            tempLabel = ui->labelinfo_4_5;
            break;
        case CAR_5:
            tempLabel = ui->labelinfo_4_6;
            break;
        case CAR_6:
            tempLabel = ui->labelinfo_4_7;
            break;
        default:
            tempLabel = NULL;
            break;
        }
        break;
    case PARA_AS_EMPTY:
        switch (carId) {
        case CAR_1:
            tempLabel = ui->labelinfo_5_2;
            break;
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
        case CAR_6:
            tempLabel = ui->labelinfo_5_7;
            break;
        default:
            tempLabel = NULL;
            break;
        }
        break;
    default:
        tempLabel = NULL;
        break;
    }

    if(NULL == tempLabel){
        return ;
    }
    tempLabel->setText(QString::number(iValue));
}

void setpara::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void setpara::slot_labelinfo_clicked(){
    QLabel * tempLabel = dynamic_cast<QLabel*>(sender());
    if(NULL == labelCurrent){
        buttonBeep();
        labelCurrent = tempLabel;
        labelCurrent->setStyleSheet(QSS_BG_BLUE_BR_WHITE_WD_BLACK);
    }
    else if(labelCurrent != tempLabel){
        buttonBeep();
        labelCurrent->setStyleSheet(QSS_BG_BLACK_BR_WHITE_WD_WHITE);

        labelCurrent = tempLabel;
        labelCurrent->setStyleSheet(QSS_BG_BLUE_BR_WHITE_WD_BLACK);
    }
    else{
        // do nothing
    }
}

void setpara::on_buttonchange_clicked(){
    if(NULL == labelCurrent){
        return ;
    }
    buttonBeep();
    QString strCarId = "",  strCarCode = "", strValue = "";

    QString strObgectName = labelCurrent->objectName();
    int itemId = strObgectName.section("_", 1, 1).toInt();
    int carId = strObgectName.section("_", 2, 2).toInt();

    switch (carId) {
    case 2:
        strCarId = ui->labelinfo_1_2->text();
        strCarCode = ui->labelinfo_2_2->text();
        break;
    case 3:
        strCarId = ui->labelinfo_1_3->text();
        strCarCode = ui->labelinfo_2_3->text();
        break;
    case 4:
        strCarId = ui->labelinfo_1_4->text();
        strCarCode = ui->labelinfo_2_4->text();
        break;
    case 5:
        strCarId = ui->labelinfo_1_5->text();
        strCarCode = ui->labelinfo_2_5->text();
        break;
    case 6:
        strCarId = ui->labelinfo_1_6->text();
        strCarCode = ui->labelinfo_2_6->text();
        break;
    case 7:
        strCarId = ui->labelinfo_1_7->text();
        strCarCode = ui->labelinfo_2_7->text();
        break;
    default:
        strCarId = "";
        strCarCode = "";
        break;
    }
    strValue = labelCurrent->text();

    setaspara * m_setaspara = new setaspara(itemId, strCarId, strCarCode, strValue);
    connect(m_setaspara, SIGNAL(sig_setValue(int)), this, SLOT(slot_setValue(int)));
    m_setaspara->setModal(true);
    m_setaspara->show();
}

void setpara::on_buttonquit_clicked(){
    buttonBeep();
    this->reject();
}

void setpara::slot_setValue(int iValue){
    labelCurrent->setText(QString::number(iValue));
}

void setpara::slot_CarIcon_Update(){
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

void setpara::slot_CarCode_Update(){
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

void setpara::slot_Car_Direction_Update(){
    slot_CarIcon_Update();
    slot_CarCode_Update();
    allDataInit();
}

void setpara::slot_Car_ParaData_Update(int dataFalg){
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        switch (dataFalg) {
        case 1:
            setValue(PARA_CAR_ID, CAR_6, carCodeNum[0]);
            setValue(PARA_CAR_ID, CAR_5, carCodeNum[1]);
            setValue(PARA_CAR_ID, CAR_4, carCodeNum[2]);
            setValue(PARA_CAR_ID, CAR_3, carCodeNum[3]);
            setValue(PARA_CAR_ID, CAR_2, carCodeNum[4]);
            setValue(PARA_CAR_ID, CAR_1, carCodeNum[5]);
            break;
        case 2:
            setValue(PARA_WHEEL_MM, CAR_6, canSignalsSlots->m_CarWheel[0]);
            setValue(PARA_WHEEL_MM, CAR_5, canSignalsSlots->m_CarWheel[1]);
            setValue(PARA_WHEEL_MM, CAR_4, canSignalsSlots->m_CarWheel[2]);
            setValue(PARA_WHEEL_MM, CAR_3, canSignalsSlots->m_CarWheel[3]);
            setValue(PARA_WHEEL_MM, CAR_2, canSignalsSlots->m_CarWheel[4]);
            setValue(PARA_WHEEL_MM, CAR_1, canSignalsSlots->m_CarWheel[5]);
            break;
        case 3:
            setValue(PARA_AS_FULL, CAR_6, canSignalsSlots->m_CarASFull[0]);
            setValue(PARA_AS_FULL, CAR_5, canSignalsSlots->m_CarASFull[1]);
            setValue(PARA_AS_FULL, CAR_4, canSignalsSlots->m_CarASFull[2]);
            setValue(PARA_AS_FULL, CAR_3, canSignalsSlots->m_CarASFull[3]);
            setValue(PARA_AS_FULL, CAR_2, canSignalsSlots->m_CarASFull[4]);
            setValue(PARA_AS_FULL, CAR_1, canSignalsSlots->m_CarASFull[5]);
            break;
        case 4:
            setValue(PARA_AS_EMPTY, CAR_6, canSignalsSlots->m_CarASEmpty[0]);
            setValue(PARA_AS_EMPTY, CAR_5, canSignalsSlots->m_CarASEmpty[1]);
            setValue(PARA_AS_EMPTY, CAR_4, canSignalsSlots->m_CarASEmpty[2]);
            setValue(PARA_AS_EMPTY, CAR_3, canSignalsSlots->m_CarASEmpty[3]);
            setValue(PARA_AS_EMPTY, CAR_2, canSignalsSlots->m_CarASEmpty[4]);
            setValue(PARA_AS_EMPTY, CAR_1, canSignalsSlots->m_CarASEmpty[5]);
            break;
        default:
            break;
        }
    }
    else{
        switch (dataFalg) {
        case 1:
            setValue(PARA_CAR_ID, CAR_1, carCodeNum[0]);
            setValue(PARA_CAR_ID, CAR_2, carCodeNum[1]);
            setValue(PARA_CAR_ID, CAR_3, carCodeNum[2]);
            setValue(PARA_CAR_ID, CAR_4, carCodeNum[3]);
            setValue(PARA_CAR_ID, CAR_5, carCodeNum[4]);
            setValue(PARA_CAR_ID, CAR_6, carCodeNum[5]);
            break;
        case 2:
            setValue(PARA_WHEEL_MM, CAR_1, canSignalsSlots->m_CarWheel[0]);
            setValue(PARA_WHEEL_MM, CAR_2, canSignalsSlots->m_CarWheel[1]);
            setValue(PARA_WHEEL_MM, CAR_3, canSignalsSlots->m_CarWheel[2]);
            setValue(PARA_WHEEL_MM, CAR_4, canSignalsSlots->m_CarWheel[3]);
            setValue(PARA_WHEEL_MM, CAR_5, canSignalsSlots->m_CarWheel[4]);
            setValue(PARA_WHEEL_MM, CAR_6, canSignalsSlots->m_CarWheel[5]);
            break;
        case 3:
            setValue(PARA_AS_FULL, CAR_1, canSignalsSlots->m_CarASFull[0]);
            setValue(PARA_AS_FULL, CAR_2, canSignalsSlots->m_CarASFull[1]);
            setValue(PARA_AS_FULL, CAR_3, canSignalsSlots->m_CarASFull[2]);
            setValue(PARA_AS_FULL, CAR_4, canSignalsSlots->m_CarASFull[3]);
            setValue(PARA_AS_FULL, CAR_5, canSignalsSlots->m_CarASFull[4]);
            setValue(PARA_AS_FULL, CAR_6, canSignalsSlots->m_CarASFull[5]);
            break;
        case 4:
            setValue(PARA_AS_EMPTY, CAR_1, canSignalsSlots->m_CarASEmpty[0]);
            setValue(PARA_AS_EMPTY, CAR_2, canSignalsSlots->m_CarASEmpty[1]);
            setValue(PARA_AS_EMPTY, CAR_3, canSignalsSlots->m_CarASEmpty[2]);
            setValue(PARA_AS_EMPTY, CAR_4, canSignalsSlots->m_CarASEmpty[3]);
            setValue(PARA_AS_EMPTY, CAR_5, canSignalsSlots->m_CarASEmpty[4]);
            setValue(PARA_AS_EMPTY, CAR_6, canSignalsSlots->m_CarASEmpty[5]);
            break;
        default:
            break;
        }
    }
}
