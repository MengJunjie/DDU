#ifndef SETASPARA_H
#define SETASPARA_H

#include <QDialog>

#include "gui_config.h"

namespace Ui {
class setaspara;
}

class setaspara : public QDialog
{
    Q_OBJECT
    
public:
    explicit setaspara(int itemId,  QString strCarId, QString strCarCode, QString strValue, QWidget *parent = 0);
    ~setaspara();
    
    void Init();

signals:
    void sig_setValue(int iValue);

private slots:
    void slot_1S_Timer_Out(QString strDateTime);

    void on_buttonrevise_clicked();
    void on_buttonset_clicked();
    void on_buttonquit_clicked();

    void slot_pushButton_Num_clicked();

private:
    Ui::setaspara *ui;

    bool bReviseFlag;
    int itemId, carId, intValueLength;
};

#endif // SETASPARA_H
