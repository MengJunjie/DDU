#ifndef PASSWORD_H
#define PASSWORD_H

#include <QDialog>

#include "gui_config.h"

enum NEXT_SCREEN_FLAG{
    IS_NULL,
    SET_DATETIME,
    SET_PARA,
    SET_PASSWORD,
    DEL_ERRINFO,
    DEL_RUNINFO,
    DEL_SPEEDINFO,
    LINKER_READOUT,
    IO_SCREEN
};

namespace Ui {
class password;
}

class password : public QDialog
{
    Q_OBJECT
    
public:
    explicit password(NEXT_SCREEN_FLAG flag, QWidget *parent = 0);
    ~password();

    void Init();

signals:

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonback_clicked();
    void on_buttonsure_clicked();
    void on_buttonrevise_clicked();

    void slot_pushButton_Num_clicked();

private:
    Ui::password *ui;
    NEXT_SCREEN_FLAG nextFlag;

    QString strPassword, strInputPassword;
};

#endif // PASSWORD_H
