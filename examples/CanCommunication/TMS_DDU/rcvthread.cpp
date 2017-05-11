#include "rcvthread.h"

rcvthread::rcvthread(QObject *parent) :
    QThread(parent)
{
    m_rtn=false;
}

void  rcvthread::stop()
{
    m_rtn=false;
}

void  rcvthread::run()
{
    m_rtn=true;
    while(m_rtn)
    {

        sleep(300);
    }
}
