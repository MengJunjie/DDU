greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SubwayDDU
TEMPLATE = app

QT       += core gui \
	    network \
	    sql

INCLUDEPATH += ../../../include ../../../include/unix ../../../include/timers_unix #../../../include/can_socket
LIBS +=  ../../../drivers/unix/libcanfestival_unix.a ../../../drivers/can_socket/libcanfestival_can_socket.so ../../../src/libcanfestival.a
LIBS += /opt/fsl-imx-x11/4.1.15-1.1.0/sysroots/cortexa9hf-vfp-neon-poky-linux-gnueabi/usr/lib/librt.so
LIBS += /opt/fsl-imx-x11/4.1.15-1.1.0/sysroots/cortexa9hf-vfp-neon-poky-linux-gnueabi/lib/libdl.so.2

HEADERS +=	\
			dialog/maincarinfo.h \
			dialog/reparismenu.h \
			airconditioninfo.h \
			doorcheck.h \
			airpumpcheck.h \
			brakecheck.h \
			password.h \
			errinfo.h \
			runinfo.h \
			speedcheck.h \
			mainmenu.h \
			carinfoinit.h \
			setdirection.h \
			setpara.h \
			delerrinfo.h \
			delruninfo.h \
			speedcheckchange.h \
			delspeedinfo.h \
			keepmove.h \
			sivcheck.h \
			viewerrlist1.h \
			viewerrlist2.h \
			setaspara.h \
			setdatetime.h \
			setpassword.h \
			rcvthread.h \
			writethread.h \
			dealdatathread.h \
			datastruct.h \
			mylabel.h \
			errorrecord.h \
			errdetail.h \
			setstation.h \
			linkerreadout.h \
			errorvvvfsiv.h \
			initsetsure.h \
			runtable.h \
			gui_config.h \
			../can1init.h \
			../can1operation.h \
			../canbootup.h \
			../can2init.h \
			../can2operation.h \
			../Can1ObjDict.h \
			../Can2ObjDict.h \
			../cansignalsslots.h \
			networkstation.h \
			setcarid.h \
			mainwidget.h \
			../objectdictdata.h \
			../cansendandrecv.h \
			threadsendpdo.h \
    ioscreen.h \
    threaddistance.h \
    threadfaultrecord.h \
    ptudownload.h

SOURCES +=  \
			dialog/maincarinfo.cpp \
			dialog/reparismenu.cpp \
			airconditioninfo.cpp \
			doorcheck.cpp \
			airpumpcheck.cpp \
			brakecheck.cpp \
			password.cpp \
			errinfo.cpp \
			runinfo.cpp \
			speedcheck.cpp \
			main.cpp \
			mainmenu.cpp \
			carinfoinit.cpp \
			setdirection.cpp \
			setpara.cpp \
			delerrinfo.cpp \
			delruninfo.cpp \
			speedcheckchange.cpp \
			delspeedinfo.cpp \
			keepmove.cpp \
			sivcheck.cpp \
			viewerrlist1.cpp \
			viewerrlist2.cpp \
			setaspara.cpp \
			setdatetime.cpp \
			setpassword.cpp \
			rcvthread.cpp \
			writethread.cpp \
			dealdatathread.cpp \
			datastruct.cpp \
			mylabel.cpp \
			errorrecord.cpp \
			errdetail.cpp \
			setstation.cpp \
			linkerreadout.cpp \
			errorvvvfsiv.cpp \
			initsetsure.cpp \
			runtable.cpp \
			gui_config.cpp \
			 ../can1init.cpp \
			 ../can1operation.cpp \
			../canbootup.cpp \
			../can2init.cpp \
			../can2operation.cpp \
			../Can1ObjDict.c \
			../Can2ObjDict.c \
			../cansignalsslots.cpp \
			networkstation.cpp \
			setcarid.cpp \
			mainwidget.cpp \
			../objectdictdata.cpp \
			../cansendandrecv.cpp \
			threadsendpdo.cpp \
    ioscreen.cpp \
    threaddistance.cpp \
    threadfaultrecord.cpp \
    ptudownload.cpp

FORMS +=	\
			dialog/maincarinfo.ui \
			dialog/reparismenu.ui \
			airconditioninfo.ui \
			doorcheck.ui \
			airpumpcheck.ui \
			brakecheck.ui \
			password.ui \
			errinfo.ui \
			runinfo.ui \
			speedcheck.ui \
			mainmenu.ui \
			carinfoinit.ui \
			setdirection.ui \
			setpara.ui \
			delerrinfo.ui \
			delruninfo.ui \
			speedcheckchange.ui \
			delspeedinfo.ui \
			keepmove.ui \
			sivcheck.ui \
			viewerrlist1.ui \
			viewerrlist2.ui \
			setaspara.ui \
			setdatetime.ui \
			setpassword.ui \
			errorrecord.ui \
			errdetail.ui \
			setstation.ui \
			linkerreadout.ui \
			errorvvvfsiv.ui \
			initsetsure.ui \
			runtable.ui \
			networkstation.ui \
			setcarid.ui \
			mainwidget.ui \
    ioscreen.ui \
    ptudownload.ui

RESOURCES +=  \
			    resource.qrc

OTHER_FILES += \
			    qss/common.qss
