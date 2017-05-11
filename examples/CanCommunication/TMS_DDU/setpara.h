#ifndef SETPARA_H
#define SETPARA_H

#include <QDialog>
#include <QLabel>

#include "gui_config.h"

enum SETPARA_ITEM{
    PARA_CAR_ID,
    PARA_WHEEL_MM,
    PARA_AS_FULL,
    PARA_AS_EMPTY
};

namespace Ui {
class setpara;
}

class setpara : public QDialog
{
    Q_OBJECT
    
public:
    explicit setpara(QWidget *parent = 0);
    ~setpara();
    
    void Init();
    void allDataInit();
    void setValue(SETPARA_ITEM item, CAR_ID carId, int iValue);


private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonchange_clicked();
    void on_buttonquit_clicked();

    void slot_labelinfo_clicked();
    void slot_setValue(int iValue);

    void slot_CarIcon_Update();
    void slot_CarCode_Update();
    void slot_Car_Direction_Update();
    void slot_Car_ParaData_Update(int dataFalg);

private:
    Ui::setpara *ui;
    QString    m_info;

    QLabel * labelCurrent;
};

#endif // SETPARA_H
