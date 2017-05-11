#ifndef VIEWERRLIST2_H
#define VIEWERRLIST2_H

#include <QDialog>

#include "gui_config.h"

namespace Ui {
class viewerrlist2;
}

class viewerrlist2 : public QDialog
{
    Q_OBJECT
    
public:
    explicit viewerrlist2(int iCarId, QString strCarId, QWidget *parent = 0);
    ~viewerrlist2();

    void  Init();
    void setErrorInfo(int iIndex, QString strDate, QString strTime, QString strCode);

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonpreviouspage_clicked();
    void on_buttonnextpage_clicked();
    void on_buttonquit_clicked();

    void slot_FaultRecordData_Get_Update();

private:
    Ui::viewerrlist2 *ui;
    int iErrCarId;
    int iShowErrIndex, iErrListLength;
};

#endif // VIEWERRLIST2_H
