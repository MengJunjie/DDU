#ifndef RUNTABLE_H
#define RUNTABLE_H

#include <QDialog>

#include "gui_config.h"

namespace Ui {
class runtable;
}

class runtable : public QDialog
{
    Q_OBJECT

public:
    explicit runtable(QWidget *parent = 0);
    ~runtable();

    void Init();
    void setCarTableId(QString strtableId, QString strcarId);

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonquit_clicked();

private:
    Ui::runtable *ui;
};

#endif // RUNTABLE_H
