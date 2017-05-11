#ifndef INITSETSURE_H
#define INITSETSURE_H

#include <QDialog>

#include "gui_config.h"

namespace Ui {
class initsetsure;
}

class initsetsure : public QDialog
{
    Q_OBJECT

public:
    explicit initsetsure(QWidget *parent = 0);
    ~initsetsure();

    void Init();
    void setTypeCarIdTableId(QString strtype, QString strtableId, QString strcarId);
    void setCurrentStation(QString strstation);
    void setEndStation(QString strstation);

signals:
    void sig_SetSure_Sure_Clicked();
    void sig_ToMainMenu();

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttoncurstation_clicked();
    void on_buttonendstation_clicked();
    void on_buttonruntable_clicked();
    void on_buttondirectionsure_clicked();
    void on_buttontmainmenu_clicked();
    void on_buttontback_clicked();
    void on_buttonsure_clicked();

    void slot_SetStationId(int flag, int stationId);
private:
    Ui::initsetsure *ui;
    int iCurrStationId, iEndStationId;
};

#endif // INITSETSURE_H
