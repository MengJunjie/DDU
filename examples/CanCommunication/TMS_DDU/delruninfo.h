#ifndef DELRUNINFO_H
#define DELRUNINFO_H

#include <QDialog>

#include "gui_config.h"

namespace Ui {
class delruninfo;
}

class delruninfo : public QDialog
{
    Q_OBJECT
    
public:
    explicit delruninfo(QWidget *parent = 0);
    ~delruninfo();
    
    void  Init();
    void cleanScreenShow();
    void setScreenShow();
    void allPBtnFocusClear();

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_btnrundata_clicked();
    void on_btnworkdata_clicked();
    void on_btnridingrate_clicked();
    void on_btntraveldistance_clicked();
    void on_btndragelec_clicked();
    void on_btnreviveelec_clicked();
    void on_btnairpump_clicked();
    void on_buttondelete_clicked();
    void on_buttonquit_clicked();

    void slot_RunRecordData_Get_Update();

private:
    Ui::delruninfo *ui;
    int iDelItemIndex;
};

#endif // DELRUNINFO_H
