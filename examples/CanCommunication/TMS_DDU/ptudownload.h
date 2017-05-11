#ifndef PTUDOWNLOAD_H
#define PTUDOWNLOAD_H

#include <QDialog>

namespace Ui {
class PTUDownload;
}

class PTUDownload : public QDialog
{
    Q_OBJECT

public:
    explicit PTUDownload(QWidget *parent = 0);
    ~PTUDownload();

private:
    Ui::PTUDownload *ui;
};

#endif // PTUDOWNLOAD_H
