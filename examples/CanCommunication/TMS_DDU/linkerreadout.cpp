#include "linkerreadout.h"
#include "ui_linkerreadout.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

linkerreadout::linkerreadout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::linkerreadout)
{
    ui->setupUi(this);

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));

    Init();
}

linkerreadout::~linkerreadout()
{
    delete ui;
}

void linkerreadout::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void linkerreadout::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void linkerreadout::on_buttonlink_clicked(){

}

void linkerreadout::on_buttonbreak_clicked(){

}

void linkerreadout::on_buttonquit_clicked(){
    this->reject();
}
