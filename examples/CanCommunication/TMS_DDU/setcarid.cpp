#include "setcarid.h"
#include "ui_setcarid.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

setCarId::setCarId(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setCarId)
{
    ui->setupUi(this);

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));

    Init();
}

setCarId::~setCarId()
{
    delete ui;
}

void setCarId::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));

    ui->labelCarId->setStyleSheet(QSS_BG_BLACK_BR_WHITE_WD_WHITE);
    ui->labelDevCode->setStyleSheet(QSS_BG_BLACK_BR_WHITE_WD_WHITE);

    QStringList carList;
    carList << "   头   车" << "   尾   车";
    ui->comboBox->addItems(carList);

    curCarId = deviceCarId;
    if(1 == deviceCarId){
        ui->comboBox->setCurrentIndex(0);
    }
    else{
        ui->comboBox->setCurrentIndex(1);
    }
}

void setCarId::setDevNodeId(int nodeId){
    QSettings * iniWriter = new QSettings("/appbin/DDUConfig.ini", QSettings::IniFormat);

    iniWriter->setValue(tr("/DDU/CarId"), nodeId);

    delete iniWriter;
}

void setCarId::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void setCarId::on_comboBox_currentIndexChanged(int index){
    buttonBeep();
    if(0 == index){
        ui->labelCarId->setText("1");
        ui->labelDevCode->setText("DDU1");
    }
    else{
        ui->labelCarId->setText("6");
        ui->labelDevCode->setText("DDU2");
    }
    ui->labelMessage->setText(QString::fromUtf8("车厢号设置完成后，点击[确定]按钮，保存设置。"));
}

void setCarId::on_buttoncalibrate_clicked(){
    buttonBeep();
    system("reCalibrate.sh");
}

void setCarId::on_buttonset_clicked(){
    if(curCarId != ui->labelCarId->text().toInt()){
        buttonBeep();
        curCarId = ui->labelCarId->text().toInt();
        setDevNodeId(curCarId);
        ui->labelMessage->setText(QString::fromUtf8("车厢号设置成功，请重新启动设备，重启后车厢号生效。"));
    }
}

void setCarId::on_buttonquit_clicked(){
    buttonBeep();
    this->reject();
}
