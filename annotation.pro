#-------------------------------------------------
#
# Project created by QtCreator 2017-04-22T14:37:59
#
#-------------------------------------------------

QT       += core gui
#CONFIG += console
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = annotation
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imagewidget.cpp \
    classificationstruct.cpp


HEADERS  += mainwindow.h \
    imagewidget.h \
    classificationstruct.h

FORMS    += mainwindow.ui


#INCLUDEPATH += $$PWD/opencv/build/include

#DEPENDPATH += $$PWD/libQGLViewer-2.6.4/QGLViewer \


#debug {
#    LIBS += -L$$PWD/opencv/build/x64/vc12/lib -lopencv_world310d
#}

#release {
#    LIBS += -L$$PWD/opencv/build/x64/vc12/lib -lopencv_world310
#}
