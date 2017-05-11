#ifndef ERRDETAIL_H
#define ERRDETAIL_H

#include <QDialog>

#include "gui_config.h"

namespace Ui {
class errdetail;
}

class errdetail : public QDialog
{
    Q_OBJECT

public:
    explicit errdetail(QWidget *parent = 0);
    ~errdetail();

    void Init();

    void setCarId(int carId);
    void setErrName(QString strName);
    void setDoorId(int doorId);
    void setErrCode(int iFlag, int iCode);

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonok_clicked();

private:
    Ui::errdetail *ui;
    int iErrorFlag;
};

#endif // ERRDETAIL_H
