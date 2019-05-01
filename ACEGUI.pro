#-------------------------------------------------
#
# Project created by QtCreator 2019-04-26T10:18:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ACEGUI
TEMPLATE = app

TRANSLATIONS += acegui_translation_zh_cn.ts

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
        acegui.cpp \
    ../ACEdemo/ace.cpp \
    ../ACEdemo/oneace.cpp

HEADERS += \
        acegui.h \
    ../ACEdemo/ace.h \
    ../ACEdemo/commondatatype.h \
    ../ACEdemo/oneace.h

FORMS += \
        acegui.ui

#配置GMP库
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/../../Qt/Qt5.9.8/lib/ -lgmp

INCLUDEPATH += $$PWD/../../Qt/Qt5.9.8/include
DEPENDPATH += $$PWD/../../Qt/Qt5.9.8/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../Qt/Qt5.9.8/lib/gmp.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../../Qt/Qt5.9.8/lib/libgmp.a

win32: LIBS += -L$$PWD/../../Qt/Qt5.9.8/lib/ -lgmpxx

INCLUDEPATH += $$PWD/../../Qt/Qt5.9.8/include
DEPENDPATH += $$PWD/../../Qt/Qt5.9.8/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../Qt/Qt5.9.8/lib/gmpxx.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../../Qt/Qt5.9.8/lib/libgmpxx.a

RESOURCES += \
    images.qrc
