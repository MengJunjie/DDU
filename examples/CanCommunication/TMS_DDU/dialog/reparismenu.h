#ifndef REPARISMENU_H
#define REPARISMENU_H

#include <QDialog>
#include <QTimer>

#include "gui_config.h"

#define BLINK_TIMEOUT 500

enum REPARISMENU_ITEM{
    REPARI_NULL,
    REPARI_DOORCHECK,
    REPARI_AIRPUMPCHECK,
    REPARI_SIVCHECK,
    REPARI_BRAKECHECK,
    REPARI_SETDATETIME,
    REPARI_SETPARA,
    REPARI_ERRORINFO,
    REPARI_DEL_ERRORINFO,
    REPARI_RUNINFO,
    REPARI_DEL_RUNINFO,
    REPARI_SPEEDINFO,
    REPARI_DEL_SPEEDINFO,
    REPARI_LINKER_GET,
    REPARI_SETPASSWORD,
    REPARI_IOSCREEN
};

namespace Ui {
class reparismenu;
}

class reparismenu : public QDialog
{
    Q_OBJECT
    
public:
    explicit reparismenu(QWidget *parent = 0);
    ~reparismenu();

    void Init();
    void allClickedFlagInit();
    void clearBtnFocus();
    void stopBlinkTimer();

signals:
    void sig_buttonmenu_clicked();

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttondoorcheck_clicked();
    void on_buttonaircheck_clicked();
    void on_buttonSIVcheck_clicked();
    void on_buttonbraketest_clicked();
    void on_buttontimeset_clicked();
    void on_buttonparameterset_clicked();
    void on_buttonerrorlog_clicked();
    void on_buttondeleteerrorlog_clicked();
    void on_buttonrunlog_clicked();
    void on_buttondeleterunlog_clicked();
    void on_buttonspeedtest_clicked();
    void on_buttondeletespeedtest_clicked();
    void on_buttonlinkerreadout_clicked();
    void on_buttonpasswordset_clicked();
    void on_buttonmenu_clicked();
    void on_buttonsure_clicked();
    void on_buttonIOScreen_clicked();

    void on_buttontempleft_clicked();
    void on_buttontempright_clicked();

    void on_button_temp3_clicked();
    void on_button_temp4_clicked();
    void on_button_temp5_clicked();

    void slot_OtherTempPBtn_Clicked();

    void slot_Blink_TimeOut();
    void slot_3STimeOut();

private:
    Ui::reparismenu *ui;
    QTimer * m_BlinkTimer;
//    QTimer * m_3STimer;

    bool bPBtnTempLeftClicked;
    bool bClickedFlag[2];

    REPARISMENU_ITEM curOperation;

    QList<QPushButton*> pBtnList;
};

#endif // REPARISMENU_H
