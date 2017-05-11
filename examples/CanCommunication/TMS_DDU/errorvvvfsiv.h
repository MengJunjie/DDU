#ifndef ERRORVVVFSIV_H
#define ERRORVVVFSIV_H

#include <QDialog>

#include "gui_config.h"

enum BREAKDOWN_TYPE{
    BD_VVVF1,
    BD_VVVF2,
    BD_SIV
};

namespace Ui {
class errorvvvfsiv;
}

class errorvvvfsiv : public QDialog
{
    Q_OBJECT

public:
    explicit errorvvvfsiv(BREAKDOWN_TYPE type, int iCarId, QString strCarId, QWidget *parent = 0);
    ~errorvvvfsiv();

    void Init();
    void setErrorInfo(int iIndex, QString strDate, QString strTime, QString strCode);

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonpreviouspage_clicked();
    void on_buttonnextpage_clicked();
    void on_buttonquit_clicked();

    void slot_FaultRecordData_Get_Update();
private:
    Ui::errorvvvfsiv *ui;
    BREAKDOWN_TYPE iErrType;
    int iErrCarId;
    int iShowErrIndex, iErrListLength;
};

#endif // ERRORVVVFSIV_H
