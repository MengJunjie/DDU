#include "brakecheck.h"
#include "ui_brakecheck.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

brakecheck::brakecheck(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::brakecheck)
{
    ui->setupUi(this);

    iCarTestFlag = 0;
    bCheckingFlag = false;
    iNowTestLevel = 0;
    b4SFlag = false;
    labelList.clear();
    labelList << ui->labelinfo_3_1 << ui->labelinfo_4_1 << ui->labelinfo_5_1 << ui->labelinfo_6_1 << ui->labelinfo_7_1 << ui->labelinfo_8_1 << ui->labelinfo_9_1 << ui->labelinfo_10_1 << ui->labelinfo_11_1;

    Init();
    testDataValueInit();
    slot_CarIcon_Update();
    slot_CarCode_Update();

    timerBCUTest = new QTimer();
    connect(timerBCUTest, SIGNAL(timeout()), this, SLOT(slot_BCUTest_TimeOut()));
    timerRefresh = new QTimer();
    connect(timerRefresh, SIGNAL(timeout()), this, SLOT(slot_Refresh_TimeOut()));

    connect(canSignalsSlots, SIGNAL(sig_Car_Code_Change()), this, SLOT(slot_CarCode_Update()));
    connect(canSignalsSlots, SIGNAL(sig_Car_Direction_Change()), this, SLOT(slot_Car_Direction_Update()));

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));
}

brakecheck::~brakecheck(){
    delete ui;
}

void  brakecheck::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

    //MC，M车空载BC压力下限
    M_MC_Empty_Lower[0] = 0;
    M_MC_Empty_Lower[1] = 14;
    M_MC_Empty_Lower[2] = 49;
    M_MC_Empty_Lower[3] = 86;
    M_MC_Empty_Lower[4] = 125;
    M_MC_Empty_Lower[5] = 166;
    M_MC_Empty_Lower[6] = 208;
    M_MC_Empty_Lower[7] = 253;
    M_MC_Empty_Lower[8] = 301;
    //MC，M车空载BC压力上限
    M_MC_Empty_Upper[0] = 35;
    M_MC_Empty_Upper[1] = 83;
    M_MC_Empty_Upper[2] = 118;
    M_MC_Empty_Upper[3] = 155;
    M_MC_Empty_Upper[4] = 194;
    M_MC_Empty_Upper[5] = 235;
    M_MC_Empty_Upper[6] = 277;
    M_MC_Empty_Upper[7] = 322;
    M_MC_Empty_Upper[8] = 370;
    //T，T1，T2车空载BC压力下限
    T_T1_T2_Empty_Lower[0] = 0;
    T_T1_T2_Empty_Lower[1] = 7;
    T_T1_T2_Empty_Lower[2] = 34;
    T_T1_T2_Empty_Lower[3] = 62;
    T_T1_T2_Empty_Lower[4] = 92;
    T_T1_T2_Empty_Lower[5] = 122;
    T_T1_T2_Empty_Lower[6] = 154;
    T_T1_T2_Empty_Lower[7] = 187;
    T_T1_T2_Empty_Lower[8] = 222;
    //T，T1，T2车空载BC压力上限
    T_T1_T2_Empty_Upper[0] = 35;
    T_T1_T2_Empty_Upper[1] = 76;
    T_T1_T2_Empty_Upper[2] = 103;
    T_T1_T2_Empty_Upper[3] = 131;
    T_T1_T2_Empty_Upper[4] = 161;
    T_T1_T2_Empty_Upper[5] = 191;
    T_T1_T2_Empty_Upper[6] = 223;
    T_T1_T2_Empty_Upper[7] = 256;
    T_T1_T2_Empty_Upper[8] = 291;
    //MC，M车满载BC压力下限
    M_MC_Full_Lower[0] = 0;
    M_MC_Full_Lower[1] = 32;
    M_MC_Full_Lower[2] = 87;
    M_MC_Full_Lower[3] = 146;
    M_MC_Full_Lower[4] = 210;
    M_MC_Full_Lower[5] = 278;
    M_MC_Full_Lower[6] = 351;
    M_MC_Full_Lower[7] = 429;
    M_MC_Full_Lower[8] = 512;
    //MC，M车满载BC压力上限
    M_MC_Full_Upper[0] = 35;
    M_MC_Full_Upper[1] = 101;
    M_MC_Full_Upper[2] = 156;
    M_MC_Full_Upper[3] = 215;
    M_MC_Full_Upper[4] = 279;
    M_MC_Full_Upper[5] = 347;
    M_MC_Full_Upper[6] = 420;
    M_MC_Full_Upper[7] = 498;
    M_MC_Full_Upper[8] = 581;
    //T，T1，T2车满载BC压力下限
    T_T1_T2_Full_Lower[0] = 0;
    T_T1_T2_Full_Lower[1] = 24;
    T_T1_T2_Full_Lower[2] = 71;
    T_T1_T2_Full_Lower[3] = 120;
    T_T1_T2_Full_Lower[4] = 173;
    T_T1_T2_Full_Lower[5] = 229;
    T_T1_T2_Full_Lower[6] = 288;
    T_T1_T2_Full_Lower[7] = 351;
    T_T1_T2_Full_Lower[8] = 418;
    //T，T1，T2车满载BC压力上限
    T_T1_T2_Full_Upper[0] = 35;
    T_T1_T2_Full_Upper[1] = 93;
    T_T1_T2_Full_Upper[2] = 140;
    T_T1_T2_Full_Upper[3] = 189;
    T_T1_T2_Full_Upper[4] = 242;
    T_T1_T2_Full_Upper[5] = 298;
    T_T1_T2_Full_Upper[6] = 357;
    T_T1_T2_Full_Upper[7] = 420;
    T_T1_T2_Full_Upper[8] = 487;

//    ui->labelarrow->setScaledContents(true);
    ui->carimagelabel1->setScaledContents(true);
    ui->carimagelabel2->setScaledContents(true);
    ui->carimagelabel3->setScaledContents(true);
    ui->carimagelabel4->setScaledContents(true);
    ui->carimagelabel5->setScaledContents(true);
    ui->carimagelabel6->setScaledContents(true);

    ui->buttonrecord->setEnabled(false);
    ui->buttonstrart->setVisible(false);
    ui->buttonstop->setVisible(false);
    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void brakecheck::testDataValueInit(){
    //测试数据初始化
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 7; j++){
            m_PWM_BC_Value[i][j] = -1;
        }
    }
}

void brakecheck::setTestLevel(){
    for(int i = 0; i < labelList.length(); i++){
        labelList.at(i)->setStyleSheet(QSS_BG_BLACK_BR_WHITE_WD_WHITE);
    }
    QLabel * tempLabel = NULL;
    QString strText = "";
    switch (iNowTestLevel) {
    case 0:
        tempLabel = NULL;
        strText = "按[开始]键，进入制动试验，按[终止]键，终止正在进入的试验";
        break;
    case 1:
        tempLabel = ui->labelinfo_3_1;
        strText = "请进入OFF位";
        break;
    case 2:
        tempLabel = ui->labelinfo_4_1;
        strText = "请进入制动1/7";
        break;
    case 3:
        tempLabel = ui->labelinfo_5_1;
        strText = "请进入制动2/7";
        break;
    case 4:
        tempLabel = ui->labelinfo_6_1;
        strText = "请进入制动3/7";
        break;
    case 5:
        tempLabel = ui->labelinfo_7_1;
        strText = "请进入制动4/7";
        break;
    case 6:
        tempLabel = ui->labelinfo_8_1;
        strText = "请进入制动5/7";
        break;
    case 7:
        tempLabel = ui->labelinfo_9_1;
        strText = "请进入制动6/7";
        break;
    case 8:
        tempLabel = ui->labelinfo_10_1;
        strText = "请进入制动7/7";
        break;
    case 9:
        tempLabel = ui->labelinfo_11_1;
        strText = "请进入紧急级位";
        break;
    case 10:
        tempLabel = NULL;
        strText = "测试结束";
        break;
    default:
        break;
    }
    if(NULL != tempLabel){
        tempLabel->setStyleSheet(QSS_BG_BLUE_BR_WHITE_WD_BLACK);
    }
    ui->labelmessage->setText(strText);
}

