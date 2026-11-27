TEMPLATE = app
TARGET = Calculator
QT += core gui widgets
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 5) {
    CONFIG -= c++11
    CONFIG += c++17
}

# 源码以 UTF-8 保存，MSVC 默认按本地代码页读取会乱码，显式指定编码。
msvc: QMAKE_CXXFLAGS += /utf-8

# 可执行程序与插件必须处于固定的相对位置：<bin>/<config>/ 与 <bin>/<config>/plugins/，
# 否则运行时 applicationDirPath()/plugins 找不到 DLL。
win32 {
    CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/../../bin/debug
    else:                         DESTDIR = $$OUT_PWD/../../bin/release
} else {
    DESTDIR = $$OUT_PWD/../../bin
}

INCLUDEPATH += $$PWD/../../interface \
               $$PWD/../core

win32 {
    CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lib/debug
    else:                         LIBS += -L$$OUT_PWD/../../lib/release
} else {
    LIBS += -L$$OUT_PWD/../../lib
}
LIBS += -lcalculator_core

SOURCES += main.cpp \
           mainwindow.cpp
HEADERS += mainwindow.h
FORMS += mainwindow.ui
