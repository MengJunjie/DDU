#ifndef DOORCHECK_H
#define DOORCHECK_H

#include <QDialog>

#include "gui_config.h"

#define DOORCHECK_TIMEOUT  7000

enum DOORCHECK_ITEM{
    DOORCHECK_NONE,
    DOORCHECK_CMD_RIGHT,
    DOORCHECK_STATE_RIGHT,
    DOORCHECK_OPENED_RIGHT,
    DOORCHECK_CLOSED_RIGHT,
    DOORCHECK_CMD_LEFT,
    DOORCHECK_STATE_LEFT,
    DOORCHECK_OPENED_LEFT,
    DOORCHECK_CLOSED_LEFT
};

enum DOORCHECK_CMD_TYPE{
    CMD_TYPE_0,
    CMD_OPEN_DOOR,
    CMD_CLOSE_DOOR
};

enum DOORCHECK_RESULT_TYPE{
    DOOR_RESULT_STANDBY,
    DOOR_RESULT_JUDGING,
    DOOR_RESULT_OK,
    DOOR_RESULT_NG
};

namespace Ui {
class doorcheck;
}

class doorcheck : public QDialog
{
    Q_OBJECT
    
public:
    explicit doorcheck(QWidget *parent = 0);
    ~doorcheck();
    
    void Init();
    void setDoorName();
    void setDoorCheckCMD(DOORCHECK_ITEM item, DOORCHECK_CMD_TYPE cmdType);
    void setDoorCheckState(DOORCHECK_ITEM item, CAR_ID carId, DOOR_ID doorId, RUN_DOOR_STATE stateType);
    void setDoorCheckResult(DOORCHECK_ITEM item, CAR_ID carId, DOORCHECK_RESULT_TYPE type);
    void labelListInit();

signals:

private slots:
    void slot_1S_Timer_Out(QString strDateTime);
    void on_buttonrecord_clicked();
    void on_buttonquit_clicked();

    void slot_CarIcon_Update();
    void slot_CarCode_Update();
    void slot_Car_Direction_Update();
    void slot_Door_State_Update(CAR_ID carId, DOOR_ID doorId,RUN_DOOR_STATE doorState);
    void slot_Car_DoorOperation_Update(int doorFlag, int operation);   //doorFlag 0:默认;1:左侧门;2:右侧门;    operation 0:默认;1:门开;2:门关

    void slot_TimerLeft_Out();
    void slot_TimerRight_Out();

private:
    Ui::doorcheck *ui;

    QList<QLabel*> leftLabelList;
    QList<QLabel*> rightLabelList;

    QTimer * timerLeft;
    QTimer * timerRight;

    int iLeftOperationFlag, iRightOperationFlag;
    bool bLeftJudgeFlag, bLeftTimerOutFlag, bRightJudgeFlag, bRightTimerOutFlag;
    int iRightOpenResult[6];
    int iRightCloseResult[6];
    int iLeftOpenResult[6];
    int iLeftCloseResult[6];
};

#endif // DOORCHECK_H