void brakecheck::setASValue(){
    for(int i = 0; i < 6; i++){
        canSignalsSlots->mBCUCheckSendData[i+1] = canSignalsSlots->m_BCUASPress[i];
    }

    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        ui->labelinfo_2_2->setText(QString("%1").arg(canSignalsSlots->m_BCUASPress[5]));
        ui->labelinfo_2_3->setText(QString("%1").arg(canSignalsSlots->m_BCUASPress[4]));
        ui->labelinfo_2_4->setText(QString("%1").arg(canSignalsSlots->m_BCUASPress[3]));
        ui->labelinfo_2_5->setText(QString("%1").arg(canSignalsSlots->m_BCUASPress[2]));
        ui->labelinfo_2_6->setText(QString("%1").arg(canSignalsSlots->m_BCUASPress[1]));
        ui->labelinfo_2_7->setText(QString("%1").arg(canSignalsSlots->m_BCUASPress[0]));
    }
    else{
        ui->labelinfo_2_2->setText(QString("%1").arg(canSignalsSlots->m_BCUASPress[0]));
        ui->labelinfo_2_3->setText(QString("%1").arg(canSignalsSlots->m_BCUASPress[1]));
        ui->labelinfo_2_4->setText(QString("%1").arg(canSignalsSlots->m_BCUASPress[2]));
        ui->labelinfo_2_5->setText(QString("%1").arg(canSignalsSlots->m_BCUASPress[3]));
        ui->labelinfo_2_6->setText(QString("%1").arg(canSignalsSlots->m_BCUASPress[4]));
        ui->labelinfo_2_7->setText(QString("%1").arg(canSignalsSlots->m_BCUASPress[5]));
    }
}

QString brakecheck::getDisplayQSS(bool bMCFlag, int iLevel, int iValue){
    QString strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
    if(-1 != iValue){
        if(bMCFlag){
            if(1 == iCarTestFlag){  // 1：空车
                if(iValue >= M_MC_Empty_Lower[iLevel-1] && iValue <= M_MC_Empty_Upper[iLevel-1]){
                    strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
                }
                else{
                    strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
                }
            }
            else if(2 == iCarTestFlag){  // 2：超员
                if(iValue >= M_MC_Full_Lower[iLevel-1] && iValue <= M_MC_Full_Upper[iLevel-1]){
                    strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
                }
                else{
                    strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
                }
            }
            else{
                // do nothing
            }
        }
        else{
            if(1 == iCarTestFlag){ // 1：空车
                if(iValue >= T_T1_T2_Empty_Lower[iLevel-1] && iValue <= T_T1_T2_Empty_Upper[iLevel-1]){
                    strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
                }
                else{
                    strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
                }
            }
            else if(2 == iCarTestFlag){  // 2：超员
                if(iValue >= T_T1_T2_Full_Lower[iLevel-1] && iValue <= T_T1_T2_Full_Upper[iLevel-1]){
                    strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
                }
                else{
                    strStyleSheet = QSS_BG_RED_BR_WHITE_WD_WHITE;
                }
            }
            else{
                // do nothing
            }
        }
    }
    return strStyleSheet;
}

void brakecheck::setPWMBCValue(int iFlag){
    switch (iFlag) {
    case 1:  // OFF
        m_PWM_BC_Value[0][0] = canSignalsSlots->m_CarPWMValue;
        for(int i = 0; i < 6; i++){
            m_PWM_BC_Value[0][i+1] = canSignalsSlots->m_BCUBCPress[i];
        }
        break;
    case 2:  // 1/7
        m_PWM_BC_Value[1][0] = canSignalsSlots->m_CarPWMValue;
        for(int i = 0; i < 6; i++){
            m_PWM_BC_Value[1][i+1] = canSignalsSlots->m_BCUBCPress[i];
        }
        break;
    case 3:  // 2/7
        m_PWM_BC_Value[2][0] = canSignalsSlots->m_CarPWMValue;
        for(int i = 0; i < 6; i++){
            m_PWM_BC_Value[2][i+1] = canSignalsSlots->m_BCUBCPress[i];
        }
        break;
    case 4:  // 3/7
        m_PWM_BC_Value[3][0] = canSignalsSlots->m_CarPWMValue;
        for(int i = 0; i < 6; i++){
            m_PWM_BC_Value[3][i+1] = canSignalsSlots->m_BCUBCPress[i];
        }
        break;
    case 5:  // 4/7
        m_PWM_BC_Value[4][0] = canSignalsSlots->m_CarPWMValue;
        for(int i = 0; i < 6; i++){
            m_PWM_BC_Value[4][i+1] = canSignalsSlots->m_BCUBCPress[i];
        }
        break;
    case 6:  // 5/7
        m_PWM_BC_Value[5][0] = canSignalsSlots->m_CarPWMValue;
        for(int i = 0; i < 6; i++){
            m_PWM_BC_Value[5][i+1] = canSignalsSlots->m_BCUBCPress[i];
        }
        break;
    case 7:  // 6/7
        m_PWM_BC_Value[6][0] = canSignalsSlots->m_CarPWMValue;
        for(int i = 0; i < 6; i++){
            m_PWM_BC_Value[6][i+1] = canSignalsSlots->m_BCUBCPress[i];
        }
        break;
    case 8:  // 7/7
        m_PWM_BC_Value[7][0] = canSignalsSlots->m_CarPWMValue;
        for(int i = 0; i < 6; i++){
            m_PWM_BC_Value[7][i+1] = canSignalsSlots->m_BCUBCPress[i];
        }
        break;
    case 9:  // 紧急
        m_PWM_BC_Value[8][0] = canSignalsSlots->m_CarPWMValue;
        for(int i = 0; i < 6; i++){
            m_PWM_BC_Value[8][i+1] = canSignalsSlots->m_BCUBCPress[i];
        }
        break;
    default:
        break;
    }
}

