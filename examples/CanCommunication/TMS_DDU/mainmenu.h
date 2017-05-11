#ifndef MAINMENU_H
#define MAINMENU_H

#include <QDialog>

#include "gui_config.h"

namespace Ui {
class mainmenu;
}

class mainmenu : public QDialog
{
    Q_OBJECT
    
public:
    explicit mainmenu(QWidget *parent = 0);
    ~mainmenu();
    void Init();

signals:
    void sig_LowPBtn_Clicked(int btnId);

public slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonaircondition_clicked();
    void on_buttoncarinfo_clicked();
    void on_buttonerr_clicked();
    void on_buttonrun_clicked();
    void on_buttonrepairmenu_clicked();
    void on_buttoninit_clicked();

    void slot_CarIcon_Update();
    void slot_CarCode_Update();
    void slot_Car_Direction_Update();
    void slot_ATOModel_Update(int model);

    void slot_HB_OFF_Update(bool bState);

private:
    Ui::mainmenu *ui;
};

#endif // MAINMENU_H
