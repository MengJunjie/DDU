#include "setpassword.h"
#include "ui_setpassword.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

setpassword::setpassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setpassword)
{
    ui->setupUi(this);

    labelCurrent = NULL;

    strPassword = "1111";

    strOldPassword = "";
    strNewPassword = "";
    strSurePassword = "";

    connect(ui->labelpassword_old, SIGNAL(clicked()), this, SLOT(slot_myLabel_clicked()));
    connect(ui->labelpassword_new, SIGNAL(clicked()), this, SLOT(slot_myLabel_clicked()));
    connect(ui->labelpassword_sure, SIGNAL(clicked()), this, SLOT(slot_myLabel_clicked()));

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

setpassword::~setpassword(){
    delete ui;
}

void setpassword::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void setpassword::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void setpassword::on_buttonrevise_clicked(){
    if(NULL != labelCurrent){
        labelCurrent->setText("");
    }
    buttonBeep();
    if(ui->labelpassword_old == labelCurrent){
        strOldPassword = "";
    }
    else if(ui->labelpassword_new == labelCurrent){
        strNewPassword = "";
    }
    else{
        strSurePassword = "";
    }
}

void setpassword::on_buttonset_clicked(){
    qDebug()<<"Password ::"<<strPassword;
    qDebug()<<"Old :: "<<strOldPassword;
    qDebug()<<"New :: "<<strNewPassword;
    qDebug()<<"Sure :: "<<strSurePassword;

    if(4 != strOldPassword.length() || 4 != strNewPassword.length() || 4 != strSurePassword.length()){
        return ;
    }
    buttonBeep();
    if(strPassword == strOldPassword && strNewPassword == strSurePassword){
//        dbOperation->setPassword(strNewPassword);
    }
}

void setpassword::on_buttonquit_clicked(){
    buttonBeep();
    this->reject();
}

void setpassword::slot_myLabel_clicked(){
    QLabel * tempLabel = dynamic_cast<QLabel*>(sender());
    if(NULL == labelCurrent){
        buttonBeep();
        labelCurrent = tempLabel;
        labelCurrent->setStyleSheet(QSS_BG_BLUE_BR_WHITE_WD_WHITE);
    }

    if(tempLabel != labelCurrent){
        buttonBeep();
        labelCurrent->setStyleSheet(QSS_BG_BLACK_BR_WHITE_WD_WHITE);
        labelCurrent = tempLabel;
        labelCurrent->setStyleSheet(QSS_BG_BLUE_BR_WHITE_WD_WHITE);
    }
}

void setpassword::slot_pushButton_Num_clicked(){
    QPushButton * tempBtn = dynamic_cast<QPushButton*>(sender());

    if(NULL == labelCurrent){
        return ;
    }
    buttonBeep();
    if(labelCurrent->text().length() < 4){
        if(ui->labelpassword_old == labelCurrent){
            strOldPassword += tempBtn->text();
        }
        else if(ui->labelpassword_new == labelCurrent){
            strNewPassword += tempBtn->text();
        }
        else{
            strSurePassword += tempBtn->text();
        }
        QString strText = labelCurrent->text() + "✻";
        labelCurrent->setText(strText);
    }
    else{
        return ;
    }
}
