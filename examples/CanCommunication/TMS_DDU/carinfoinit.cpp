#include "carinfoinit.h"
#include "ui_carinfoinit.h"
#include "initsetsure.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

carinfoinit::carinfoinit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::carinfoinit)
{
    ui->setupUi(this);
    labelCurrent = NULL;
    strRunType = "";

    connect(ui->labelInitTableId, SIGNAL(clicked()), this, SLOT(slot_myLabel_clicked()));
    connect(ui->labelInitCarId, SIGNAL(clicked()), this, SLOT(slot_myLabel_clicked()));

    connect(ui->pushButton_0, SIGNAL(clicked(bool)), this, SLOT(slot_pushButton_Num_clicked()));
    connect(ui->pushButton_1, SIGNAL(clicked(bool)), this, SLOT(slot_pushButton_Num_clicked()));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(slot_pushButton_Num_clicked()));
    connect(ui->pushButton_3, SIGNAL(clicked(bool)), this, SLOT(slot_pushButton_Num_clicked()));
    connect(ui->pushButton_4, SIGNAL(clicked(bool)), this, SLOT(slot_pushButton_Num_clicked()));
    connect(ui->pushButton_5, SIGNAL(clicked(bool)), this, SLOT(slot_pushButton_Num_clicked()));
    connect(ui->pushButton_6, SIGNAL(clicked(bool)), this, SLOT(slot_pushButton_Num_clicked()));
    connect(ui->pushButton_7, SIGNAL(clicked(bool)), this, SLOT(slot_pushButton_Num_clicked()));
    connect(ui->pushButton_8, SIGNAL(clicked(bool)), this, SLOT(slot_pushButton_Num_clicked()));
    connect(ui->pushButton_9, SIGNAL(clicked(bool)), this, SLOT(slot_pushButton_Num_clicked()));

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));

    Init();
}

carinfoinit::~carinfoinit(){
    delete ui;
}

void carinfoinit::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void carinfoinit::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void carinfoinit::on_buttondays_clicked(){
    buttonBeep();
    strRunType = ui->buttondays->text();
}

void carinfoinit::on_buttonweekdays_clicked(){
    buttonBeep();
    strRunType = ui->buttonweekdays->text();
}

void carinfoinit::on_buttontableId_clicked(){
    buttonBeep();
}

void carinfoinit::on_buttoncarId_clicked(){
    buttonBeep();
}

void carinfoinit::on_buttonrevise_clicked(){
    if(NULL == labelCurrent){
        return ;
    }
    buttonBeep();
    labelCurrent->setText("");
}

void carinfoinit::on_buttoninput_clicked(){
    buttonBeep();
}

void carinfoinit::on_buttonmainmenu_clicked(){
    buttonBeep();
    this->reject();
}

void carinfoinit::on_buttonsure_clicked(){
//    if("" != strRunType){
        buttonBeep();
        initsetsure * dlg_initsetsure = new initsetsure();

        connect(dlg_initsetsure, SIGNAL(sig_SetSure_Sure_Clicked()), this, SLOT(slot_SetSureScreen_SurePBtn_Clicked()));
        connect(dlg_initsetsure, SIGNAL(sig_ToMainMenu()), this, SLOT(reject()));
        dlg_initsetsure->setTypeCarIdTableId(strRunType, ui->labelInitTableId->text(), ui->labelInitCarId->text());
        dlg_initsetsure->setModal(true);
        dlg_initsetsure->show();
//    }
}

void carinfoinit::slot_myLabel_clicked(){
    QLabel * tempLabel = dynamic_cast<QLabel*>(sender());
    if(NULL == labelCurrent){
        labelCurrent = tempLabel;
    }
    else{
        buttonBeep();
        labelCurrent->setStyleSheet(QSS_BG_BLACK_BR_WHITE_WD_WHITE);

        labelCurrent = tempLabel;
    }

    labelCurrent->setStyleSheet(QSS_BG_BLUE_BR_WHITE_WD_WHITE);
}

void carinfoinit::slot_pushButton_Num_clicked(){
    QPushButton * tempBtn = dynamic_cast<QPushButton*>(sender());

    if(NULL == labelCurrent){
        return ;
    }
    buttonBeep();
    QString strText = labelCurrent->text() + tempBtn->text();
    labelCurrent->setText(strText);
}

void carinfoinit::slot_SetSureScreen_SurePBtn_Clicked(){
    this->reject();
    emit sig_LowPBtn_Clicked(KEEPMOVE_BTN_CLICKED);
}
