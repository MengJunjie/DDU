#ifndef BRAKECHECK_H
#define BRAKECHECK_H

#include <QDialog>

#include "gui_config.h"

#define TEST_TIMEOUT 4*1000
#define REFRESH_TIMTOUT 1000

namespace Ui {
class brakecheck;
}

class brakecheck : public QDialog
{
    Q_OBJECT
    
public:
    explicit brakecheck(QWidget *parent = 0);
    ~brakecheck();
    
    void Init();
    void testDataValueInit();
    void setTestLevel();
    void setASValue();

    QString getDisplayQSS(bool bMCFlag, int iLevel, int iValue);
    void setPWMBCValue(int iFlag);
    void setPWMBCDisplay(int iFlag);
    void setPWMBCResultDisplay(int iFlag);
    void cleanAllData();

signals:

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonemptycar_clicked();
    void on_buttonoverman_clicked();
    void on_buttonstrart_clicked();
    void on_buttonstop_clicked();
    void on_buttonrecord_clicked();
    void on_buttonquit_clicked();

    void slot_CarIcon_Update();
    void slot_CarCode_Update();
    void slot_Car_Direction_Update();

    void slot_BCUTest_TimeOut();
    void slot_Refresh_TimeOut();

private:
    Ui::brakecheck *ui;
    QTimer * timerBCUTest;
    QTimer * timerRefresh;
    int iCarTestFlag;  // 0：默认，1：空车，2：超员
    bool bCheckingFlag;
    int iNowTestLevel; //0：默认，1：OFF，2～8：1/7 ～ 7/7，9：紧急, 10：测试结束
    bool b4SFlag;

    int M_MC_Empty_Lower[9];  //
    int M_MC_Empty_Upper[9];
    int T_T1_T2_Empty_Lower[9];
    int T_T1_T2_Empty_Upper[9];
    int M_MC_Full_Lower[9];
    int M_MC_Full_Upper[9];
    int T_T1_T2_Full_Lower[9];
    int T_T1_T2_Full_Upper[9];
    int m_PWM_BC_Value[9][7];

    QList<QLabel*> labelList;
};

#endif // BRAKECHECK_H
