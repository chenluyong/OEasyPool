TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += "../src/"
DESTDIR   = ../bin




SOURCES += "../demo/test_base/ThreadPoolDemo.cpp" \
    ../src/OE/EasyPool/TaskQueue.cpp \
    ../src/OE/EasyPool/ThreadPool.cpp \
    ../test/TaskTest.cpp


HEADERS += \
    ../src/OE/EasyPool/Task.h \
    ../src/OE/EasyPool/TaskQueue.h \
    ../src/OE/EasyPool/ThreadPool.h \
    ../test/TaskTest.h