void brakecheck::setPWMBCDisplay(int iFlag){
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        switch (iFlag) {
        case 1:  // OFF
            ui->labelinfo_3_8->setText(QString("%1").arg(m_PWM_BC_Value[0][0]));
            ui->labelinfo_3_2->setText(QString("%1").arg(m_PWM_BC_Value[0][6]));
            ui->labelinfo_3_3->setText(QString("%1").arg(m_PWM_BC_Value[0][5]));
            ui->labelinfo_3_4->setText(QString("%1").arg(m_PWM_BC_Value[0][4]));
            ui->labelinfo_3_5->setText(QString("%1").arg(m_PWM_BC_Value[0][3]));
            ui->labelinfo_3_6->setText(QString("%1").arg(m_PWM_BC_Value[0][2]));
            ui->labelinfo_3_7->setText(QString("%1").arg(m_PWM_BC_Value[0][1]));
            break;
        case 2:  // 1/7
            ui->labelinfo_4_8->setText(QString("%1").arg(m_PWM_BC_Value[1][0]));
            ui->labelinfo_4_2->setText(QString("%1").arg(m_PWM_BC_Value[1][6]));
            ui->labelinfo_4_3->setText(QString("%1").arg(m_PWM_BC_Value[1][5]));
            ui->labelinfo_4_4->setText(QString("%1").arg(m_PWM_BC_Value[1][4]));
            ui->labelinfo_4_5->setText(QString("%1").arg(m_PWM_BC_Value[1][3]));
            ui->labelinfo_4_6->setText(QString("%1").arg(m_PWM_BC_Value[1][2]));
            ui->labelinfo_4_7->setText(QString("%1").arg(m_PWM_BC_Value[1][1]));
            break;
        case 3:  // 2/7
            ui->labelinfo_5_8->setText(QString("%1").arg(m_PWM_BC_Value[2][0]));
            ui->labelinfo_5_2->setText(QString("%1").arg(m_PWM_BC_Value[2][6]));
            ui->labelinfo_5_3->setText(QString("%1").arg(m_PWM_BC_Value[2][5]));
            ui->labelinfo_5_4->setText(QString("%1").arg(m_PWM_BC_Value[2][4]));
            ui->labelinfo_5_5->setText(QString("%1").arg(m_PWM_BC_Value[2][3]));
            ui->labelinfo_5_6->setText(QString("%1").arg(m_PWM_BC_Value[2][2]));
            ui->labelinfo_5_7->setText(QString("%1").arg(m_PWM_BC_Value[2][1]));
            break;
        case 4:  // 3/7
            ui->labelinfo_6_8->setText(QString("%1").arg(m_PWM_BC_Value[3][0]));
            ui->labelinfo_6_2->setText(QString("%1").arg(m_PWM_BC_Value[3][6]));
            ui->labelinfo_6_3->setText(QString("%1").arg(m_PWM_BC_Value[3][5]));
            ui->labelinfo_6_4->setText(QString("%1").arg(m_PWM_BC_Value[3][4]));
            ui->labelinfo_6_5->setText(QString("%1").arg(m_PWM_BC_Value[3][3]));
            ui->labelinfo_6_6->setText(QString("%1").arg(m_PWM_BC_Value[3][2]));
            ui->labelinfo_6_7->setText(QString("%1").arg(m_PWM_BC_Value[3][1]));
            break;
        case 5:  // 4/7
            ui->labelinfo_7_8->setText(QString("%1").arg(m_PWM_BC_Value[4][0]));
            ui->labelinfo_7_2->setText(QString("%1").arg(m_PWM_BC_Value[4][6]));
            ui->labelinfo_7_3->setText(QString("%1").arg(m_PWM_BC_Value[4][5]));
            ui->labelinfo_7_4->setText(QString("%1").arg(m_PWM_BC_Value[4][4]));
            ui->labelinfo_7_5->setText(QString("%1").arg(m_PWM_BC_Value[4][3]));
            ui->labelinfo_7_6->setText(QString("%1").arg(m_PWM_BC_Value[4][2]));
            ui->labelinfo_7_7->setText(QString("%1").arg(m_PWM_BC_Value[4][1]));
            break;
        case 6:  // 5/7
            ui->labelinfo_8_8->setText(QString("%1").arg(m_PWM_BC_Value[5][0]));
            ui->labelinfo_8_2->setText(QString("%1").arg(m_PWM_BC_Value[5][6]));
            ui->labelinfo_8_3->setText(QString("%1").arg(m_PWM_BC_Value[5][5]));
            ui->labelinfo_8_4->setText(QString("%1").arg(m_PWM_BC_Value[5][4]));
            ui->labelinfo_8_5->setText(QString("%1").arg(m_PWM_BC_Value[5][3]));
            ui->labelinfo_8_6->setText(QString("%1").arg(m_PWM_BC_Value[5][2]));
            ui->labelinfo_8_7->setText(QString("%1").arg(m_PWM_BC_Value[5][1]));
            break;
        case 7:  // 6/7
            ui->labelinfo_9_8->setText(QString("%1").arg(m_PWM_BC_Value[6][0]));
            ui->labelinfo_9_2->setText(QString("%1").arg(m_PWM_BC_Value[6][6]));
            ui->labelinfo_9_3->setText(QString("%1").arg(m_PWM_BC_Value[6][5]));
            ui->labelinfo_9_4->setText(QString("%1").arg(m_PWM_BC_Value[6][4]));
            ui->labelinfo_9_5->setText(QString("%1").arg(m_PWM_BC_Value[6][3]));
            ui->labelinfo_9_6->setText(QString("%1").arg(m_PWM_BC_Value[6][2]));
            ui->labelinfo_9_7->setText(QString("%1").arg(m_PWM_BC_Value[6][1]));
            break;
        case 8:  // 7/7
            ui->labelinfo_10_8->setText(QString("%1").arg(m_PWM_BC_Value[7][0]));
            ui->labelinfo_10_2->setText(QString("%1").arg(m_PWM_BC_Value[7][6]));
            ui->labelinfo_10_3->setText(QString("%1").arg(m_PWM_BC_Value[7][5]));
            ui->labelinfo_10_4->setText(QString("%1").arg(m_PWM_BC_Value[7][4]));
            ui->labelinfo_10_5->setText(QString("%1").arg(m_PWM_BC_Value[7][3]));
            ui->labelinfo_10_6->setText(QString("%1").arg(m_PWM_BC_Value[7][2]));
            ui->labelinfo_10_7->setText(QString("%1").arg(m_PWM_BC_Value[7][1]));
            break;
        case 9:  // 紧急
            ui->labelinfo_11_8->setText(QString("%1").arg(m_PWM_BC_Value[8][0]));
            ui->labelinfo_11_2->setText(QString("%1").arg(m_PWM_BC_Value[8][6]));
            ui->labelinfo_11_3->setText(QString("%1").arg(m_PWM_BC_Value[8][5]));
            ui->labelinfo_11_4->setText(QString("%1").arg(m_PWM_BC_Value[8][4]));
            ui->labelinfo_11_5->setText(QString("%1").arg(m_PWM_BC_Value[8][3]));
            ui->labelinfo_11_6->setText(QString("%1").arg(m_PWM_BC_Value[8][2]));
            ui->labelinfo_11_7->setText(QString("%1").arg(m_PWM_BC_Value[8][1]));
            break;
        default:
            break;
        }
    }
    else{
        switch (iFlag) {
        case 1:  // OFF
            ui->labelinfo_3_8->setText(QString("%1").arg(m_PWM_BC_Value[0][0]));
            ui->labelinfo_3_2->setText(QString("%1").arg(m_PWM_BC_Value[0][1]));
            ui->labelinfo_3_3->setText(QString("%1").arg(m_PWM_BC_Value[0][2]));
            ui->labelinfo_3_4->setText(QString("%1").arg(m_PWM_BC_Value[0][3]));
            ui->labelinfo_3_5->setText(QString("%1").arg(m_PWM_BC_Value[0][4]));
            ui->labelinfo_3_6->setText(QString("%1").arg(m_PWM_BC_Value[0][5]));
            ui->labelinfo_3_7->setText(QString("%1").arg(m_PWM_BC_Value[0][6]));
            break;
        case 2:  // 1/7
            ui->labelinfo_4_8->setText(QString("%1").arg(m_PWM_BC_Value[1][0]));
            ui->labelinfo_4_2->setText(QString("%1").arg(m_PWM_BC_Value[1][1]));
            ui->labelinfo_4_3->setText(QString("%1").arg(m_PWM_BC_Value[1][2]));
            ui->labelinfo_4_4->setText(QString("%1").arg(m_PWM_BC_Value[1][3]));
            ui->labelinfo_4_5->setText(QString("%1").arg(m_PWM_BC_Value[1][4]));
            ui->labelinfo_4_6->setText(QString("%1").arg(m_PWM_BC_Value[1][5]));
            ui->labelinfo_4_7->setText(QString("%1").arg(m_PWM_BC_Value[1][6]));
            break;
        case 3:  // 2/7
            ui->labelinfo_5_8->setText(QString("%1").arg(m_PWM_BC_Value[2][0]));
            ui->labelinfo_5_2->setText(QString("%1").arg(m_PWM_BC_Value[2][1]));
            ui->labelinfo_5_3->setText(QString("%1").arg(m_PWM_BC_Value[2][2]));
            ui->labelinfo_5_4->setText(QString("%1").arg(m_PWM_BC_Value[2][3]));
            ui->labelinfo_5_5->setText(QString("%1").arg(m_PWM_BC_Value[2][4]));
            ui->labelinfo_5_6->setText(QString("%1").arg(m_PWM_BC_Value[2][5]));
            ui->labelinfo_5_7->setText(QString("%1").arg(m_PWM_BC_Value[2][6]));
            break;
        case 4:  // 3/7
            ui->labelinfo_6_8->setText(QString("%1").arg(m_PWM_BC_Value[3][0]));
            ui->labelinfo_6_2->setText(QString("%1").arg(m_PWM_BC_Value[3][1]));
            ui->labelinfo_6_3->setText(QString("%1").arg(m_PWM_BC_Value[3][2]));
            ui->labelinfo_6_4->setText(QString("%1").arg(m_PWM_BC_Value[3][3]));
            ui->labelinfo_6_5->setText(QString("%1").arg(m_PWM_BC_Value[3][4]));
            ui->labelinfo_6_6->setText(QString("%1").arg(m_PWM_BC_Value[3][5]));
            ui->labelinfo_6_7->setText(QString("%1").arg(m_PWM_BC_Value[3][6]));
            break;
        case 5:  // 4/7
            ui->labelinfo_7_8->setText(QString("%1").arg(m_PWM_BC_Value[4][0]));
            ui->labelinfo_7_2->setText(QString("%1").arg(m_PWM_BC_Value[4][1]));
            ui->labelinfo_7_3->setText(QString("%1").arg(m_PWM_BC_Value[4][2]));
            ui->labelinfo_7_4->setText(QString("%1").arg(m_PWM_BC_Value[4][3]));
            ui->labelinfo_7_5->setText(QString("%1").arg(m_PWM_BC_Value[4][4]));
            ui->labelinfo_7_6->setText(QString("%1").arg(m_PWM_BC_Value[4][5]));
            ui->labelinfo_7_7->setText(QString("%1").arg(m_PWM_BC_Value[4][6]));
            break;
        case 6:  // 5/7
            ui->labelinfo_8_8->setText(QString("%1").arg(m_PWM_BC_Value[5][0]));
            ui->labelinfo_8_2->setText(QString("%1").arg(m_PWM_BC_Value[5][1]));
            ui->labelinfo_8_3->setText(QString("%1").arg(m_PWM_BC_Value[5][2]));
            ui->labelinfo_8_4->setText(QString("%1").arg(m_PWM_BC_Value[5][3]));
            ui->labelinfo_8_5->setText(QString("%1").arg(m_PWM_BC_Value[5][4]));
            ui->labelinfo_8_6->setText(QString("%1").arg(m_PWM_BC_Value[5][5]));
            ui->labelinfo_8_7->setText(QString("%1").arg(m_PWM_BC_Value[5][6]));
            break;
        case 7:  // 6/7
            ui->labelinfo_9_8->setText(QString("%1").arg(m_PWM_BC_Value[6][0]));
            ui->labelinfo_9_2->setText(QString("%1").arg(m_PWM_BC_Value[6][1]));
            ui->labelinfo_9_3->setText(QString("%1").arg(m_PWM_BC_Value[6][2]));
            ui->labelinfo_9_4->setText(QString("%1").arg(m_PWM_BC_Value[6][3]));
            ui->labelinfo_9_5->setText(QString("%1").arg(m_PWM_BC_Value[6][4]));
            ui->labelinfo_9_6->setText(QString("%1").arg(m_PWM_BC_Value[6][5]));
            ui->labelinfo_9_7->setText(QString("%1").arg(m_PWM_BC_Value[6][6]));
            break;
        case 8:  // 7/7
            ui->labelinfo_10_8->setText(QString("%1").arg(m_PWM_BC_Value[7][0]));
            ui->labelinfo_10_2->setText(QString("%1").arg(m_PWM_BC_Value[7][1]));
            ui->labelinfo_10_3->setText(QString("%1").arg(m_PWM_BC_Value[7][2]));
            ui->labelinfo_10_4->setText(QString("%1").arg(m_PWM_BC_Value[7][3]));
            ui->labelinfo_10_5->setText(QString("%1").arg(m_PWM_BC_Value[7][4]));
            ui->labelinfo_10_6->setText(QString("%1").arg(m_PWM_BC_Value[7][5]));
            ui->labelinfo_10_7->setText(QString("%1").arg(m_PWM_BC_Value[7][6]));
            break;
        case 9:  // 紧急
            ui->labelinfo_11_8->setText(QString("%1").arg(m_PWM_BC_Value[8][0]));
            ui->labelinfo_11_2->setText(QString("%1").arg(m_PWM_BC_Value[8][1]));
            ui->labelinfo_11_3->setText(QString("%1").arg(m_PWM_BC_Value[8][2]));
            ui->labelinfo_11_4->setText(QString("%1").arg(m_PWM_BC_Value[8][3]));
            ui->labelinfo_11_5->setText(QString("%1").arg(m_PWM_BC_Value[8][4]));
            ui->labelinfo_11_6->setText(QString("%1").arg(m_PWM_BC_Value[8][5]));
            ui->labelinfo_11_7->setText(QString("%1").arg(m_PWM_BC_Value[8][6]));
            break;
        default:
            break;
        }
    }
}

