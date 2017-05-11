#ifndef SETPASSWORD_H
#define SETPASSWORD_H

#include <QDialog>
#include <QLabel>

#include "gui_config.h"

namespace Ui {
class setpassword;
}

class setpassword : public QDialog
{
    Q_OBJECT
    
public:
    explicit setpassword(QWidget *parent = 0);
    ~setpassword();
    
    void Init();

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonrevise_clicked();
    void on_buttonset_clicked();
    void on_buttonquit_clicked();

    void slot_myLabel_clicked();
    void slot_pushButton_Num_clicked();
private:
    Ui::setpassword *ui;
    QLabel * labelCurrent;
    QString strPassword, strOldPassword, strNewPassword, strSurePassword;
};

#endif // SETPASSWORD_H
