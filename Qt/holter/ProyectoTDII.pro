#-------------------------------------------------
#
# Project created by QtCreator 2016-11-12T22:52:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = ProyectoTDII
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    grafico.cpp \
    qcustomplot.cpp \
    tiemporeal.cpp

HEADERS  += mainwindow.h \
    grafico.h \
    qcustomplot.h \
    tiemporeal.h

FORMS    += mainwindow.ui \
    grafico.ui \
    tiemporeal.ui


win32: LIBS += -L$$PWD/./ -llibusb-1.0.dll

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