void brakecheck::setPWMBCResultDisplay(int iFlag){
    QString strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
    QString strTemp = "";
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        switch (iFlag) {
        case 1:  // OFF
            ui->labelinfo_3_8->setText(m_PWM_BC_Value[0][0]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[0][0]));
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[0][6]);
            ui->labelinfo_3_2->setText(m_PWM_BC_Value[0][6]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[0][6]));
            ui->labelinfo_3_2->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[0][5]);
            ui->labelinfo_3_3->setText(m_PWM_BC_Value[0][5]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[0][5]));
            ui->labelinfo_3_3->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[0][4]);
            ui->labelinfo_3_4->setText(m_PWM_BC_Value[0][4]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[0][4]));
            ui->labelinfo_3_4->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[0][3]);
            ui->labelinfo_3_5->setText(m_PWM_BC_Value[0][3]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[0][3]));
            ui->labelinfo_3_5->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[0][2]);
            ui->labelinfo_3_6->setText(m_PWM_BC_Value[0][2]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[0][2]));
            ui->labelinfo_3_6->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[0][1]);
            ui->labelinfo_3_7->setText(m_PWM_BC_Value[0][1]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[0][1]));
            ui->labelinfo_3_7->setStyleSheet(strStyleSheet);
            break;
        case 2:  // 1/7
            ui->labelinfo_4_8->setText(m_PWM_BC_Value[1][0]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[1][0]));
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[1][6]);
            ui->labelinfo_4_2->setText(m_PWM_BC_Value[1][6]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[1][6]));
            ui->labelinfo_4_2->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[1][5]);
            ui->labelinfo_4_3->setText(m_PWM_BC_Value[1][5]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[1][5]));
            ui->labelinfo_4_3->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[1][4]);
            ui->labelinfo_4_4->setText(m_PWM_BC_Value[1][4]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[1][4]));
            ui->labelinfo_4_4->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[1][3]);
            ui->labelinfo_4_5->setText(m_PWM_BC_Value[1][3]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[1][3]));
            ui->labelinfo_4_5->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[1][2]);
            ui->labelinfo_4_6->setText(m_PWM_BC_Value[1][2]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[1][2]));
            ui->labelinfo_4_6->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[1][1]);
            ui->labelinfo_4_7->setText(m_PWM_BC_Value[1][1]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[1][1]));
            ui->labelinfo_4_7->setStyleSheet(strStyleSheet);
            break;
        case 3:  // 2/7
            ui->labelinfo_5_8->setText(m_PWM_BC_Value[2][0]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[2][0]));
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[2][6]);
            ui->labelinfo_5_2->setText(m_PWM_BC_Value[2][6]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[2][6]));
            ui->labelinfo_5_2->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[2][5]);
            ui->labelinfo_5_3->setText(m_PWM_BC_Value[2][5]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[2][5]));
            ui->labelinfo_5_3->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[2][4]);
            ui->labelinfo_5_4->setText(m_PWM_BC_Value[2][4]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[2][4]));
            ui->labelinfo_5_4->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[2][3]);
            ui->labelinfo_5_5->setText(m_PWM_BC_Value[2][3]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[2][3]));
            ui->labelinfo_5_5->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[2][2]);
            ui->labelinfo_5_6->setText(m_PWM_BC_Value[2][2]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[2][2]));
            ui->labelinfo_5_6->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[2][1]);
            ui->labelinfo_5_7->setText(m_PWM_BC_Value[2][1]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[2][1]));
            ui->labelinfo_5_7->setStyleSheet(strStyleSheet);
            break;
        case 4:  // 3/7
            ui->labelinfo_6_8->setText(m_PWM_BC_Value[3][0]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[3][0]));
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[3][6]);
            ui->labelinfo_6_2->setText(m_PWM_BC_Value[3][6]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[3][6]));
            ui->labelinfo_6_2->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[3][5]);
            ui->labelinfo_6_3->setText(m_PWM_BC_Value[3][5]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[3][5]));
            ui->labelinfo_6_3->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[3][4]);
            ui->labelinfo_6_4->setText(m_PWM_BC_Value[3][4]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[3][4]));
            ui->labelinfo_6_4->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[3][3]);
            ui->labelinfo_6_5->setText(m_PWM_BC_Value[3][3]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[3][3]));
            ui->labelinfo_6_5->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[3][2]);
            ui->labelinfo_6_6->setText(m_PWM_BC_Value[3][2]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[3][2]));
            ui->labelinfo_6_6->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[3][1]);
            ui->labelinfo_6_7->setText(m_PWM_BC_Value[3][1]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[3][1]));
            ui->labelinfo_6_7->setStyleSheet(strStyleSheet);
            break;
        case 5:  // 4/7
            ui->labelinfo_7_8->setText(m_PWM_BC_Value[4][0]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[4][0]));
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[4][6]);
            ui->labelinfo_7_2->setText(m_PWM_BC_Value[4][6]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[4][6]));
            ui->labelinfo_7_2->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[4][5]);
            ui->labelinfo_7_3->setText(m_PWM_BC_Value[4][5]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[4][5]));
            ui->labelinfo_7_3->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[4][4]);
            ui->labelinfo_7_4->setText(m_PWM_BC_Value[4][4]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[4][4]));
            ui->labelinfo_7_4->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[4][3]);
            ui->labelinfo_7_5->setText(m_PWM_BC_Value[4][3]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[4][3]));
            ui->labelinfo_7_5->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[4][2]);
            ui->labelinfo_7_6->setText(m_PWM_BC_Value[4][2]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[4][2]));
            ui->labelinfo_7_6->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[4][1]);
            ui->labelinfo_7_7->setText(m_PWM_BC_Value[4][1]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[4][1]));
            ui->labelinfo_7_7->setStyleSheet(strStyleSheet);
            break;
        case 6:  // 5/7
            ui->labelinfo_8_8->setText(m_PWM_BC_Value[5][0]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[5][0]));
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[5][6]);
            ui->labelinfo_8_2->setText(m_PWM_BC_Value[5][6]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[5][6]));
            ui->labelinfo_8_2->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[5][5]);
            ui->labelinfo_8_3->setText(m_PWM_BC_Value[5][5]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[5][5]));
            ui->labelinfo_8_3->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[5][4]);
            ui->labelinfo_8_4->setText(m_PWM_BC_Value[5][4]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[5][4]));
            ui->labelinfo_8_4->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[5][3]);
            ui->labelinfo_8_5->setText(m_PWM_BC_Value[5][3]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[5][3]));
            ui->labelinfo_8_5->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[5][2]);
            ui->labelinfo_8_6->setText(m_PWM_BC_Value[5][2]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[5][2]));
            ui->labelinfo_8_6->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[5][1]);
            ui->labelinfo_8_7->setText(m_PWM_BC_Value[5][1]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[5][1]));
            ui->labelinfo_8_7->setStyleSheet(strStyleSheet);
            break;
        case 7:  // 6/7
            ui->labelinfo_9_8->setText(m_PWM_BC_Value[6][0]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[6][0]));
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[6][6]);
            ui->labelinfo_9_2->setText(m_PWM_BC_Value[6][6]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[6][6]));
            ui->labelinfo_9_2->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[6][5]);
            ui->labelinfo_9_3->setText(m_PWM_BC_Value[6][5]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[6][5]));
            ui->labelinfo_9_3->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[6][4]);
            ui->labelinfo_9_4->setText(m_PWM_BC_Value[6][4]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[6][4]));
            ui->labelinfo_9_4->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[6][3]);
            ui->labelinfo_9_5->setText(m_PWM_BC_Value[6][3]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[6][3]));
            ui->labelinfo_9_5->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[6][2]);
            ui->labelinfo_9_6->setText(m_PWM_BC_Value[6][2]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[6][2]));
            ui->labelinfo_9_6->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[6][1]);
            ui->labelinfo_9_7->setText(m_PWM_BC_Value[6][1]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[6][1]));
            ui->labelinfo_9_7->setStyleSheet(strStyleSheet);
            break;
        case 8:  // 7/7
            ui->labelinfo_10_8->setText(m_PWM_BC_Value[7][0]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[7][0]));
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[7][6]);
            ui->labelinfo_10_2->setText(m_PWM_BC_Value[7][6]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[7][6]));
            ui->labelinfo_10_2->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[7][5]);
            ui->labelinfo_10_3->setText(m_PWM_BC_Value[7][5]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[7][5]));
            ui->labelinfo_10_3->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[7][4]);
            ui->labelinfo_10_4->setText(m_PWM_BC_Value[7][4]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[7][4]));
            ui->labelinfo_10_4->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[7][3]);
            ui->labelinfo_10_5->setText(m_PWM_BC_Value[7][3]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[7][3]));
            ui->labelinfo_10_5->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[7][2]);
            ui->labelinfo_10_6->setText(m_PWM_BC_Value[7][2]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[7][2]));
            ui->labelinfo_10_6->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[7][1]);
            ui->labelinfo_10_7->setText(m_PWM_BC_Value[7][1]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[7][1]));
            ui->labelinfo_10_7->setStyleSheet(strStyleSheet);
            break;
        case 9:  // 紧急
            ui->labelinfo_11_8->setText(m_PWM_BC_Value[8][0]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[8][0]));
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[8][6]);
            ui->labelinfo_11_2->setText(m_PWM_BC_Value[8][6]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[8][6]));
            ui->labelinfo_11_2->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[8][5]);
            ui->labelinfo_11_3->setText(m_PWM_BC_Value[8][5]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[8][5]));
            ui->labelinfo_11_3->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[8][4]);
            ui->labelinfo_11_4->setText(m_PWM_BC_Value[8][4]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[8][4]));
            ui->labelinfo_11_4->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[8][3]);
            ui->labelinfo_11_5->setText(m_PWM_BC_Value[8][3]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[8][3]));
            ui->labelinfo_11_5->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[8][2]);
            ui->labelinfo_11_6->setText(m_PWM_BC_Value[8][2]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[8][2]));
            ui->labelinfo_11_6->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[8][1]);
            ui->labelinfo_11_7->setText(m_PWM_BC_Value[8][1]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[8][1]));
            ui->labelinfo_11_7->setStyleSheet(strStyleSheet);
            break;
        default:
            break;
        }
    }
    else{
        switch (iFlag) {
        case 1:  // OFF
            ui->labelinfo_3_8->setText(m_PWM_BC_Value[0][0]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[0][0]));
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[0][1]);
            ui->labelinfo_3_2->setText(m_PWM_BC_Value[0][1]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[0][1]));
            ui->labelinfo_3_2->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[0][2]);
            ui->labelinfo_3_3->setText(m_PWM_BC_Value[0][2]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[0][2]));
            ui->labelinfo_3_3->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[0][3]);
            ui->labelinfo_3_4->setText(m_PWM_BC_Value[0][3]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[0][3]));
            ui->labelinfo_3_4->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[0][4]);
            ui->labelinfo_3_5->setText(m_PWM_BC_Value[0][4]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[0][4]));
            ui->labelinfo_3_5->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[0][5]);
            ui->labelinfo_3_6->setText(m_PWM_BC_Value[0][5]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[0][5]));
            ui->labelinfo_3_6->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[0][6]);
            ui->labelinfo_3_7->setText(m_PWM_BC_Value[0][6]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[0][6]));
            ui->labelinfo_3_7->setStyleSheet(strStyleSheet);
            break;
        case 2:  // 1/7
            ui->labelinfo_4_8->setText(m_PWM_BC_Value[1][0]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[1][0]));
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[1][1]);
            ui->labelinfo_4_2->setText(m_PWM_BC_Value[1][1]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[1][1]));
            ui->labelinfo_4_2->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[1][2]);
            ui->labelinfo_4_3->setText(m_PWM_BC_Value[1][2]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[1][2]));
            ui->labelinfo_4_3->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[1][3]);
            ui->labelinfo_4_4->setText(m_PWM_BC_Value[1][3]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[1][3]));
            ui->labelinfo_4_4->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[1][4]);
            ui->labelinfo_4_5->setText(m_PWM_BC_Value[1][4]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[1][4]));
            ui->labelinfo_4_5->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[1][5]);
            ui->labelinfo_4_6->setText(m_PWM_BC_Value[1][5]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[1][5]));
            ui->labelinfo_4_6->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[1][6]);
            ui->labelinfo_4_7->setText(m_PWM_BC_Value[1][6]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[1][6]));
            ui->labelinfo_4_7->setStyleSheet(strStyleSheet);
            break;
        case 3:  // 2/7
            ui->labelinfo_5_8->setText(m_PWM_BC_Value[2][0]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[2][0]));
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[2][1]);
            ui->labelinfo_5_2->setText(m_PWM_BC_Value[2][1]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[2][1]));
            ui->labelinfo_5_2->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[2][2]);
            ui->labelinfo_5_3->setText(m_PWM_BC_Value[2][2]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[2][2]));
            ui->labelinfo_5_3->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[2][3]);
            ui->labelinfo_5_4->setText(m_PWM_BC_Value[2][3]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[2][3]));
            ui->labelinfo_5_4->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[2][4]);
            ui->labelinfo_5_5->setText(m_PWM_BC_Value[2][4]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[2][4]));
            ui->labelinfo_5_5->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[2][5]);
            ui->labelinfo_5_6->setText(m_PWM_BC_Value[2][5]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[2][5]));
            ui->labelinfo_5_6->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[2][6]);
            ui->labelinfo_5_7->setText(m_PWM_BC_Value[2][6]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[2][6]));
            ui->labelinfo_5_7->setStyleSheet(strStyleSheet);
            break;
        case 4:  // 3/7
            ui->labelinfo_6_8->setText(m_PWM_BC_Value[3][0]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[3][0]));
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[3][1]);
            ui->labelinfo_6_2->setText(m_PWM_BC_Value[3][1]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[3][1]));
            ui->labelinfo_6_2->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[3][2]);
            ui->labelinfo_6_3->setText(m_PWM_BC_Value[3][2]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[3][2]));
            ui->labelinfo_6_3->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[3][3]);
            ui->labelinfo_6_4->setText(m_PWM_BC_Value[3][3]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[3][3]));
            ui->labelinfo_6_4->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[3][4]);
            ui->labelinfo_6_5->setText(m_PWM_BC_Value[3][4]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[3][4]));
            ui->labelinfo_6_5->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[3][5]);
            ui->labelinfo_6_6->setText(m_PWM_BC_Value[3][5]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[3][5]));
            ui->labelinfo_6_6->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[3][6]);
            ui->labelinfo_6_7->setText(m_PWM_BC_Value[3][6]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[3][6]));
            ui->labelinfo_6_7->setStyleSheet(strStyleSheet);
            break;
        case 5:  // 4/7
            ui->labelinfo_7_8->setText(m_PWM_BC_Value[4][0]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[4][0]));
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[4][1]);
            ui->labelinfo_7_2->setText(m_PWM_BC_Value[4][1]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[4][1]));
            ui->labelinfo_7_2->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[4][2]);
            ui->labelinfo_7_3->setText(m_PWM_BC_Value[4][2]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[4][2]));
            ui->labelinfo_7_3->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[4][3]);
            ui->labelinfo_7_4->setText(m_PWM_BC_Value[4][3]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[4][3]));
            ui->labelinfo_7_4->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[4][4]);
            ui->labelinfo_7_5->setText(m_PWM_BC_Value[4][4]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[4][4]));
            ui->labelinfo_7_5->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[4][5]);
            ui->labelinfo_7_6->setText(m_PWM_BC_Value[4][5]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[4][5]));
            ui->labelinfo_7_6->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[4][6]);
            ui->labelinfo_7_7->setText(m_PWM_BC_Value[4][6]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[4][6]));
            ui->labelinfo_7_7->setStyleSheet(strStyleSheet);
            break;
        case 6:  // 5/7
            ui->labelinfo_8_8->setText(m_PWM_BC_Value[5][0]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[5][0]));
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[5][1]);
            ui->labelinfo_8_2->setText(m_PWM_BC_Value[5][1]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[5][1]));
            ui->labelinfo_8_2->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[5][2]);
            ui->labelinfo_8_3->setText(m_PWM_BC_Value[5][2]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[5][2]));
            ui->labelinfo_8_3->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[5][3]);
            ui->labelinfo_8_4->setText(m_PWM_BC_Value[5][3]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[5][3]));
            ui->labelinfo_8_4->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[5][4]);
            ui->labelinfo_8_5->setText(m_PWM_BC_Value[5][4]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[5][4]));
            ui->labelinfo_8_5->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[5][5]);
            ui->labelinfo_8_6->setText(m_PWM_BC_Value[5][5]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[5][5]));
            ui->labelinfo_8_6->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[5][6]);
            ui->labelinfo_8_7->setText(m_PWM_BC_Value[5][6]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[5][6]));
            ui->labelinfo_8_7->setStyleSheet(strStyleSheet);
            break;
        case 7:  // 6/7
            ui->labelinfo_9_8->setText(m_PWM_BC_Value[6][0]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[6][0]));
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[6][1]);
            ui->labelinfo_9_2->setText(m_PWM_BC_Value[6][1]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[6][1]));
            ui->labelinfo_9_2->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[6][2]);
            ui->labelinfo_9_3->setText(m_PWM_BC_Value[6][2]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[6][2]));
            ui->labelinfo_9_3->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[6][3]);
            ui->labelinfo_9_4->setText(m_PWM_BC_Value[6][3]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[6][3]));
            ui->labelinfo_9_4->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[6][4]);
            ui->labelinfo_9_5->setText(m_PWM_BC_Value[6][4]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[6][4]));
            ui->labelinfo_9_5->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[6][5]);
            ui->labelinfo_9_6->setText(m_PWM_BC_Value[6][5]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[6][5]));
            ui->labelinfo_9_6->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[6][6]);
            ui->labelinfo_9_7->setText(m_PWM_BC_Value[6][6]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[6][6]));
            ui->labelinfo_9_7->setStyleSheet(strStyleSheet);
            break;
        case 8:  // 7/7
            ui->labelinfo_10_8->setText(m_PWM_BC_Value[7][0]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[7][0]));
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[7][1]);
            ui->labelinfo_10_2->setText(m_PWM_BC_Value[7][1]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[7][1]));
            ui->labelinfo_10_2->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[7][2]);
            ui->labelinfo_10_3->setText(m_PWM_BC_Value[7][2]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[7][2]));
            ui->labelinfo_10_3->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[7][3]);
            ui->labelinfo_10_4->setText(m_PWM_BC_Value[7][3]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[7][3]));
            ui->labelinfo_10_4->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[7][4]);
            ui->labelinfo_10_5->setText(m_PWM_BC_Value[7][4]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[7][4]));
            ui->labelinfo_10_5->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[7][5]);
            ui->labelinfo_10_6->setText(m_PWM_BC_Value[7][5]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[7][5]));
            ui->labelinfo_10_6->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[7][6]);
            ui->labelinfo_10_7->setText(m_PWM_BC_Value[7][6]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[7][6]));
            ui->labelinfo_10_7->setStyleSheet(strStyleSheet);
            break;
        case 9:  // 紧急
            ui->labelinfo_11_8->setText(m_PWM_BC_Value[8][0]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[8][0]));
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[8][1]);
            ui->labelinfo_11_2->setText(m_PWM_BC_Value[8][1]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[8][1]));
            ui->labelinfo_11_2->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[8][2]);
            ui->labelinfo_11_3->setText(m_PWM_BC_Value[8][2]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[8][2]));
            ui->labelinfo_11_3->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[8][3]);
            ui->labelinfo_11_4->setText(m_PWM_BC_Value[8][3]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[8][3]));
            ui->labelinfo_11_4->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[8][4]);
            ui->labelinfo_11_5->setText(m_PWM_BC_Value[8][4]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[8][4]));
            ui->labelinfo_11_5->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(false, iFlag, m_PWM_BC_Value[8][5]);
            ui->labelinfo_11_6->setText(m_PWM_BC_Value[8][5]<0 ? strTemp : QString("%1").arg(m_PWM_BC_Value[8][5]));
            ui->labelinfo_11_6->setStyleSheet(strStyleSheet);
            strStyleSheet = getDisplayQSS(true, iFlag, m_PWM_BC_Value[8][6]);
            ui->labelinfo_11_7->setText(m_PWM_BC_Value[8][6]<0 ? strTemp: QString("%1").arg(m_PWM_BC_Value[8][6]));
            ui->labelinfo_11_7->setStyleSheet(strStyleSheet);
            break;
        default:
            break;
        }
    }
}

