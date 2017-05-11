#include "ptudownload.h"
#include "ui_ptudownload.h"

PTUDownload::PTUDownload(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PTUDownload)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);
}

PTUDownload::~PTUDownload()
{
    delete ui;
}
