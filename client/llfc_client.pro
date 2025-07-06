QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    global.cpp \
    httpmanager.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    registerdialog.cpp

HEADERS += \
    global.h \
    httpmanager.h \
    logindialog.h \
    mainwindow.h \
    registerdialog.h \
    singleton.h

FORMS += \
    logindialog.ui \
    mainwindow.ui \
    registerdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rsc.qrc

DISTFILES += \
    config.ini

# copy config.ini to build directory
win32::CONFIG(debug, debug | release)
{
    TargetConfig = $${_PRO_FILE_PWD_}/config.ini
    TargetConfig = $$replace(TargetConfig, /, \\) # TO ensure Windows compatibility

    OutputDir = $${OUT_PWD}/debug #DESTDIR is the directory where the executable will be placed
    OutputDir = $$replace(OutputDir, /, \\)

    system(copy /Y \"$$TargetConfig\" \"$$OutputDir\\config.ini\") # copy config.ini to build directory
}
