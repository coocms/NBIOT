#-------------------------------------------------
#
# Project created by QtCreator 2018-12-24T19:20:48
#
#-------------------------------------------------


QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NB_IOT
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    form.cpp \
    registerform.cpp \
    dialog.cpp

HEADERS  += widget.h \
    form.h \
    registerform.h \
    dialog.h

FORMS    += widget.ui \
    form.ui \
    registerform.ui \
    dialog.ui

RESOURCES += \
    res.qrc

DISTFILES +=
