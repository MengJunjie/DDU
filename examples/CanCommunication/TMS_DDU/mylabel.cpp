#include "mylabel.h"
#include "gui_config.h"

extern void buttonBeep();

MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{
    bErrorState = false;
}

MyLabel::~MyLabel(){

}

void MyLabel::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        emit clicked();
    }

    if(bErrorState){
        buttonBeep();
    }

    //将该事件传给父类处理
    QLabel::mousePressEvent(event);
}

//void MyLabel::focusOutEvent(QFocusEvent *event){
//    if(QLabel::isModified()){
//        QLabel::focusInEvent(event);
//    }
//    else{
//        QLabel::focusOutEvent(event);
//    }
//}

void MyLabel::setErrorState(bool bState){
    bErrorState = bState;
}

bool MyLabel::getErrorState(){
    return bErrorState;
}

void MyLabel::setMyLabelText(QString str){
    if("1" == str){
        setText(str);
        setStyleSheet(QSS_BG_GREEN_BR_WHITE_WD_WHITE);
    }
    else{
        setText(str);
        setStyleSheet(QSS_BG_BLACK_BR_WHITE_WD_WHITE);
    }
}
