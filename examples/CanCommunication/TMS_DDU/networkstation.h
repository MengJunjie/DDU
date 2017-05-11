#ifndef NETWORKSTATION_H
#define NETWORKSTATION_H

#include <QDialog>
#include <QList>
#include <QLabel>

#include "gui_config.h"

namespace Ui {
class networkstation;
}

class networkstation : public QDialog
{
    Q_OBJECT

public:
    explicit networkstation(QWidget *parent = 0);
    ~networkstation();
    void Init();

public slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonback_clicked();

    void slot_SetCanState(int canId, CAN_STATE canState);
    void slot_SetCanWorking();
    void slot_SetCCUWorking(int ccuId);
    void slot_SetR1R2ComnErr(int rrId);

private:
    Ui::networkstation *ui;

    QList<QLabel*> can1DevNodeList;
    QList<QLabel*> can2DevNodeList;
};

#endif // NETWORKSTATION_H
