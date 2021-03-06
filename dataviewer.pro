
QT += core gui widgets serialport charts

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    datachartviewer.cpp \
    datalogger.cpp \
    layout.cpp \
    main.cpp \
    mainwindow.cpp \
    serialportreader.cpp

HEADERS += \
    datachartviewer.h \
    datalogger.h \
    layout.h \
    mainwindow.h \
    serialportreader.h

FORMS += \
    mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    layouts/layout.json
