#-------------------------------------------------
#
# Project created by QtCreator 2018-10-20T19:10:48
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = BasicDrawing
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    painterarea.cpp \
    mypath.cpp \
    dialog/dialogms.cpp \
    dialog/dialogmm.cpp \
    data/mypathdata.cpp \
    data/curvepoint.cpp \
    splash/splash.cpp \
    data/labelpoint.cpp \
    dialog/dialogdesign.cpp \
    dialog/dialogdesign/dialogpantsh.cpp \
    dialog/dialogsize.cpp \
    dxflib/dl_dxf.cpp \
    dxflib/dl_writer_ascii.cpp \
    components/mycomponent.cpp \
    rules/myrule.cpp \
    dialog/dialogtest/dialogtestcurve.cpp \
    dialog/dialogrule/dialogruleedit.cpp \
    dialog/dialogrule/dialogrule.cpp

HEADERS += \
    mainwindow.h \
    painterarea.h \
    mypath.h \
    dialog/dialogms.h \
    dialog/dialogmm.h \
    data/mypathdata.h \
    data/curvepoint.h \
    splash/splash.h \
    data/labelpoint.h \
    dialog/dialogdesign.h \
    dialog/dialogdesign/dialogpantsh.h \
    dialog/dialogsize.h \
    dxflib/dl_dxf.h \
    components/mycomponent.h \
    rules/myrule.h \
    dialog/dialogtest/dialogtestcurve.h \
    dialog/dialogrule/dialogruleedit.h \
    dialog/dialogrule/dialogrule.h

FORMS += \
        mainwindow.ui \
    dialog/dialogms.ui \
    dialog/dialogmm.ui \
    splash/splash.ui \
    dialog/dialogdesign.ui \
    dialog/dialogdesign/dialogpantsh.ui \
    dialog/dialogsize.ui \
    dialog/dialogtest/dialogtestcurve.ui \
    dialog/dialogrule/dialogruleedit.ui \
    dialog/dialogrule/dialogrule.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images/images.qrc

DISTFILES += \
    images/ico.rc

RC_FILE = images/ico.rc
