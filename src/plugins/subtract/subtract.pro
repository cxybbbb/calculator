TEMPLATE = lib
TARGET = subtract_operation
CONFIG += plugin c++11
QT += core
QT -= gui

greaterThan(QT_MAJOR_VERSION, 5) {
    CONFIG -= c++11
    CONFIG += c++17
}

msvc: QMAKE_CXXFLAGS += /utf-8

win32 {
    CONFIG(debug, debug|release): DESTDIR = $$OUT_PWD/../../../bin/debug/plugins
    else:                         DESTDIR = $$OUT_PWD/../../../bin/release/plugins
} else {
    DESTDIR = $$OUT_PWD/../../../bin/plugins
}

INCLUDEPATH += $$PWD/../../../interface
HEADERS += SubtractOperationPlugin.h \
           $$PWD/../../../interface/IOperationPlugin.h
SOURCES += SubtractOperationPlugin.cpp
