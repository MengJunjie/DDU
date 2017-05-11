#ifndef SIVCHECK_H
#define SIVCHECK_H

#include <QDialog>

#include "gui_config.h"

#define SIV_CHECK_TIMEOUT 1000

enum SIVCHECK_ITEM{
    SIVCHECK_VOLTAGE,
    SIVCHECK_FREQUENCY,
    SIVCHECK_JUDGE
};

enum SIVCHECK_RESULT{
    SIV_RESULT_STANDBY,
    SIV_RESULT_JUDGING,
    SIV_RESULT_OK,
    SIV_RESULT_NG
};

namespace Ui {
class sivcheck;
}

class sivcheck : public QDialog
{
    Q_OBJECT
    
public:
    explicit sivcheck(QWidget *parent = 0);
    ~sivcheck();
    void Init();
    void setVoltage();
    void setCurrent();

    void setSIVOutVoltage(CAR_ID carId, SIV_STATE iState, int iValue=0);
    void setSIVOutFreq(CAR_ID carId, SIV_STATE iState, int iValue=0);
    void setSIVCheckResult(CAR_ID carId, SIVCHECK_RESULT iRestlt);

signals:
    void sig_SIV_Check_Start();

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonstrart_clicked();
    void on_buttonstop_clicked();
    void on_buttonrecord_clicked();
    void on_buttonquit_clicked();

    void slot_CarIcon_Update();
    void slot_CarCode_Update();
    void slot_Car_Direction_Update();
    void slot_SIV_Voltage_Freq_Update(CAR_ID carId, int iState, int iValue1, int iValue2);

    void slot_Car2SIV_TimeOut();
    void slot_Car4SIV_TimeOut();

private:
    Ui::sivcheck *ui;
    QTimer * timerCar2SIV;
    QTimer * timerCar4SIV;
    bool bCheckStartFlag, bCar2SIVCheckFlag, bCar4SIVCheckFlag, bCar2SIVValidFlag, bCar4SIVValidFlag;
    int iCar2SIVCheckTime, iCar4SIVCheckTime, iCar2SIVCheckResult, iCar4SIVCheckResult;
    int iCar2SIVData[3], iCar4SIVData[3];
};

#endif // SIVCHECK_H
