#ifndef VIEWERRLIST1_H
#define VIEWERRLIST1_H

#include <QDialog>
#include <QLabel>

#include "gui_config.h"

namespace Ui {
class viewerrlist1;
}

class viewerrlist1 : public QDialog
{
    Q_OBJECT
    
public:
    explicit viewerrlist1(QWidget *parent = 0);
    ~viewerrlist1();
    
    void Init();
    void setFalutRecordCount(CAR_ID carId, int iCount);
    void setVVVF1FalutCount(CAR_ID carId, int iCount);
    void setVVVF2FalutCount(CAR_ID carId, int iCount);
    void setSIVFalutCount(CAR_ID carId, int iCount);
    void setBrakeFalutCount(CAR_ID carId, int iCount);

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonshow_clicked();
    void on_buttonquit_clicked();

    void slot_labelinfo_clicked();

    void slot_CarIcon_Update();
    void slot_CarCode_Update();
    void slot_Car_Direction_Update();

    void slot_FaultRecordCount_Get_Update();

private:
    Ui::viewerrlist1 *ui;

    QLabel * labelCurrent;
};

#endif // VIEWERRLIST1_H
