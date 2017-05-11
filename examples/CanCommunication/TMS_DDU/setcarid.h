#ifndef SETCARID_H
#define SETCARID_H

#include <QDialog>
#include <QSettings>

#include "gui_config.h"

namespace Ui {
class setCarId;
}

class setCarId : public QDialog
{
    Q_OBJECT

public:
    explicit setCarId(QWidget *parent = 0);
    ~setCarId();

    void Init();
    void setDevNodeId(int nodeId);

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_comboBox_currentIndexChanged(int index);
    void on_buttoncalibrate_clicked();
    void on_buttonset_clicked();
    void on_buttonquit_clicked();

private:
    Ui::setCarId *ui;

    int curCarId;
};

#endif // SETCARID_H
