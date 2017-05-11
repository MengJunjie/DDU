#ifndef CARINFOINIT_H
#define CARINFOINIT_H

#include <QDialog>
#include <QLabel>

#include "gui_config.h"

namespace Ui {
class carinfoinit;
}

class carinfoinit : public QDialog
{
    Q_OBJECT
    
public:
    explicit carinfoinit(QWidget *parent = 0);
    ~carinfoinit();
    
    void Init();

signals:
    void sig_LowPBtn_Clicked(int btnId);

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttondays_clicked();
    void on_buttonweekdays_clicked();
    void on_buttontableId_clicked();
    void on_buttoncarId_clicked();
    void on_buttonrevise_clicked();
    void on_buttoninput_clicked();
    void on_buttonmainmenu_clicked();
    void on_buttonsure_clicked();

    void slot_myLabel_clicked();
    void slot_pushButton_Num_clicked();
    void slot_SetSureScreen_SurePBtn_Clicked();

private:
    Ui::carinfoinit *ui;
    QLabel * labelCurrent;
    QString strRunType;
};

#endif // CARINFOINIT_H
