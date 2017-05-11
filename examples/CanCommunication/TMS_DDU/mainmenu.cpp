#include "mainmenu.h"
#include "ui_mainmenu.h"
#include "dialog/reparismenu.h"
#include "carinfoinit.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

mainmenu::mainmenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mainmenu)
{
    ui->setupUi(this);

    Init();
    slot_CarIcon_Update();
    slot_CarCode_Update();
    slot_ATOModel_Update(0);

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));
    connect(canSignalsSlots, SIGNAL(sig_Car_Direction_Change()), this, SLOT(slot_Car_Direction_Update()));
    connect(canSignalsSlots, SIGNAL(sig_Car_Code_Change()), this, SLOT(slot_CarCode_Update()));
    connect(canSignalsSlots, SIGNAL(sig_Car_ATOModel_Change(int)), this, SLOT(slot_ATOModel_Update(int)));
    connect(canSignalsSlots, SIGNAL(sig_HB_OFF_Update(bool)), this, SLOT(slot_HB_OFF_Update(bool)));
}

mainmenu::~mainmenu(){
    delete ui;
}

void mainmenu::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

//    ui->labelarrow->setScaledContents(true);
    ui->carimagelabel1->setScaledContents(true);
    ui->carimagelabel2->setScaledContents(true);
    ui->carimagelabel3->setScaledContents(true);
    ui->carimagelabel4->setScaledContents(true);
    ui->carimagelabel5->setScaledContents(true);
    ui->carimagelabel6->setScaledContents(true);

    ui->labelmessage_2->setVisible(false);
    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void mainmenu::slot_ATOModel_Update(int model){
    QString strText = "", strStyleSheet = "";
    ATO_MODEL atoMode = ATO_MODEL(model);
    switch (atoMode) {
    case ATO_POWER_OFF:
        strText = "ATO无效";
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        break;
    case ATO_COMN_ERR:
        strText = "ATO无效";
        strStyleSheet = QSS_BG_PURPLE_BR_PURPLE_WD_BLACK;
        break;
    case ATO_INVALID:
        strText = "ATO无效";
        strStyleSheet = QSS_BG_YELLOW_BR_NONE_WD_BLACK;
        break;
    case ATO_VALID:
        strText = "ATO模式";
        strStyleSheet = QSS_BG_GREEN_BR_NONE_WD_BLACK;
        break;
    case ATC_CUT_OFF:
        strText = "ATC切除";
        strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
        break;
    default:
        strText = "";
        strStyleSheet = "";
        break;
    }
    ui->labelATO->setText(strText);
    ui->labelATO->setStyleSheet(strStyleSheet);
}

void mainmenu::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void mainmenu::on_buttonaircondition_clicked(){
    emit sig_LowPBtn_Clicked(ACC_BTN_CLICKED);
}

void mainmenu::on_buttoncarinfo_clicked(){
    emit sig_LowPBtn_Clicked(CARINFO_BTN_CLICKED);
}

void mainmenu::on_buttonerr_clicked(){
//    emit sig_LowPBtn_Clicked(ERROR_BTN_CLICKED);
}

void mainmenu::on_buttonrun_clicked(){
    emit sig_LowPBtn_Clicked(KEEPMOVE_BTN_CLICKED);
}

void mainmenu::on_buttonrepairmenu_clicked(){
    buttonBeep();
    reparismenu * dlg_reparismenu = new reparismenu();

    dlg_reparismenu->setModal(true);
    dlg_reparismenu->show();
}

void mainmenu::on_buttoninit_clicked(){
    buttonBeep();
    carinfoinit * dlg_carinfoinit = new carinfoinit();
    connect(dlg_carinfoinit, SIGNAL(sig_LowPBtn_Clicked(int)), this, SIGNAL(sig_LowPBtn_Clicked(int)));

    dlg_carinfoinit->setModal(true);
    dlg_carinfoinit->show();
}

void mainmenu::slot_CarIcon_Update(){
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

void mainmenu::slot_CarCode_Update(){
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

void mainmenu::slot_Car_Direction_Update(){
    slot_CarIcon_Update();
    slot_CarCode_Update();
}

void mainmenu::slot_HB_OFF_Update(bool bState){
    if(bState){
        ui->labelmessage_2->setVisible(true);
    }
    else{
        ui->labelmessage_2->setVisible(false);
    }
}
