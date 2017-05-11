#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QMouseEvent>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = 0);
    ~MyLabel();

    void setErrorState(bool bState);
    bool getErrorState();
    void setMyLabelText(QString str);

protected:
    //重写mousePressEvent事件
    virtual void mousePressEvent(QMouseEvent *event);
//    virtual void focusOutEvent(QFocusEvent *event);

signals:
    //自定义clicked()信号,在mousePressEvent事件发生时触发
    void clicked();

public slots:

private:
    bool bErrorState;
};

#endif // MYLABEL_H
