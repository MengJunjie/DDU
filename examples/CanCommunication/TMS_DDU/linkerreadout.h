#ifndef LINKERREADOUT_H
#define LINKERREADOUT_H

#include <QDialog>

#include "gui_config.h"

namespace Ui {
class linkerreadout;
}

class linkerreadout : public QDialog
{
    Q_OBJECT

public:
    explicit linkerreadout(QWidget *parent = 0);
    ~linkerreadout();

    void Init();

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonlink_clicked();
    void on_buttonbreak_clicked();
    void on_buttonquit_clicked();

private:
    Ui::linkerreadout *ui;
};

#endif // LINKERREADOUT_H
