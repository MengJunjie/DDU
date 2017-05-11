#include "writethread.h"

writethread::writethread(QObject *parent) :
    QThread(parent)
{
    m_rtn=false;
}

void  writethread::stop()
{
    m_rtn=false;
}

void  writethread::run()
{
    m_rtn=true;
    while(m_rtn)
    {

        sleep(300);
    }
}
