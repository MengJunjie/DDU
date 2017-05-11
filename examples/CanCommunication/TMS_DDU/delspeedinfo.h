#ifndef DELSPEEDINFO_H
#define DELSPEEDINFO_H

#include <QDialog>

#include "gui_config.h"

namespace Ui {
class delspeedinfo;
}

class delspeedinfo : public QDialog
{
    Q_OBJECT
    
public:
    explicit delspeedinfo(QWidget *parent = 0);
    ~delspeedinfo();
    
    void Init();
    void setRowLabelText(int index, structSpeedCheck * speedData);
    void cleanScreenShow();
    void setScreenShow();

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void slot_1S_Timer_Out_Show();
    void on_buttondelete_clicked();
    void on_buttonpreviouspage_clicked();
    void on_buttonnextpage_clicked();
    void on_buttonquit_clicked();

    void slot_SpeedRecordData_Get_Update(int flag);

private:
    Ui::delspeedinfo *ui;
    int iShowIndex, iShowLength;
    QTimer * timer_1S;
    bool bShowTimeFlag;
    bool bGetAllFlag;
};

#endif // DELSPEEDINFO_H