void brakecheck::cleanAllData(){
    QString strStyleSheet = QSS_BG_BLACK_BR_WHITE_WD_WHITE;
    ui->labelinfo_2_2->setText("");
    ui->labelinfo_2_3->setText("");
    ui->labelinfo_2_4->setText("");
    ui->labelinfo_2_5->setText("");
    ui->labelinfo_2_6->setText("");
    ui->labelinfo_2_7->setText("");

    ui->labelinfo_3_8->setText("");
    ui->labelinfo_3_2->setText("");
    ui->labelinfo_3_2->setStyleSheet(strStyleSheet);
    ui->labelinfo_3_3->setText("");
    ui->labelinfo_3_3->setStyleSheet(strStyleSheet);
    ui->labelinfo_3_4->setText("");
    ui->labelinfo_3_4->setStyleSheet(strStyleSheet);
    ui->labelinfo_3_5->setText("");
    ui->labelinfo_3_5->setStyleSheet(strStyleSheet);
    ui->labelinfo_3_6->setText("");
    ui->labelinfo_3_6->setStyleSheet(strStyleSheet);
    ui->labelinfo_3_7->setText("");
    ui->labelinfo_3_7->setStyleSheet(strStyleSheet);

    ui->labelinfo_4_8->setText("");
    ui->labelinfo_4_2->setText("");
    ui->labelinfo_4_2->setStyleSheet(strStyleSheet);
    ui->labelinfo_4_3->setText("");
    ui->labelinfo_4_3->setStyleSheet(strStyleSheet);
    ui->labelinfo_4_4->setText("");
    ui->labelinfo_4_4->setStyleSheet(strStyleSheet);
    ui->labelinfo_4_5->setText("");
    ui->labelinfo_4_5->setStyleSheet(strStyleSheet);
    ui->labelinfo_4_6->setText("");
    ui->labelinfo_4_6->setStyleSheet(strStyleSheet);
    ui->labelinfo_4_7->setText("");
    ui->labelinfo_4_7->setStyleSheet(strStyleSheet);

    ui->labelinfo_5_8->setText("");
    ui->labelinfo_5_2->setText("");
    ui->labelinfo_5_2->setStyleSheet(strStyleSheet);
    ui->labelinfo_5_3->setText("");
    ui->labelinfo_5_3->setStyleSheet(strStyleSheet);
    ui->labelinfo_5_4->setText("");
    ui->labelinfo_5_4->setStyleSheet(strStyleSheet);
    ui->labelinfo_5_5->setText("");
    ui->labelinfo_5_5->setStyleSheet(strStyleSheet);
    ui->labelinfo_5_6->setText("");
    ui->labelinfo_5_6->setStyleSheet(strStyleSheet);
    ui->labelinfo_5_7->setText("");
    ui->labelinfo_5_7->setStyleSheet(strStyleSheet);

    ui->labelinfo_6_8->setText("");
    ui->labelinfo_6_2->setText("");
    ui->labelinfo_6_2->setStyleSheet(strStyleSheet);
    ui->labelinfo_6_3->setText("");
    ui->labelinfo_6_3->setStyleSheet(strStyleSheet);
    ui->labelinfo_6_4->setText("");
    ui->labelinfo_6_4->setStyleSheet(strStyleSheet);
    ui->labelinfo_6_5->setText("");
    ui->labelinfo_6_5->setStyleSheet(strStyleSheet);
    ui->labelinfo_6_6->setText("");
    ui->labelinfo_6_6->setStyleSheet(strStyleSheet);
    ui->labelinfo_6_7->setText("");
    ui->labelinfo_6_7->setStyleSheet(strStyleSheet);

    ui->labelinfo_7_8->setText("");
    ui->labelinfo_7_2->setText("");
    ui->labelinfo_7_2->setStyleSheet(strStyleSheet);
    ui->labelinfo_7_3->setText("");
    ui->labelinfo_7_3->setStyleSheet(strStyleSheet);
    ui->labelinfo_7_4->setText("");
    ui->labelinfo_7_4->setStyleSheet(strStyleSheet);
    ui->labelinfo_7_5->setText("");
    ui->labelinfo_7_5->setStyleSheet(strStyleSheet);
    ui->labelinfo_7_6->setText("");
    ui->labelinfo_7_6->setStyleSheet(strStyleSheet);
    ui->labelinfo_7_7->setText("");
    ui->labelinfo_7_7->setStyleSheet(strStyleSheet);

    ui->labelinfo_8_8->setText("");
    ui->labelinfo_8_2->setText("");
    ui->labelinfo_8_2->setStyleSheet(strStyleSheet);
    ui->labelinfo_8_3->setText("");
    ui->labelinfo_8_3->setStyleSheet(strStyleSheet);
    ui->labelinfo_8_4->setText("");
    ui->labelinfo_8_4->setStyleSheet(strStyleSheet);
    ui->labelinfo_8_5->setText("");
    ui->labelinfo_8_5->setStyleSheet(strStyleSheet);
    ui->labelinfo_8_6->setText("");
    ui->labelinfo_8_6->setStyleSheet(strStyleSheet);
    ui->labelinfo_8_7->setText("");
    ui->labelinfo_8_7->setStyleSheet(strStyleSheet);

    ui->labelinfo_9_8->setText("");
    ui->labelinfo_9_2->setText("");
    ui->labelinfo_9_2->setStyleSheet(strStyleSheet);
    ui->labelinfo_9_3->setText("");
    ui->labelinfo_9_3->setStyleSheet(strStyleSheet);
    ui->labelinfo_9_4->setText("");
    ui->labelinfo_9_4->setStyleSheet(strStyleSheet);
    ui->labelinfo_9_5->setText("");
    ui->labelinfo_9_5->setStyleSheet(strStyleSheet);
    ui->labelinfo_9_6->setText("");
    ui->labelinfo_9_6->setStyleSheet(strStyleSheet);
    ui->labelinfo_9_7->setText("");
    ui->labelinfo_9_7->setStyleSheet(strStyleSheet);

    ui->labelinfo_10_8->setText("");
    ui->labelinfo_10_2->setText("");
    ui->labelinfo_10_2->setStyleSheet(strStyleSheet);
    ui->labelinfo_10_3->setText("");
    ui->labelinfo_10_3->setStyleSheet(strStyleSheet);
    ui->labelinfo_10_4->setText("");
    ui->labelinfo_10_4->setStyleSheet(strStyleSheet);
    ui->labelinfo_10_5->setText("");
    ui->labelinfo_10_5->setStyleSheet(strStyleSheet);
    ui->labelinfo_10_6->setText("");
    ui->labelinfo_10_6->setStyleSheet(strStyleSheet);
    ui->labelinfo_10_7->setText("");
    ui->labelinfo_10_7->setStyleSheet(strStyleSheet);

    ui->labelinfo_11_8->setText("");
    ui->labelinfo_11_2->setText("");
    ui->labelinfo_11_2->setStyleSheet(strStyleSheet);
    ui->labelinfo_11_3->setText("");
    ui->labelinfo_11_3->setStyleSheet(strStyleSheet);
    ui->labelinfo_11_4->setText("");
    ui->labelinfo_11_4->setStyleSheet(strStyleSheet);
    ui->labelinfo_11_5->setText("");
    ui->labelinfo_11_5->setStyleSheet(strStyleSheet);
    ui->labelinfo_11_6->setText("");
    ui->labelinfo_11_6->setStyleSheet(strStyleSheet);
    ui->labelinfo_11_7->setText("");
    ui->labelinfo_11_7->setStyleSheet(strStyleSheet);
}

