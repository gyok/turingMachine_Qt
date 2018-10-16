#-------------------------------------------------
#
# Project created by QtCreator 2018-04-05T01:48:55
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TuringMachine
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        turingmachine.cpp \
    bubble.cpp \
    bubblewindow.cpp \
    turingline.cpp \
    bubbleconnection.cpp \
    bubbleconnectionline.cpp \
    bubbleconnectionmanager.cpp \
    linepushbutton.cpp \
    controlBar.cpp \
    core.cpp

HEADERS += \
        turingmachine.h \
    bubble.h \
    bubblewindow.h \
    turingline.h \
    bubbleconnection.h \
    bubbleconnectionline.h \
    bubbleconnectionmanager.h \
    linepushbutton.h \
    controlBar.h \
    core.h

FORMS +=
