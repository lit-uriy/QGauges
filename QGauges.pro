QT += core gui widgets serialport

TARGET = QGauges
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    usbserialdevice.cpp

HEADERS += \
    mainwindow.h \
    usbserialdevice.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc

linux {
    LIBS += \
        -ludev
}

macx {
    QMAKE_INFO_PLIST = Info.plist
    DISTFILES += \
        Info.plist
}

