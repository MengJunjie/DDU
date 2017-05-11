#ifndef SPEEDCHECKCHANGE_H
#define SPEEDCHECKCHANGE_H

#include <QDialog>

#include "gui_config.h"
#include "speedcheck.h"

#define BACK_TIME  5000

namespace Ui {
class speedcheckchange;
}

class speedcheckchange : public QDialog
{
    Q_OBJECT
    
public:
    explicit speedcheckchange(int iFlag, speedcheck *tempSpeedCheck, QWidget *parent = 0);
    ~speedcheckchange();
    
    void Init();
    void setRowLabelText(int index, structSpeedCheck * speedData);
    void cleanScreenShow();
    void setScreenShow();

signals:
    void sig_Closed();
    void sig_PBtn_Start_Clicked();
    void sig_PBtn_Stop_Clicked();

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonstart_clicked();
    void on_buttonstop_clicked();
    void on_buttonchange_clicked();
    void on_buttonpreviouspage_clicked();
    void on_buttonnextpage_clicked();
    void on_buttonquit_clicked();

    void slot_TimerOut_5S();
    void slot_SpeedCheckData_Update();

private:
    Ui::speedcheckchange *ui;
    speedcheck *mSpeedCheck;
    int showFlag;   //1:主动显示，不返回;  2:自动切换，5s返回;
    bool bCheckingFlag;
    int iShowIndex;
    QTimer * timer5S;
};

#endif // SPEEDCHECKCHANGE_H
