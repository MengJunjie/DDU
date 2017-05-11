#include "runtable.h"
#include "ui_runtable.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

runtable::runtable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::runtable)
{
    ui->setupUi(this);

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));

    Init();
}

runtable::~runtable(){
    delete ui;
}

void runtable::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void runtable::setCarTableId(QString strtableId, QString strcarId){
    ui->labeltableId->setText(strtableId);
    ui->labelcarId->setText(strcarId);
}

void runtable::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void runtable::on_buttonquit_clicked(){
    buttonBeep();
    this->reject();
}