void brakecheck::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void brakecheck::on_buttonemptycar_clicked(){
    if(1 != iCarTestFlag && !bCheckingFlag){
        ui->buttonemptycar->setStyleSheet(QSS_PBTN_SELECTED_GREEN);
        ui->buttonoverman->setStyleSheet(QSS_PBTN_NO_SELECTED);
        buttonBeep();
        if(0 == iCarTestFlag){
            ui->buttonstrart->setVisible(true);
            ui->buttonstop->setVisible(true);
            ui->labelmessage->setText("按[开始]键，进入制动试验，按[终止]键，终止正在进入的试验");
        }
        canSignalsSlots->mBCUCheckSendData[0] = 1;
        iCarTestFlag = 1;
    }
}

void brakecheck::on_buttonoverman_clicked(){
    if(2 != iCarTestFlag && !bCheckingFlag){
        ui->buttonemptycar->setStyleSheet(QSS_PBTN_NO_SELECTED);
        ui->buttonoverman->setStyleSheet(QSS_PBTN_SELECTED_GREEN);
        buttonBeep();
        if(0 == iCarTestFlag){
            ui->buttonstrart->setVisible(true);
            ui->buttonstop->setVisible(true);
            ui->labelmessage->setText("按[开始]键，进入制动试验，按[终止]键，终止正在进入的试验");
        }
        canSignalsSlots->mBCUCheckSendData[0] = 2;
        iCarTestFlag = 2;
    }
}

