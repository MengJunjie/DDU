#ifndef DEALDATATHREAD_H
#define DEALDATATHREAD_H

#include <QThread>
#include  "datastruct.h"

class dealdatathread : public QThread
{
    Q_OBJECT
public:
    explicit dealdatathread(QObject *parent = 0);
    
signals:
    
public slots:
public:
    void  stop();
    volatile  bool  m_rtn;
private:
    void  run();
    
};

#endif // DEALDATATHREAD_H
