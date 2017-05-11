#ifndef ERRORRECORD_H
#define ERRORRECORD_H

#include <QDialog>

#include "gui_config.h"

namespace Ui {
class errorrecord;
}

class errorrecord : public QDialog
{
    Q_OBJECT

public:
    explicit errorrecord(int iCarId, QString carName, int errCount, QWidget *parent = 0);
    ~errorrecord();

    void Init();

    void setRowLabelText(int index, structFaultRecordData *mFaultData);
    void cleanScreenShow();
    void setScreenShow();

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonpreviouspage_clicked();
    void on_buttonnextpage_clicked();
    void on_buttonquit_clicked();

    void slot_FaultRecordData_Get_Update();

private:
    Ui::errorrecord *ui;
    int iErrCarId;
    QString strCarName;
    int iShowIndex, iShowLength;
    int iErrCount;
};

#endif // ERRORRECORD_H
