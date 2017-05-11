#ifndef SETSTATION_H
#define SETSTATION_H

#include <QDialog>
#include <QList>

#include "gui_config.h"

namespace Ui {
class setstation;
}

class setstation : public QDialog
{
    Q_OBJECT

public:
    explicit setstation(QWidget *parent = 0);
    ~setstation();

    void Init();
    void setTitleName(int flag = 0);
    void setStationId(int stationId);
    void setStationName();

signals:
    void sig_setStationId(int iFlag, int stationId);

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonback_clicked();
    void on_buttonok_clicked();

    void slot_StationBtn_Clicked();

private:
    Ui::setstation *ui;
    QList<QPushButton*> pBtnList;
    QString strStationName;
    int iStationCount, iStationFlag, iStationId;
};

#endif // SETSTATION_H
