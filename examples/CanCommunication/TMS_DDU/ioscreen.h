#ifndef IOSCREEN_H
#define IOSCREEN_H

#include <QDialog>
#include "mylabel.h"
#include "gui_config.h"

namespace Ui {
class IOScreen;
}

class IOScreen : public QDialog
{
    Q_OBJECT

public:
    explicit IOScreen(QWidget *parent = 0);
    ~IOScreen();

    void Init();
    void clearBtnFocus();
    void setLabelHeader();
    void setLabelList1();
    void setLabelList2();
    void setLabelList3();
    void setLabelPWM();
    void setPNPBtnState();

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonMC1_clicked();
    void on_buttonT_clicked();
    void on_buttonM_clicked();
    void on_buttonT1_clicked();
    void on_buttonT2_clicked();
    void on_buttonMC2_clicked();
    void on_buttonprevious_clicked();
    void on_buttonnext_clicked();
    void on_buttonquit_clicked();

    void slot_Car_DIDoorOperation_Update(CAR_ID carId);
    void slot_TimerRefresh_TimeOut();

private:
    Ui::IOScreen *ui;

    int mCurCarShow;
    int mMC1PageIndex;
    int mMC2PageIndex;
    int mCurShowIndex;
    int mCardNum;

    bool bMultiPageFlag;

    QList<QLabel*> mLabelList1;
    QList<QLabel*> mLabelList2;
    QList<MyLabel*> mLabelList3;

    QStringList mDINameList[10];
    QStringList mDIValueList[16];

    QTimer * timerRefresh;
};

#endif // IOSCREEN_H
