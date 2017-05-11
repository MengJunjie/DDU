#ifndef SETDIRECTION_H
#define SETDIRECTION_H

#include <QDialog>

#include "gui_config.h"

namespace Ui {
class setdirection;
}

class setdirection : public QDialog
{
    Q_OBJECT
    
public:
    explicit setdirection(QWidget *parent = 0);
    ~setdirection();
    
    void Init();

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonsure_clicked();

    void slot_CarIcon_Update();
    void slot_CarCode_Update();
    void slot_Car_Direction_Update();

private:
    Ui::setdirection *ui;
};

#endif // SETDIRECTION_H
