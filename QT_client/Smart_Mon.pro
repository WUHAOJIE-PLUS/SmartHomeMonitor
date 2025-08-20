QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Login.cpp \
    TcpCon.cpp \
    alertitem.cpp \
    alertwindow.cpp \
    ciphertext.cpp \
    cmainwindow.cpp \
    conbtn.cpp \
    cryptstring.cpp \
    ctitlebar.cpp \
    deviceList.cpp \
    ffmpegkits.cpp \
    ffmpegwin.cpp \
    ffwidget.cpp \
    framelesswidget.cpp \
    http.cpp \
    main.cpp \
    wininfo.cpp

HEADERS += \
    Login.h \
    Message.h \
    TcpCon.h \
    alertitem.h \
    alertwindow.h \
    ciphertext.h \
    cmainwindow.h \
    conbtn.h \
    cryptstring.h \
    ctitlebar.h \
    deviceList.h \
    ffmpegkits.h \
    ffmpegwin.h \
    ffwidget.h \
    framelesswidget.h \
    http.h \
    wininfo.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

# 设置FFmpeg路径
FFMPEG_PATH = /usr/local/ffmpeg-4.4.1

# 包含头文件
INCLUDEPATH += $${FFMPEG_PATH}/include

# 链接库文件
LIBS += -L$${FFMPEG_PATH}/lib \
        -lavcodec -lavformat -lavutil -lswscale -lswresample