void brakecheck::on_buttonstrart_clicked(){
    if(!bCheckingFlag){
        for(int i = 1; i < 70; i++){
            canSignalsSlots->mBCUCheckSendData[i] = -1;
        }

        bCheckingFlag = true;
        iNowTestLevel = 1;
        ui->buttonrecord->setEnabled(true);
        cleanAllData();
        testDataValueInit();
        setTestLevel();
        setASValue();
        setPWMBCValue(1);
        setPWMBCDisplay(1);
        timerBCUTest->start(TEST_TIMEOUT);
        timerRefresh->start(REFRESH_TIMTOUT);
        buttonBeep();
    }
}

void brakecheck::on_buttonstop_clicked(){
    if(bCheckingFlag){
        bCheckingFlag = false;
        timerBCUTest->stop();
        timerRefresh->stop();
        if(iNowTestLevel >= 1 && iNowTestLevel <= 9){
            setPWMBCResultDisplay(iNowTestLevel);
        }
        ui->labelmessage->setText("按[开始]键，进入制动试验，按[终止]键，终止正在进入的试验");
        buttonBeep();
    }
}

void brakecheck::on_buttonrecord_clicked(){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 7; j++){
            canSignalsSlots->mBCUCheckSendData[i*7+j+7] = m_PWM_BC_Value[i][j];
        }
    }
    canSignalsSlots->sendBCUCheckData();
    buttonBeep();
}

