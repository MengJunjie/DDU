#include "errdetail.h"
#include "ui_errdetail.h"
#include "../cansignalsslots.h"
extern CanSignalsSlots * canSignalsSlots;

errdetail::errdetail(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::errdetail)
{
    ui->setupUi(this);
    iErrorFlag = 0;
    Init();

    connect(canSignalsSlots, SIGNAL(sig_1S_Timer_Out(QString)), this, SLOT(slot_1S_Timer_Out(QString)));
}

errdetail::~errdetail()
{
    delete ui;
}

void errdetail::Init(){
    setWindowFlags(Qt::FramelessWindowHint);

    ui->title_3->setText(QDateTime::currentDateTime().toString("yy年MM月dd日 hh:mm:ss"));
}

void errdetail::setCarId(int carId){
    ui->labeldetail_1_1->setText(QString("车号 %1").arg(carId));
}

void errdetail::setErrName(QString strName){
    ui->labeldetail_1_2->setText(strName);
}

void errdetail::setDoorId(int doorId){
    ui->labeldetail_1_3->setText(QString("%1/4").arg(doorId));
}

void errdetail::setErrCode(int iFlag, int iCode){
    QString strText1 = "", strText2 = "", strText3 = "";
    QString strVVVFText = "VVVF 发生保护动作。请进行回零动作。";
    QString strSIVText = "（第一次） \nSIV发生保护动作。 请稍等，SIV自动再起动。\n\n（累次重故障发生）\nSIV发生保护动作。 请进行复位操作。\n\n（复位后120秒以内发生故障）\nSIV发生保护动作。\n不能进行复位操作。\n请确认是否在扩展供电后，维持使用。";
    QString strACC_Text1 = "检查热继电器";
    QString strACC_Text2 = "是否保护。";
    QString strDoor_Text1 = "手动关门后将门隔离。\n\n";
    QString strDoor_Text2 = "如为自动防夹，手动关门后将门隔离。\n\n";
    QString strDoor_Text3 = "不影响车门基本开关门功能，可在车辆回库后处理。\n\n";
    QString strDoor_Text4 = "门运动停止并处于释放状态。如满足开门或关门条件，则门尝试再次运动。\n应检查此门或隔离此门。";
    QString strDoor_Text5 = "门将立即关闭，不响应开门指令。应隔离此门。";
    QString strDoor_Text6 = "门尝试运动。应隔离此门。";
    QString strDoor_Text7 = "关闭该输出口输出功能，不影响门运动。";
    switch (iFlag) {
    case 1:   // VVVF
        switch (iCode) {
        case 0x01:
            strText1 = "HBT";
            strText2 = "HB 脱扣";
            strText3 = "（第一次）HB 因过流闸。停车后，请按复位按钮1 秒以上。\n\n（第四次）HB 因过流闸。发生故障的VVVF 切除了，不能进行复位操作。\n\n〈故障等级为C 级时〉发生故障的VVVF 切除了。运行到下一车站后，请停止车辆运营。\n\n〈故障等级为D 级时〉发生故障的VVVF 切除了，运营到终点站后，请停止车辆运营。";
            break;
        case 0x02:
            strText1 = "FAIL";
            strText2 = "控制故障";
            strText3 = "（第一次）VVVF 发生保护动作。停车后，请按复位按钮1 秒以上。\n\n（第四次）发生故障的VVVF 切除了。不能进行复位操作。\n\n〈故障等级为C 级时〉发生故障的VVVF 切除了，运行到下一车站后，停止车辆运营。\n\n〈故障等级伪D 级时〉发生故障的VVVF 切除了，运营到终点站后，停止车辆运营。";
            break;
        case 0x03:
            strText1 = "MMOCD";
            strText2 = "牵引电机过电流";   //"电机过电流";
            strText3 = strVVVFText;
            break;
        case 0x04:
            strText1 = "FOVD";
            strText2 = "FC 过电压1";   //"滤波电容器过电压";
            strText3 = strVVVFText;
            break;
        case 0x05:
            strText1 = "FLVD";
            strText2 = "FC 低电压";//"滤波电容器低电压";
            strText3 = strVVVFText;
            break;
        case 0x06:
            strText1 = "SOVD";
            strText2 = "FC 过电压2";
            strText3 = "";
            break;
        case 0x07:
            strText1 = "PGD";
            strText2 = "后退检测";
            strText3 = strVVVFText;
            break;
        case 0x08:
            strText1 = "BSLP";
            strText2 = "大空转";
            strText3 = strVVVFText;
            break;
        case 0x09:
            strText1 = "BSLD";
            strText2 = "大滑行";
            strText3 = strVVVFText;
            break;
        case 0x0A:
            strText1 = "THD1";
            strText2 = "冷却器升温";
            strText3 = "由于温度上升，1 组和2 组一共再生回路被切除。温度下降后可自动恢复。";
            break;
        case 0x0B:
            strText1 = "LGD";
            strText2 = "架空线接地";
            strText3 = strVVVFText;
            break;
        case 0x0C:
            strText1 = "FMEF";
            strText2 = "PG 故障";
            strText3 = "（推定速度异常时）\nVVVF 发生保护动作。请进行回零动作。\n\n\n（PG 故障时）\nPG 发生故障。自动切换。";
            break;
        case 0x0D:
            strText1 = "BCHF";
            strText2 = "BCH 元件过热";
            strText3 = "";
            break;
        case 0x0E:
            strText1 = "T_PWMF";
            strText2 = "指令异常";
            strText3 = "";
            break;
        case 0x0F:
            strText1 = "TEST";
            strText2 = "模拟故障";
            strText3 = "";
            break;
        default:
            strText1 = "";
            strText2 = "";
            strText3 = "";
            break;
        }
        break;
    case 2:   // SIV
        switch (iCode) {
        case 0x01:
            strText1 = "";
            strText2 = "";
            strText3 = "";
            break;
        case 0x02:
            strText1 = "";
            strText2 = "";
            strText3 = "";
            break;
        case 0x03:
            strText1 = "PWF";
            strText2 = "控制电源异常";
            strText3 = strSIVText;
            break;
        case 0x04:
            strText1 = "FRQF";
            strText2 = "频率异常";
            strText3 = strSIVText;
            break;
        case 0x05:
            strText1 = "CTF";
            strText2 = "CT故障";
            strText3 = strSIVText;
            break;
        case 0x06:
            strText1 = "WD_C";
            strText2 = "CPU异常";
            strText3 = "（第一次）\nSIV发生保护动作。 请稍等，自动再起动。\n\n（第二次）\nSIV发生保护动作。\n再一次接通SIV控制电源NFB（QF43）后,进行复位操作。\n\n（复位后120秒以内发生故障） \nSIV发生保护动作。\n不能进行复位操作。\n请确认是否在扩展供电后，维持使用。";
            break;
        case 0x07:
            strText1 = "MKF";
            strText2 = "继电器故障";
            strText3 = strSIVText;
            break;
        case 0x08:
            strText1 = "THR";
            strText2 = "IGBT冷却器加热";
            strText3 = strSIVText;
            break;
        case 0x11:
            strText1 = "DIF_S";
            strText2 = "检测到门极信号不一致";
            strText3 = strSIVText;
            break;
        case 0x12:
            strText1 = "FCOV";
            strText2 = "滤波电容器过电压";
            strText3 = strSIVText;
            break;
        case 0x13:
            strText1 = "OUTOC";
            strText2 = "输出过电流";
            strText3 = strSIVText;
            break;
        case 0x14:
            strText1 = "IVOC";
            strText2 = "逆变器输出过电流";
            strText3 = strSIVText;
            break;
        case 0x15:
            strText1 = "OUTLV";
            strText2 = "输出低电压";
            strText3 = strSIVText;
            break;
        case 0x16:
            strText1 = "OUTOV";
            strText2 = "输出过电压";
            strText3 = strSIVText;
            break;
        case 0x18:
            strText1 = "HBT";
            strText2 = "HB脱扣";
            strText3 = strSIVText;
            break;
        case 0x21:
            strText1 = "RCDF";
            strText2 = "电阻故障";
            strText3 = strSIVText;
            break;
        case 0x22:
            strText1 = "FCF";
            strText2 = "滤波电容器故障";
            strText3 = strSIVText;
            break;
        case 0xFF:
            strText1 = "MOGI";
            strText2 = "模拟故障";
            strText3 = "SIV发生模拟故障。";
            break;
        default:
            strText1 = "";
            break;
        }
        break;
    case 3:   // BCU
        switch (iCode) {
        case 0x90:
            strText1 = "VLCF";
            strText2 = "空重车行驶信号异常";
            strText3 = "牵引载荷信号异常。\nVVVF不能进行牵引载荷控制。\n运行到当日运营结束为止。";
            break;
        case 0x91:
            strText1 = "RPCF";
            strText2 = "电制模式信号异常";
            strText3 = "";
            break;
        case 0x92:
            strText1 = "MCVF";
            strText2 = "常用制动控制阀异常";
            strText3 = "不能进行常用制动控制。\n再一次接通制动控制器电源之后， 如不恢复时，请关断该制动控制器后再运行。\n运行到下一站后，停止车辆运营。\n\n不能进行常用制动控制。\n再一次接通制动控制器电源之后， 如不恢复时，请关断该制动控制器后再运行。\n运行到终点站后，停止车辆运营。";
            break;
        case 0x93:
            strText1 = "PCVF(1)";
            strText2 = "防滑阀异常（前台车）";
            strText3 = "不能进行1位转向架防滑控制及不缓解时的强迫缓解。\n运行到终点站后，停止车辆运营。";
            break;
        case 0x94:
            strText1 = "PCVF(2)";
            strText2 = "防滑阀异常（后台车）";
            strText3 = "不能进行2位转向架防滑控制及不缓解时的强迫缓解。\n运行到终点站后，停止车辆运营。";
            break;
        case 0x95:
            strText1 = "RFBF";
            strText2 = "电制反馈信号异常";
            strText3 = "再生制动反馈信号异常。\n只进行空气制动控制。\n运行到当日运营结束为止。";
            break;
        case 0x96:
            strText1 = "RAME";
            strText2 = "RAM检测错误";
            strText3 = "存储器读写检查。\n再一次接通制动控制器电源之后，如不恢复时，请关断该制动控制器后再运行。\n运行到下一站后，停止车辆运营。\n\n存储器读写检查。\n再一次接通制动控制器电源之后，如不恢复时，请关断该制动控制器后再运行。\n运行到终点站后，停止车辆运营。";
            break;
        case 0x97:
            strText1 = "TRCF";
            strText2 = "初期测试异常";
            strText3 = "初始检查异常。\n再一次接通制动控制器电源之后， 如不恢复时，请关断该制动控制器后再运行。\n运行到下一车站后，停止车辆运营。";
            break;
        case 0x98:
            strText1 = "TWLF";
            strText2 = "通线逻辑异常";
            strText3 = "列车导线逻辑异常。\n运行到终点站后，停止车辆运营。\n回库后，查看状态数据，确认故障部位。";
            break;
        case 0x9A:
            strText1 = "AS1F";
            strText2 = "第１轴速度异常";
            strText3 = "第1轴速度异常。\n不能进行1轴滑行控制。\n运行到当日运营结束为止。";
            break;
        case 0x9B:
            strText1 = "AS2F";
            strText2 = "第２轴速度异常";
            strText3 = "第2轴速度异常。\n不能进行2轴滑行控制。\n运行到当日运营结束为止。";
            break;
        case 0x9C:
            strText1 = "AS3F";
            strText2 = "第３轴速度异常";
            strText3 = "第3轴速度异常。\n不能进行3轴滑行控制。\n运行到当日运营结束为止。";
            break;
        case 0x9D:
            strText1 = "AS4F";
            strText2 = "第４轴速度异常";
            strText3 = "第4轴速度异常。\n不能进行4轴滑行控制。\n运行到当日运营结束为止。";
            break;
        case 0x9E:
            strText1 = "ARSF";
            strText2 = "空制减算指令异常";
            strText3 = "";
            break;
        case 0x9F:
            strText1 = "PWMF";
            strText2 = "常用制动PWM指令异常";
            strText3 = "";
            break;
        case 0xA0:
            strText1 = "TVLCF";
            strText2 = "T车空重车信号异常";
            strText3 = "";
            break;
        case 0xA1:
            strText1 = "SASF1";
            strText2 = "AS1传感器异常";
            strText3 = "AS1压力传感器异常。\n由AS2进行压力控制。\n运行到终点站后，停止车辆运营。";
            break;
        case 0xA2:
            strText1 = "SASF2";
            strText2 = "AS2传感器异常";
            strText3 = "AS2压力传感器异常。\n由AS1进行压力控制。\n运行到终点站后，停止车辆运营。";
            break;
        case 0xA3:
            strText1 = "SACF";
            strText2 = "AC传感器异常";
            strText3 = "AC压力传感器异常。\n再一次接通制动控制器电源之后， 如不恢复时，请关断该制动控制器后再运行。\n运行到终点站后，停止车辆运营。";
            break;
        case 0xA4:
            strText1 = "SBCF";
            strText2 = "BC传感器异常";
            strText3 = "BC压力传感器异常。\n不能进行制动压力不足·电不缓解检测。\n运行到终点站后，停止车辆运营。";
            break;
        case 0xA5:
            strText1 = "IBD";
            strText2 = "不足检测";
            strText3 = "制动不足。\n再一次接通制动控制器电源之后， 如不恢复时，请关断该制动控制器后再运行。\n运行到终点站后，停止车辆运营。";
            break;
        case 0xA6:
            strText1 = "NRBD";
            strText2 = "不缓解检测";
            strText3 = "制动不缓解。\n请闭合强迫缓解开关。\n运行到终点站后，停止车辆运营。";
            break;
        case 0xA7:
            strText1 = "12VF";
            strText2 = "控制电源异常";
            strText3 = "";
            break;
        default:
            strText1 = "";
            strText2 = "";
            strText3 = "";
            break;
        }
        break;
    case 4:   // ACC
        switch (iCode) {
        case 0x01:
            strText1 = "PWF";
            strText2 = "空调电源故障";
            strText3 = "检查电源过欠压继电器TCR正常信号触点是否断开。";
            break;
        case 0x02:
            strText1 = "EVF";
            strText2 = "紧急通风逆变器故障";
            strText3 = "检查端子排X43-40号端子FAULT是否有+24V信号。";
            break;
        case 0x03:
            strText1 = "VEF11";
            strText2 = "空调机组1通风机1故障";
            strText3 = strACC_Text1+"EFTHR11"+strACC_Text2;
            break;
        case 0x04:
            strText1 = "VEF21";
            strText2 = "空调机组2通风机1故障";
            strText3 = strACC_Text1+"EFTHR21"+strACC_Text2;
            break;
        case 0x05:
            strText1 = "VEF12";
            strText2 = "空调机组1通风机2故障";
            strText3 = strACC_Text1+"EFTHR12"+strACC_Text2;
            break;
        case 0x06:
            strText1 = "VEF22";
            strText2 = "空调机组2通风机2故障";
            strText3 = strACC_Text1+"EFTHR22"+strACC_Text2;
            break;
        case 0x07:
            strText1 = "VEF13";
            strText2 = "空调机组1通风机3故障";
            strText3 = strACC_Text1+"EFTHR13"+strACC_Text2;
            break;
        case 0x08:
            strText1 = "VEF23";
            strText2 = "空调机组2通风机3故障";
            strText3 = strACC_Text1+"EFTHR23"+strACC_Text2;
            break;
        case 0x09:
            strText1 = "VEF14";
            strText2 = "空调机组1通风机4故障";
            strText3 = strACC_Text1+"EFTHR14"+strACC_Text2;
            break;
        case 0x0A:
            strText1 = "VEF24";
            strText2 = "空调机组2通风机4故障";
            strText3 = strACC_Text1+"EFTHR24"+strACC_Text2;
            break;
        case 0x0B:
            strText1 = "CPF11";
            strText2 = "空调机组1压缩机1过流故障";
            strText3 = "检查过流继电器CPFA11是否保护。";
            break;
        case 0x0C:
            strText1 = "CPF21";
            strText2 = "空调机组2压缩机1过流故障";
            strText3 = "检查过流继电器CPFA21是否保护。";
            break;
        case 0x0D:
            strText1 = "CPF12";
            strText2 = "空调机组1压缩机2过流故障";
            strText3 = "检查过流继电器CPFA12是否保护。";
            break;
        case 0x0E:
            strText1 = "CPF22";
            strText2 = "空调机组2压缩机2过流故障";
            strText3 = "检查过流继电器CPFA22是否保护。";
            break;
        case 0x0F:
            strText1 = "CPP11";
            strText2 = "空调机组1压缩机1压力保护";
            strText3 = "检查端子排X43-31号端子F187是否有+24V信号。";
            break;
        case 0x10:
            strText1 = "CPP21";
            strText2 = "空调机组2压缩机1压力保护";
            strText3 = "检查端子排X43-33号端子F287是否有+24V信号。";
            break;
        case 0x11:
            strText1 = "CPP12";
            strText2 = "空调机组1压缩机2压力保护";
            strText3 = "检查端子排X43-32号端子F189是否有+24V信号。";
            break;
        case 0x12:
            strText1 = "CPP22";
            strText2 = "空调机组2压缩机2压力保护";
            strText3 = "检查端子排X43-34号端子F289是否有+24V信号。";
            break;
        case 0x13:
            strText1 = "COF11";
            strText2 = "空调机组1冷凝风机1故障";
            strText3 = strACC_Text1+"CFTHR11"+strACC_Text2;
            break;
        case 0x14:
            strText1 = "COF21";
            strText2 = "空调机组2冷凝风机1故障";
            strText3 = strACC_Text1+"CFTHR21"+strACC_Text2;
            break;
        case 0x15:
            strText1 = "COF12";
            strText2 = "空调机组1冷凝风机2故障";
            strText3 = strACC_Text1+"CFTHR12"+strACC_Text2;
            break;
        case 0x16:
            strText1 = "COF22";
            strText2 = "空调机组2冷凝风机2故障";
            strText3 = strACC_Text1+"CFTHR22"+strACC_Text2;
            break;
        case 0x17:
            strText1 = "FTS1";
            strText2 = "新风温度传感器1故障";
            strText3 = "检查温度模块EXP3端子C+与C-间阻值是否无穷大或零。";
            break;
        case 0x18:
            strText1 = "FTS2";
            strText2 = "新风温度传感器2故障";
            strText3 = "检查温度模块EXP3端子D+与D-间阻值是否无穷大或零。";
            break;
        case 0x19:
            strText1 = "RTS1";
            strText2 = "回风温度传感器1故障";
            strText3 = "检查温度模块EXP3端子A+与A-间阻值是否无穷大或零。";
            break;
        case 0x1A:
            strText1 = "RTS2";
            strText2 = "回风温度传感器2故障";
            strText3 = "检查温度模块EXP3端子B+与B-间阻值是否无穷大或零。";
            break;
        case 0x1B:
            strText1 = "FAN1";
            strText2 = "废排风机1故障";
            strText3 = "检查马达断路器QM1是否保护。";
            break;
        case 0x1C:
            strText1 = "FAN2";
            strText2 = "废排风机2故障";
            strText3 = "检查马达断路器QM2是否保护。";
            break;
        default:
            strText1 = "";
            strText2 = "";
            strText3 = "";
            break;
        }
        break;
    case 5:
        switch (iCode) {
        case 0x01:
            strText1 = "01";
            strText2 = "门驱动电机电路断路";
            strText3 = strDoor_Text1+strDoor_Text4;
            break;
        case 0x02:
            strText1 = "02";
            strText2 = "门“锁到位开关”故障";
            strText3 = strDoor_Text1+strDoor_Text5;
            break;
        case 0x03:
            strText1 = "03";
            strText2 = "门“关到位开关”故障";
            strText3 = strDoor_Text1+strDoor_Text5;
            break;
        case 0x04:
            strText1 = "04";
            strText2 = "门3秒內未解锁";
            strText3 = strDoor_Text1+strDoor_Text4;
            break;
        case 0x05:
            strText1 = "05";
            strText2 = "门位置传感器故障";
            strText3 = strDoor_Text1+strDoor_Text6;
            break;
        case 0x06:
            strText1 = "06";
            strText2 = "门未经许可离开关锁到位位置";
            strText3 = strDoor_Text1+"尝试关门。如满足开门或关门条件，这门尝试再次运动。应隔离此门。";
            break;
        case 0x07:
            strText1 = "07";
            strText2 = "内部安全继电器故障";
            strText3 = strDoor_Text1+strDoor_Text6;
            break;
        case 0x08:
            strText1 = "08";
            strText2 = "安全互锁回路异常故障";
            strText3 = strDoor_Text1+strDoor_Text6;
            break;
        case 0x09:
            strText1 = "09";
            strText2 = "门锁闭装置故障";
            strText3 = strDoor_Text1+"在没有任何开门进程到情况下，门位置传感器检测到数次门向开门方向偏移一定距离。";
            break;
//        case 0x10:
//            strText1 = "10";
//            strText2 = "关门过程中的障碍检测触发指定次数";
//            strText3 = strDoor_Text2+"次数可设定，指定次尝试之后，门自动打开到开到位位置。如满足开门活关门条件，则门尝试再次运行。";
//            break;
//        case 0x11:
//            strText1 = "11";
//            strText2 = "开门过程中的障碍检测触发指定次数";
//            strText3 = strDoor_Text2+"次数可设定，指定次参数之后，门停下并接受这一位置为最大可达开门位置。如满足开门或关门条件，则门尝试再次运动。";
//            break;
        case 0x12:
            strText1 = "12";
            strText2 = "EDCU输出口O0短路";
            strText3 = strDoor_Text3+strDoor_Text7;
            break;
        case 0x13:
            strText1 = "13";
            strText2 = "EDCU输出口O1短路";
            strText3 = strDoor_Text3+strDoor_Text7;
            break;
        case 0x14:
            strText1 = "14";
            strText2 = "EDCU输出口O2短路";
            strText3 = strDoor_Text3+strDoor_Text7;
            break;
        case 0x15:
            strText1 = "15";
            strText2 = "EDCU输出口O3短路";
            strText3 = strDoor_Text3+strDoor_Text7;
            break;
        case 0x16:
            strText1 = "16";
            strText2 = "诊断存储器故障";
            strText3 = strDoor_Text3+"存储器中的所有诊断数据将会丢失，若在重新上电时发生此故障，则从存储器内读取的相关门控制参数将赋初值，不影响门运动功能。";
            break;
//        case 0x17:
//            strText1 = "17";
//            strText2 = "数据总线通信故障";
//            strText3 = strDoor_Text1+"与车辆数据总线到连接中断，不影响门运动功能，应检查此门通讯插头。";
//            break;
        case 0x24:
            strText1 = "24";
            strText2 = "维护按钮恒有效故障";
            strText3 = strDoor_Text3+"不影响维护按钮开关门。";
            break;
        case 0x25:
            strText1 = "25";
            strText2 = "门地址编码故障";
            strText3 = ""; //strDoor_Text3+"不影响维护按钮开关门。";
            break;
        default:
            strText1 = "";
            strText2 = "";
            strText3 = "";
            break;
        }
        break;
    default:
        strText1 = "";
        strText2 = "";
        strText3 = "";
        break;
    }
    ui->labeldetail_2_1->setText(strText1);
    ui->labeldetail_2_2->setText(strText2);
    ui->labelerrdetail3->setText(strText3);
}

void errdetail::slot_1S_Timer_Out(QString strDateTime){
    ui->title_3->setText(strDateTime);
}

void errdetail::on_buttonok_clicked(){
    buttonBeep();
    this->reject();
}
