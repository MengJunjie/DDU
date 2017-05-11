#include "setdatetime.h"
#include "ui_setdatetime.h"
#include <QProcess>

#include "../can1operation.h"
#include "../can2operation.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

setdatetime::setdatetime(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setdatetime)
{
    ui->setupUi(this);

    setFlag = FLAG_NULL;
    bDateTimeEditFlag = false;
    labelCurrent = NULL;

    Init();

    connect(ui->label_year, SIGNAL(clicked()), this, SLOT(slot_label_clicked()));
    connect(ui->label_month, SIGNAL(clicked()), this, SLOT(slot_label_clicked()));
    connect(ui->label_day, SIGNAL(clicked()), this, SLOT(slot_label_clicked()));
    connect(ui->label_hour, SIGNAL(clicked()), this, SLOT(slot_label_clicked()));
    connect(ui->label_minute, SIGNAL(clicked()), this, SLOT(slot_label_clicked()));
    connect(ui->label_second, SIGNAL(clicked()), this, SLOT(slot_label_clicked()));

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
}

setdatetime::~setdatetime(){
    delete ui;
}

void setdatetime::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void setdatetime::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);

    if(!bDateTimeEditFlag){
        ui->label_year->setText(strDateTime.mid(0, 2));
        ui->label_month->setText(strDateTime.mid(3, 2));
        ui->label_day->setText(strDateTime.mid(6, 2));
        ui->label_hour->setText(strDateTime.mid(10, 2));
        ui->label_minute->setText(strDateTime.mid(13, 2));
        ui->label_second->setText(strDateTime.mid(16, 2));
    }
}

void setdatetime::on_buttonrevise_clicked(){
    if(NULL != labelCurrent){
        buttonBeep();
        labelCurrent->setText("");
        setFlag = FLAG_CLEAR;
        bDateTimeEditFlag = true;
    }
}

void setdatetime::on_buttonstrart_clicked(){
     if(atoModelFlag){
         return;
     }
    if(FLAG_NULL == setFlag){
        return ;
    }
    buttonBeep();
    if(FLAG_REVISING == setFlag){
        labelCurrent->setText("0"+labelCurrent->text());
    }
    if(NULL != labelCurrent){
        labelCurrent->setStyleSheet(QSS_BG_BLACK_BR_WHITE_WD_WHITE);
        labelCurrent = NULL;
    }
    bool bErrorFlag = false;
    if(ui->label_month->text().toInt() > 12 || ui->label_month->text().toInt() < 1){
        bErrorFlag = true;
        goto errOperation;
    }
    if(ui->label_day->text().toInt() > 31 || ui->label_day->text().toInt() < 1){
        bErrorFlag = true;
        goto errOperation;
    }
    if(ui->label_hour->text().toInt() > 23 || ui->label_hour->text().toInt() < 0){
        bErrorFlag = true;
        goto errOperation;
    }
    if(ui->label_minute->text().toInt() > 59 || ui->label_minute->text().toInt() < 0){
        bErrorFlag = true;
        goto errOperation;
    }
    if(ui->label_second->text().toInt() > 59 || ui->label_second->text().toInt() < 0){
        bErrorFlag = true;
        goto errOperation;
    }

errOperation: {
        if(!bErrorFlag){
            int intYear = 2000 + ui->label_year->text().toInt();

            canSignalsSlots->sendCarDateTimeHMI(ui->label_year->text().toInt(), ui->label_month->text().toInt(), ui->label_day->text().toInt(), ui->label_hour->text().toInt(), ui->label_minute->text().toInt(), ui->label_second->text().toInt());

            QString tempTime = QString::number(intYear) + "-" + ui->label_month->text() + "-" + ui->label_day->text() + " " + ui->label_hour->text() + ":" + ui->label_minute->text() + ":" + ui->label_second->text();
//            qDebug()<<"Set The DateTime  ==  "<<tempTime;
            if(masterHMIFlag){
                QProcess::execute(QString("date \"%1\"").arg(tempTime));
                QProcess::execute("hwclock -w");
            }

            ui->labelmessage->setText("启动完毕，按[退出]键退出当前设置。");
            bDateTimeEditFlag = false;
        }
    }
    setFlag = FLAG_NULL;
}

void setdatetime::on_buttonquit_clicked(){
    buttonBeep();
    this->reject();
}

void setdatetime::slot_label_clicked(){
    buttonBeep();
    QLabel * tempLabel = dynamic_cast<QLabel*>(sender());
    if(FLAG_REVISING == setFlag && labelCurrent != tempLabel){
        labelCurrent->setText("0"+labelCurrent->text());
    }

    if(NULL == labelCurrent){
        labelCurrent = tempLabel;
        labelCurrent->setStyleSheet(QSS_BG_BLUE_BR_WHITE_WD_WHITE);
    }
    else if(tempLabel != labelCurrent){
        labelCurrent->setStyleSheet(QSS_BG_BLACK_BR_WHITE_WD_WHITE);
        labelCurrent = tempLabel;
        labelCurrent->setStyleSheet(QSS_BG_BLUE_BR_WHITE_WD_WHITE);
    }
    else{
        //do nothing
    }

    setFlag = FLAG_SELECT;

    ui->labelmessage->setText("设置完毕，按[启动]键启动时钟。");
}

void setdatetime::slot_pushButton_Num_clicked(){
    QPushButton * tempPBtn = dynamic_cast<QPushButton*>(sender());
    QString strText = "";

    if(FLAG_CLEAR == setFlag){
        buttonBeep();
        strText = tempPBtn->text();
        setFlag = FLAG_REVISING;
    }
    else if(FLAG_REVISING == setFlag){
        buttonBeep();
        strText = labelCurrent->text()+tempPBtn->text();
        setFlag = FLAG_FINISHED;
    }
    else{
        return ;
    }

    labelCurrent->setText(strText);
}
