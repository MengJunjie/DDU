#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTimer>

#include "mainmenu.h"
#include "airconditioninfo.h"
#include "dialog/maincarinfo.h"
#include "errinfo.h"
#include "keepmove.h"
#include "ptudownload.h"

#include "gui_config.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

signals:
    void sig_SingleShot_200ms();

public slots:
    void slot_LowPBtn_Clicked(int btnId);
    void slot_Error_Happened();
    void slot_ErrorScreen_OKBtn_Clicked();
    void slot_ACC_BackPBtn_Clicked();
    void slot_Speed_Door_State_Update(int iState);
    void slot_PTUDownloadState_Update(bool bFlag);

private:
    Ui::MainWidget *ui;
    mainmenu * m_mainmenu;
    airconditioninfo * m_airconditioninfo;
    MainCarInfo * m_MainCarInfo;
    errinfo * m_errinfo;
    keepmove * m_keepmove;
    PTUDownload * m_PTUDownload;

    bool bCarOrRunScreenFlag, bErrorScreenFlag;
    int intLastWidgetIndex;
};

#endif // MAINWIDGET_H