void brakecheck::on_buttonquit_clicked(){
    buttonBeep();
    this->reject();
}

void brakecheck::slot_CarIcon_Update(){
    if(CAR_NO_HEADER != carHeaderFlag){
        ui->labelarrow->setPixmap(ARROW_LEFT);
    }
    else{
        ui->labelarrow->setPixmap(NO_QPIXMAP);
    }
    ui->carimagelabel1->setPixmap(MC1_CAR_BMP);
    ui->carimagelabel2->setPixmap(T_CAR_BMP);
    ui->carimagelabel5->setPixmap(T_CAR_BMP);
    ui->carimagelabel6->setPixmap(MC2_CAR_BMP);

    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        ui->carimagelabel3->setPixmap(T_CAR_BMP);
        ui->carimagelabel4->setPixmap(M_CAR_BMP);

        ui->labelinfo_1_2->setText(CAR_6_ID);
        ui->labelinfo_1_3->setText(CAR_5_ID);
        ui->labelinfo_1_4->setText(CAR_4_ID);
        ui->labelinfo_1_5->setText(CAR_3_ID);
        ui->labelinfo_1_6->setText(CAR_2_ID);
        ui->labelinfo_1_7->setText(CAR_1_ID);
    }
    else{
        ui->carimagelabel3->setPixmap(M_CAR_BMP);
        ui->carimagelabel4->setPixmap(T_CAR_BMP);

        ui->labelinfo_1_2->setText(CAR_1_ID);
        ui->labelinfo_1_3->setText(CAR_2_ID);
        ui->labelinfo_1_4->setText(CAR_3_ID);
        ui->labelinfo_1_5->setText(CAR_4_ID);
        ui->labelinfo_1_6->setText(CAR_5_ID);
        ui->labelinfo_1_7->setText(CAR_6_ID);
    }
}

void brakecheck::slot_CarCode_Update(){
    if((CAR_NO_HEADER == carHeaderFlag && CAR_6_HEADER == lastCarHeaderFlag) || CAR_6_HEADER == carHeaderFlag){
        ui->carlabel1->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[5]));
        ui->carlabel2->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[4]));
        ui->carlabel3->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[3]));
        ui->carlabel4->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[2]));
        ui->carlabel5->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[1]));
        ui->carlabel6->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[0]));
    }
    else{
        ui->carlabel1->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[0]));
        ui->carlabel2->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[1]));
        ui->carlabel3->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[2]));
        ui->carlabel4->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[3]));
        ui->carlabel5->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[4]));
        ui->carlabel6->setText(QString("%1%2").arg(CAR_TYPE).arg(carCodeNum[5]));
    }
}

void brakecheck::slot_Car_Direction_Update(){
    slot_CarIcon_Update();
    slot_CarCode_Update();
    cleanAllData();
    setASValue();
    for(int i = 0; i < 9; i++){
        setPWMBCResultDisplay(i+1);
    }
}

void brakecheck::slot_BCUTest_TimeOut(){
    b4SFlag = true;
    setPWMBCValue(iNowTestLevel);
    setPWMBCResultDisplay(iNowTestLevel);
    if(iNowTestLevel < 9){
//        setPWMBCDisplay(iNowTestLevel);
        iNowTestLevel++;
        setASValue();
        setPWMBCValue(iNowTestLevel);
        setPWMBCDisplay(iNowTestLevel);
    }
    else{
        timerBCUTest->stop();
        timerRefresh->stop();
        iNowTestLevel = 10;
    }
    setTestLevel();
    b4SFlag = false;
}

void brakecheck::slot_Refresh_TimeOut(){
    if(!b4SFlag){
        setPWMBCValue(iNowTestLevel);
        setPWMBCDisplay(iNowTestLevel);
    }
}
