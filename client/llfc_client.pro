QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    baselistitem.cpp \
    basemessageitem.cpp \
    bubbleframe.cpp \
    chatdialog.cpp \
    chatpage.cpp \
    chatviewarea.cpp \
    clickablelabel.cpp \
    customized_button.cpp \
    customized_editline.cpp \
    customized_label.cpp \
    customized_textedit.cpp \
    forgetlabel.cpp \
    global.cpp \
    httpmanager.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    pictureframe.cpp \
    registerdialog.cpp \
    resetpassworddialog.cpp \
    sessionlist.cpp \
    sessionlistitem.cpp \
    statewidget.cpp \
    tcpmanager.cpp \
    textframe.cpp \
    timerbutton.cpp

HEADERS += \
    baselistitem.h \
    basemessageitem.h \
    bubbleframe.h \
    chatdialog.h \
    chatpage.h \
    chatviewarea.h \
    clickablelabel.h \
    customized_button.h \
    customized_editline.h \
    customized_label.h \
    customized_textedit.h \
    forgetlabel.h \
    global.h \
    httpmanager.h \
    logindialog.h \
    mainwindow.h \
    pictureframe.h \
    registerdialog.h \
    resetpassworddialog.h \
    sessionlist.h \
    sessionlistitem.h \
    singleton.h \
    statewidget.h \
    tcpmanager.h \
    textframe.h \
    timerbutton.h

FORMS += \
    chatdialog.ui \
    chatpage.ui \
    chatviewarea.ui \
    logindialog.ui \
    mainwindow.ui \
    registerdialog.ui \
    resetpassworddialog.ui \
    sessionlistitem.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rsc.qrc

DISTFILES += \
    config.ini \
    images/add_friend_hover.png \
    images/add_friend_normal.png \
    images/add_friend_pressed.png \
    images/addtip.png \
    images/arowdown.png \
    images/chat_icon.png \
    images/chat_icon_hover.png \
    images/chat_icon_press.png \
    images/chat_icon_select_hover.png \
    images/chat_icon_select_press.png \
    images/close_search.png \
    images/close_transparent.png \
    images/contact_list.png \
    images/contact_list_hover.png \
    images/contact_list_press.png \
    images/female.png \
    images/filedir.png \
    images/filedir_hover.png \
    images/filedir_press.png \
    images/head_1.jpg \
    images/head_2.jpg \
    images/head_3.jpg \
    images/head_4.jpg \
    images/head_5.jpg \
    images/hidePassword_hover.png \
    images/hidePassword_normal.png \
    images/ice.png \
    images/loading.gif \
    images/male.png \
    images/msg_chat_hover.png \
    images/msg_chat_normal.png \
    images/msg_chat_press.png \
    images/red_point.png \
    images/right_tip.png \
    images/search.png \
    images/settings.png \
    images/settings_hover.png \
    images/settings_press.png \
    images/settings_select_hover.png \
    images/settings_select_press.png \
    images/showPassword_hover.png \
    images/showPassword_normal.png \
    images/smile.png \
    images/smile_hover.png \
    images/smile_press.png \
    images/tipclose.png \
    images/video_chat_hover.png \
    images/video_chat_normal.png \
    images/video_chat_press.png \
    images/voice_chat_hover.png \
    images/voice_chat_normal.png \
    images/voice_chat_press.png \
    images/wechat.png

# copy config.ini to build directory
win32::CONFIG(debug, debug | release)
{
    TargetConfig = $${_PRO_FILE_PWD_}/config.ini
    TargetConfig = $$replace(TargetConfig, /, \\) # TO ensure Windows compatibility

    OutputDir = $${OUT_PWD}/debug #DESTDIR is the directory where the executable will be placed
    OutputDir = $$replace(OutputDir, /, \\)

    system(copy /Y \"$$TargetConfig\" \"$$OutputDir\\config.ini\") # copy config.ini to build directory
}
