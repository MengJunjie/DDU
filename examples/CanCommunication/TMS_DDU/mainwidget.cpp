#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "../canbootup.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);

    bCarOrRunScreenFlag = false;
    bErrorScreenFlag = false;
    intLastWidgetIndex = 0;

    m_mainmenu = new mainmenu();
    connect(m_mainmenu, SIGNAL(sig_LowPBtn_Clicked(int)), this, SLOT(slot_LowPBtn_Clicked(int)));

    m_airconditioninfo = new airconditioninfo();
    connect(m_airconditioninfo, SIGNAL(sig_ACC_BackPBtn_Clicked()), this, SLOT(slot_ACC_BackPBtn_Clicked()));

    m_MainCarInfo = new MainCarInfo();
    connect(m_MainCarInfo, SIGNAL(sig_LowPBtn_Clicked(int)), this, SLOT(slot_LowPBtn_Clicked(int)));

    m_errinfo = new errinfo();
    connect(m_errinfo, SIGNAL(sig_Error_Happened()), this, SLOT(slot_Error_Happened()));
    connect(m_errinfo, SIGNAL(sig_Error_OKBtn_Clicked()), this, SLOT(slot_ErrorScreen_OKBtn_Clicked()));

    m_keepmove = new keepmove();
    connect(m_keepmove, SIGNAL(sig_LowPBtn_Clicked(int)), this, SLOT(slot_LowPBtn_Clicked(int)));

    connect(canSignalsSlots, SIGNAL(sig_Speed_Door_State_Update(int)), this, SLOT(slot_Speed_Door_State_Update(int)));
    connect(canSignalsSlots, SIGNAL(sig_PTUDownloadState_Update(bool)), this, SLOT(slot_PTUDownloadState_Update(bool)));

    ui->stackedWidget->addWidget(m_mainmenu);
    ui->stackedWidget->addWidget(m_airconditioninfo);
    ui->stackedWidget->addWidget(m_MainCarInfo);
    ui->stackedWidget->addWidget(m_errinfo);
    ui->stackedWidget->addWidget(m_keepmove);

    ui->stackedWidget->setCurrentIndex(0);

    CanBootUp * canBootUp = new CanBootUp();
    canBootUp->timerCan1_Start();
    canBootUp->timerCan2_Start();

    canSignalsSlots->slot_ScreenData_Refresh_400MS();
}

MainWidget::~MainWidget(){
    delete ui;
}

void MainWidget::slot_LowPBtn_Clicked(int btnId){
    if(bErrorScreenFlag && ERROR_BTN_CLICKED == btnId){
        return;
    }
    buttonBeep();
    switch (btnId) {
    case MAINMENU_BTN_CLICKED:
        ui->stackedWidget->setCurrentIndex(0);
        bCarOrRunScreenFlag = false;
        bErrorScreenFlag = false;
        intLastWidgetIndex = 0;
        break;
    case ACC_BTN_CLICKED:
        ui->stackedWidget->setCurrentIndex(1);
        bCarOrRunScreenFlag = false;
        bErrorScreenFlag = false;
        break;
    case CARINFO_BTN_CLICKED:
        ui->stackedWidget->setCurrentIndex(2);
        bCarOrRunScreenFlag = true;
        bErrorScreenFlag = false;
        intLastWidgetIndex = 1;
        break;
    case ERROR_BTN_CLICKED:
        ui->stackedWidget->setCurrentIndex(3);
        bCarOrRunScreenFlag = false;
        bErrorScreenFlag = true;
        break;
    case KEEPMOVE_BTN_CLICKED:
        ui->stackedWidget->setCurrentIndex(4);
        bCarOrRunScreenFlag = true;
        bErrorScreenFlag = false;
        intLastWidgetIndex = 2;
        break;
    default:
        ui->stackedWidget->setCurrentIndex(0);
        bCarOrRunScreenFlag = false;
        bErrorScreenFlag = false;
        intLastWidgetIndex = 0;
        break;
    }
    if(bErrorScreenFlag){
        m_errinfo->showErrorScreen();
    }
}

void MainWidget::slot_Error_Happened(){
    if(bCarOrRunScreenFlag){
        slot_LowPBtn_Clicked(ERROR_BTN_CLICKED);
    }
}

void MainWidget::slot_ErrorScreen_OKBtn_Clicked(){
    switch (intLastWidgetIndex) {
    case 0:
        ui->stackedWidget->setCurrentIndex(0);
        bCarOrRunScreenFlag = false;
        bErrorScreenFlag = false;
        break;
    case 1:
        ui->stackedWidget->setCurrentIndex(2);
        bCarOrRunScreenFlag = true;
        bErrorScreenFlag = false;
        break;
    case 2:
        ui->stackedWidget->setCurrentIndex(4);
        bCarOrRunScreenFlag = true;
        bErrorScreenFlag = false;
        break;
    default:
        ui->stackedWidget->setCurrentIndex(0);
        bCarOrRunScreenFlag = false;
        bErrorScreenFlag = false;
        intLastWidgetIndex = 0;
        break;
    }
}

void MainWidget::slot_ACC_BackPBtn_Clicked(){
    switch (intLastWidgetIndex) {
    case 0:
        ui->stackedWidget->setCurrentIndex(0);
        bCarOrRunScreenFlag = false;
        bErrorScreenFlag = false;
        break;
    case 1:
        ui->stackedWidget->setCurrentIndex(2);
        bCarOrRunScreenFlag = true;
        bErrorScreenFlag = false;
        break;
    case 2:
        ui->stackedWidget->setCurrentIndex(4);
        bCarOrRunScreenFlag = true;
        bErrorScreenFlag = false;
        break;
    default:
        ui->stackedWidget->setCurrentIndex(0);
        bCarOrRunScreenFlag = false;
        bErrorScreenFlag = false;
        intLastWidgetIndex = 0;
        break;
    }
}

void MainWidget::slot_Speed_Door_State_Update(int iState){
    if(bCarOrRunScreenFlag){
        if(1 == iState){  //车速变为零，且门开好
            if(2 == ui->stackedWidget->currentIndex()){
                slot_LowPBtn_Clicked(KEEPMOVE_BTN_CLICKED);
            }
        }
        else if(2 == iState){  //车门关好，且车速不为零
            if(4 == ui->stackedWidget->currentIndex()){
                slot_LowPBtn_Clicked(CARINFO_BTN_CLICKED);
            }
        }
    }
}

void MainWidget::slot_PTUDownloadState_Update(bool bFlag){
    if(bFlag){
        m_PTUDownload = new PTUDownload();
        m_PTUDownload->setModal(true);
        m_PTUDownload->show();
    }
    else{
        m_PTUDownload->reject();
    }
}
