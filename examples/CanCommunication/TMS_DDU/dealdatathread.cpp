#include "dealdatathread.h"

dealdatathread::dealdatathread(QObject *parent) :
    QThread(parent)
{
    m_rtn=false;
}

void  dealdatathread::stop()
{
    m_rtn=false;
}

void  dealdatathread::run()
{
    m_rtn=true;
    while(m_rtn)
    {

        sleep(300);
    }
}
