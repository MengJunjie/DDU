#include "password.h"
#include "ui_password.h"

#include "setdatetime.h"
#include "setpara.h"
#include "setpassword.h"
#include "delruninfo.h"
#include "delspeedinfo.h"
#include "delerrinfo.h"
#include "linkerreadout.h"

#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

password::password(NEXT_SCREEN_FLAG flag, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::password)
{
    ui->setupUi(this);

    nextFlag = flag;
    strPassword = "1111";

//    qDebug()<<"Default Password == "<<strPassword;

    strInputPassword = "";

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

password::~password(){
    delete ui;
}

void password::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

    ui->labelpassword->setStyleSheet(QSS_BG_BLUE_BR_WHITE_WD_WHITE);

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void password::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void password::on_buttonrevise_clicked(){
    buttonBeep();
    strInputPassword = "";
    ui->labelpassword->setText("");
}

void password::on_buttonback_clicked(){
    buttonBeep();
    this->reject();
}

void password::on_buttonsure_clicked(){
    qDebug()<<"strPassword == "<<strPassword<<"  strInputPassword == "<<strInputPassword;
    if(strPassword == strInputPassword){
        QDialog * m_Dialog = NULL;

        switch (nextFlag) {
        case SET_DATETIME:
            m_Dialog = new setdatetime();
            break;
        case SET_PARA:
            m_Dialog = new setpara();
            break;
        case SET_PASSWORD:
            m_Dialog = new setpassword();
            break;
        case DEL_ERRINFO:
            m_Dialog = new delerrinfo();
            break;
        case DEL_RUNINFO:
            m_Dialog = new delruninfo();
            break;
        case DEL_SPEEDINFO:
            m_Dialog = new delspeedinfo();
            break;
        case LINKER_READOUT:
            m_Dialog = new linkerreadout();
            break;
        default:
            m_Dialog = NULL;
            break;
        }

        if(NULL == m_Dialog){
            return ;
        }
        buttonBeep();
        ui->labelmessage_2->setText("");
        this->reject();

        m_Dialog->setModal(true);
        m_Dialog->show();
    }
//    else {
//        ui->labelmessage_2->setText(QString::fromUtf8("密码错误，请重新输入密码。"));
//        ui->labelpassword->setText("");
//        strInputPassword = "";
//    }
}

void password::slot_pushButton_Num_clicked(){
    QPushButton * tempLabel = dynamic_cast<QPushButton*>(sender());

    if(4 == ui->labelpassword->text().length()){
        return ;
    }
    buttonBeep();
    strInputPassword += tempLabel->text();

    QString strText = ui->labelpassword->text() + "*"; //"✻";
    ui->labelpassword->setText(strText);
}
