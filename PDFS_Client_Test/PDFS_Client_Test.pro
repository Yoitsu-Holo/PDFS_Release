QT += core gui
QT += core
QT += widgets
QT += network
QT += core5compat
QT += svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    download.cpp \
    loginwindow.cpp \
    logonwindow.cpp \
    main.cpp \
    mainconnect.cpp \
    mainwindow.cpp \
    transmissionmodel.cpp \
    upload.cpp\
    PDFSFileSystem.cpp

HEADERS += \
    download.h \
    transmissionmodel.h \
    upload.h \
    userinfo.h \
    loginwindow.h \
    logonwindow.h \
    mainconnect.h \
    mainwindow.h  \
    protocol.h

FORMS += \
    loginwindow.ui \
    logonwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc \
    qss.qrc\
    server.qrc

OTHER_FILES += \
    protocol.md

#pack tool path : D:\soft\Qt\6.2.3\msvc2019_64\bin\windeployqt.exe
