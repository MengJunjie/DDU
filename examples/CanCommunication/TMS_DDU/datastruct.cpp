#include "datastruct.h"

_CURRENTUSE  datastruct::m_inuse;
QReadWriteLock   datastruct::m_inuseLock;

QReadWriteLock                  datastruct::m_rcvlistLock;
QList<_RSDATA>                   datastruct::m_rcvlistlist;
QList<_RSDATA>::iterator    datastruct::m_rcvlistItear;

QReadWriteLock                   datastruct::m_sendlistLock;
QList<_RSDATA>                   datastruct::m_sendlist;
QList<_RSDATA>::iterator    datastruct::m_sendItear;

QReadWriteLock                  datastruct::m_datetieLock;
DATETIME                             datastruct::m_datetime;

datastruct::datastruct(QObject *parent) :
    QObject(parent)
{
}
