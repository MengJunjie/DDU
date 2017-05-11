#ifndef WRITETHREAD_H
#define WRITETHREAD_H

#include <QThread>
#include  "datastruct.h"

class writethread : public QThread
{
    Q_OBJECT
public:
    explicit writethread(QObject *parent = 0);
    
signals:
    
public slots:
public:
    void  stop();
    volatile  bool  m_rtn;
private:
    void  run();
    
};

#endif // WRITETHREAD_H
