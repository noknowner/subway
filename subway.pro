QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    clientdataprocess.cpp \
    devconfigure.cpp \
    heartbeatthread.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    pay.cpp \
    sqldataprocess.cpp \
    tgraphicsview.cpp

HEADERS += \
    clientdataprocess.h \
    datastruct.h \
    devconfigure.h \
    heartbeatthread.h \
    login.h \
    mainwindow.h \
    pay.h \
    sqldataprocess.h \
    tgraphicsview.h

FORMS += \
    devconfigure.ui \
    login.ui \
    mainwindow.ui \
    pay.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md \
    subway.pro.user

RESOURCES += \
    res.qrc
