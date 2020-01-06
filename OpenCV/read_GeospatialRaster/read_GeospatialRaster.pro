QT += core gui
greaterThan(QT_MAJOR_VERSION,4):QT +=widgets
TARGET = read_geo
TEMPLATE = app
CONFIG += C++11

INCLUDEPATH += /usr/local/include/

LIBS +=/usr/local/lib/libopencv_*.so

SOURCES += \
    main.cpp
