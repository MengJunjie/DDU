#include "initsetsure.h"
#include "ui_initsetsure.h"
#include "setstation.h"
#include "runtable.h"
#include "setdirection.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

initsetsure::initsetsure(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::initsetsure)
{
    ui->setupUi(this);

    iCurrStationId = canSignalsSlots->iCurrStation;
    iEndStationId = canSignalsSlots->iEndStation;

    Init();

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));
}

initsetsure::~initsetsure(){
    delete ui;
}

void initsetsure::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

    ui->labelshowcurstation->setText(canSignalsSlots->getCarStationName(iCurrStationId));
    ui->labelshowendstation->setText(canSignalsSlots->getCarStationName(iEndStationId));
    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void initsetsure::setTypeCarIdTableId(QString strtype, QString strtableId, QString strcarId){
    ui->labelshowtype->setText(strtype);
    ui->labelshowtableId->setText(strtableId);
    ui->labelshowcarId->setText(strcarId);
}

void initsetsure::setCurrentStation(QString strstation){
    ui->labelshowcurstation->setText(strstation);
}

void initsetsure::setEndStation(QString strstation){
    ui->labelshowendstation->setText(strstation);
}

void initsetsure::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void initsetsure::on_buttoncurstation_clicked(){
    buttonBeep();
    setstation * dlg_setstation = new setstation();

    connect(dlg_setstation, SIGNAL(sig_setStationId(int,int)), this, SLOT(slot_SetStationId(int,int)));
    dlg_setstation->setTitleName(1);
    dlg_setstation->setModal(true);
    dlg_setstation->show();
}

void initsetsure::on_buttonendstation_clicked(){
    buttonBeep();
    setstation * dlg_setstation = new setstation();

    connect(dlg_setstation, SIGNAL(sig_setStationId(int,int)), this, SLOT(slot_SetStationId(int,int)));
    dlg_setstation->setTitleName(2);
    dlg_setstation->setModal(true);
    dlg_setstation->show();
}

void initsetsure::on_buttonruntable_clicked(){
    buttonBeep();
    runtable * dlg_runtable = new runtable();

    dlg_runtable->setCarTableId(ui->labelshowtableId->text(), ui->labelshowcarId->text());
    dlg_runtable->setModal(true);
    dlg_runtable->show();
}

void initsetsure::on_buttondirectionsure_clicked(){
    buttonBeep();
    setdirection * dlg_setdirection = new setdirection();

    dlg_setdirection->setModal(true);
    dlg_setdirection->show();
}

void initsetsure::on_buttontmainmenu_clicked(){
    buttonBeep();
    this->reject();
    emit sig_ToMainMenu();
}

void initsetsure::on_buttontback_clicked(){
    buttonBeep();
    this->reject();
}

void initsetsure::on_buttonsure_clicked(){
    buttonBeep();
    canSignalsSlots->setCarStationManual(iCurrStationId, iEndStationId);
    this->reject();
    emit sig_SetSure_Sure_Clicked();
}

void initsetsure::slot_SetStationId(int flag, int stationId){
    if(1 == flag){
        iCurrStationId = stationId;
        ui->labelshowcurstation->setText(canSignalsSlots->getCarStationName(stationId));
    }
    else if(2 == flag){
        iEndStationId = stationId;
        ui->labelshowendstation->setText(canSignalsSlots->getCarStationName(stationId));
    }
    else{
        //do nothing
    }
}
