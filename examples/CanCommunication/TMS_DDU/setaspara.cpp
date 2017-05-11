#include "setaspara.h"
#include "ui_setaspara.h"
#include "../cansignalsslots.h"
#include "../can1operation.h"
#include "../can2operation.h"

extern CanSignalsSlots * canSignalsSlots;

setaspara::setaspara(int itemId, QString strCarId, QString strCarCode, QString strValue, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setaspara)
{
    ui->setupUi(this);

    this->itemId = itemId;
    carId = strCarId.toInt();

    ui->labelcarId->setText(strCarCode);
    ui->labeloldvalue->setText(strValue);
    ui->labelnewvalue->setText(strValue);

    intValueLength = 3;

    QString strUnit = "", strItemName = "";
    switch (itemId) {
    case 2:
        strUnit = "";
        strItemName = "车辆编号";
        intValueLength = 4;
        break;
    case 3:
        strUnit = "mm";
        strItemName = "车轮直径";
        intValueLength = 3;
        break;
    case 4:
        strUnit = "kPa";
        strItemName = "AS压力(满)";
        intValueLength = 3;
        break;
    case 5:
        strUnit = "kPa";
        strItemName = "AS压力(空)";
        intValueLength = 3;
        break;
    default:
        strUnit = "";
        strItemName = "";
        intValueLength = 3;
        break;
    }
    ui->labelrevisetype->setText(strItemName);

    ui->labelunit1->setText(strUnit);
    ui->labelunit2->setText(strUnit);

    bReviseFlag = false;

    Init();

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
//    qDebug()<<"Set AS Para :: "<< this->itemId << " @@   "<< strCarId<<"  @@ "<<strItemName <<"  @@  "<< strCarCode <<"  @@  "<<strValue;
}

setaspara::~setaspara(){
    delete ui;
}

void setaspara::Init(){
    setWindowFlags(Qt::FramelessWindowHint);
    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
    ui->labelnewvalue->setStyleSheet(QSS_BG_BLUE_BR_WHITE_WD_WHITE);
}

void setaspara::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void setaspara::on_buttonrevise_clicked(){
    buttonBeep();
    bReviseFlag = true;
    ui->labelnewvalue->setText("");
}

void setaspara::on_buttonset_clicked(){
    int tempValue = ui->labelnewvalue->text().toInt();
    if(3 == itemId){
        if(tempValue < 770 || tempValue > 840){
                return;
        }
    }
    if(intValueLength == ui->labelnewvalue->text().length()){
        if((3 == intValueLength && tempValue > 99 && tempValue < 1000) || (4 == intValueLength && tempValue > 999 && tempValue < 10000)){
            buttonBeep();
            qDebug()<<"HMI Set $$$$$$$$$$$$$$$$  Para  Value  ==  "<<tempValue;
            canSignalsSlots->sendCarParaData_Changed(CAR_ID(carId), itemId, tempValue);
            if(masterHMIFlag && 2 != itemId){
                emit sig_setValue(tempValue);
            }
            this->reject();
        }
    }
}

void setaspara::on_buttonquit_clicked(){
    buttonBeep();
    this->reject();
}

void setaspara::slot_pushButton_Num_clicked(){
    if(bReviseFlag){
        QPushButton * tempBtn = dynamic_cast<QPushButton*>(sender());

        if(0 == ui->labelnewvalue->text().length() && "0" == tempBtn->text()){
            return;
        }
        buttonBeep();
        if(ui->labelnewvalue->text().length() < intValueLength){
            QString strText = ui->labelnewvalue->text()+tempBtn->text();

            ui->labelnewvalue->setText(strText);
        }
    }
}
