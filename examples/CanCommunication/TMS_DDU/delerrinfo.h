#ifndef DELERRINFO_H
#define DELERRINFO_H

#include <QDialog>

#include "gui_config.h"

namespace Ui {
class delerrinfo;
}

class delerrinfo : public QDialog
{
    Q_OBJECT
    
public:
    explicit delerrinfo(QWidget *parent = 0);
    ~delerrinfo();
    
    void Init();
    void setFalutRecordCount(CAR_ID carId, int iCount);
    void setVVVF1FalutCount(CAR_ID carId, int iCount);
    void setVVVF2FalutCount(CAR_ID carId, int iCount);
    void setSIVFalutCount(CAR_ID carId, int iCount);
    void setBrakeFalutCount(CAR_ID carId, int iCount);
    void allPBtnFocusClear();

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_btnerrorrecord_clicked();
    void on_btnvvvf1_clicked();
    void on_btnvvvf2_clicked();
    void on_btnsiv_clicked();
    void on_btnbrake_clicked();
    void on_buttondelete_clicked();
    void on_buttonquit_clicked();

    void slot_CarIcon_Update();
    void slot_CarCode_Update();
    void slot_Car_Direction_Update();

    void slot_FaultRecordCount_Get_Update();

private:
    Ui::delerrinfo *ui;
    int iDelItemIndex;
};

#endif // DELERRINFO_H
