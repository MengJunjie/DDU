#ifndef RUNINFO_H
#define RUNINFO_H

#include <QDialog>

#include "gui_config.h"

namespace Ui {
class runinfo;
}

class runinfo : public QDialog
{
    Q_OBJECT
    
public:
    explicit runinfo(QWidget *parent = 0);
    ~runinfo();
    
    void Init();

    void cleanScreenShow();
    void setScreenShow();

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonquit_clicked();

    void slot_RunRecordData_Get_Update();

private:
    Ui::runinfo *ui;
};

#endif // RUNINFO_H
