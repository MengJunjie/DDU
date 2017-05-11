#ifndef SETDATETIME_H
#define SETDATETIME_H

#include <QDialog>
#include <QLabel>
#include "gui_config.h"

enum DATETIME_SET_FLAG{
    FLAG_NULL,
    FLAG_SELECT,
    FLAG_CLEAR,
    FLAG_REVISING,
    FLAG_FINISHED
};

namespace Ui {
class setdatetime;
}

class setdatetime : public QDialog
{
    Q_OBJECT
    
public:
    explicit setdatetime(QWidget *parent = 0);
    ~setdatetime();
    
    void Init();

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonrevise_clicked();
    void on_buttonstrart_clicked();
    void on_buttonquit_clicked();

    void slot_label_clicked();
    void slot_pushButton_Num_clicked();

private:
    Ui::setdatetime *ui;

    DATETIME_SET_FLAG setFlag;
    bool bDateTimeEditFlag;
    QLabel * labelCurrent;
};

#endif // SETDATETIME_H
