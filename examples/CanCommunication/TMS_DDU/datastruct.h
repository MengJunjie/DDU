#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <QObject>
#include  "qreadwritelock.h"
#include  <QList>

////CANID TYPE(bit28--25)
//#define  NMT            0x00
//#define  SYNC           0x01
//#define  TIMESTAMP      0x02
//#define  PDO1SEND       0x03
//#define  PDO1RCV        0x04
//#define  PDO2SEND       0x05
//#define  PDO2RCV        0x06
//#define  PDO3SEND       0x07
//#define  PDO3RCV        0x08
//#define  PDO4SEND       0x09
//#define  PDO4RCV        0x0b
//#define  SDOSEND        0x0c
//#define  SDORCV         0x0e

////CANID  ID(bit24---21)
//#define  ERM1           0x01
//#define  ERM2           0x02
//#define  DDU1           0x03
//#define  DDU2           0x04
//#define  MC1            0x05
//#define  MC2            0x06
//#define  Tcar           0x07
//#define  T1car          0x08
//#define  T2car          0x09
//#define  Mcar           0x0a
//#define  BCU1           0x0b
//#define  BCU2           0x0c
////CANID  BOARDID(bit20---18)
//#define  BOARDA         0x00
//#define  BOARD1         0x01
//#define  BOARD2         0x02
//#define  BOARD3         0x03
//#define  BOARD4         0x04
//#define  BOARDPOW       0x05
//#define  BOARDB         0x07


//the struct whice head is the main inuse
typedef struct
{
    bool    m_carhead;
    bool    m_carwake;
}_CURRENTUSE;


//whice line of rs232 to can  transmission is inuse
typedef struct
{
    bool    m_rs_can1;
    bool    m_rs_can2;
}_HEALTH;

//struct of rs232 to can data
typedef struct
{
    unsigned  int     ID;
    unsigned  char    SendType;
    unsigned  char    ExternFlag;
    unsigned  char    RemoteFlag;
    unsigned  char    DataLength;
    unsigned  char    Data[8];
}_RSDATA;


//struct of date and times
typedef struct
{
    unsigned char   m_year;
    unsigned char   m_month;
    unsigned char   m_day;
    unsigned char   m_hour;
    unsigned char   m_minute;
    unsigned char   m_second;
}DATETIME;

class datastruct : public QObject
{
    Q_OBJECT
public:
    explicit datastruct(QObject *parent = 0);
    
signals:
    
public slots:

public:
    static   _CURRENTUSE              m_inuse;
    static   QReadWriteLock          m_inuseLock;

    static   QReadWriteLock                  m_rcvlistLock;
    static   QList<_RSDATA>                   m_rcvlistlist;
   static    QList<_RSDATA>::iterator    m_rcvlistItear;

    static   QReadWriteLock                   m_sendlistLock;
    static   QList<_RSDATA>                   m_sendlist;
    static   QList<_RSDATA>::iterator    m_sendItear;

    //lock for datetime
    static   QReadWriteLock      m_datetieLock;
    static   DATETIME  m_datetime;
    
};

#endif // DATASTRUCT_H
