TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += "../src/"
DESTDIR   = ../bin




SOURCES += "../demo/test_base/ThreadPoolDemo.cpp" \
    ../src/TaskQueue.cpp \
    ../src/ThreadPool.cpp \
    ../test/TaskTest.cpp


HEADERS += \
    ../src/Task.h \
    ../src/TaskQueue.h \
    ../src/ThreadPool.h \
    ../test/TaskTest.h
