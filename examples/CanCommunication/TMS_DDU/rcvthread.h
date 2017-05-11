#ifndef RCVTHREAD_H
#define RCVTHREAD_H

#include <QThread>
#include  "datastruct.h"

class rcvthread : public QThread
{
    Q_OBJECT
public:
    explicit rcvthread(QObject *parent = 0);
    
signals:
    
public slots:
public:
    void  stop();
    volatile  bool  m_rtn;
private:
    void  run();
    
};

#endif // RCVTHREAD_H
