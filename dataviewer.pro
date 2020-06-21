QT += core gui \
    widgets
QT += charts

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    datachartviewer.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    datachartviewer.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
