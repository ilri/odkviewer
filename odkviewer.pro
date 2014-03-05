#-------------------------------------------------
#
# Project created by QtCreator 2014-02-18T11:45:53
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = odkviewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    frmsettings.cpp \
    choosedir.cpp \
    formsmodel.cpp \
    frmdata.cpp \
    datamodel.cpp \
    frmdataeditor.cpp \
    odkformreader.cpp \
    editormodel.cpp \
    chosevalue.cpp \
    lkptablemodel.cpp \
    flickcharm.cpp \
    about.cpp \
    odkthread.cpp

HEADERS  += mainwindow.h \
    frmsettings.h \
    choosedir.h \
    formsmodel.h \
    frmdata.h \
    datamodel.h \
    frmdataeditor.h \
    odkformreader.h \
    genstruct.h \
    editormodel.h \
    chosevalue.h \
    lkptablemodel.h \
    flickcharm.h \
    about.h \
    odkthread.h

FORMS    += \
    frmsettings.ui \
    mainwindow.ui \
    choosedir.ui \
    frmdata.ui \
    frmdataeditor.ui \
    chosevalue.ui \
    about.ui

CONFIG += mobility
MOBILITY = 

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml

RESOURCES += \
    movies.qrc

