HEADERS += \
    board.h \
    stone.h \
    step.h \
    netgame.h

SOURCES += \
    board.cpp \
    main.cpp \
    stone.cpp \
    step.cpp \
    netgame.cpp

QT += core widgets gui network

QMAKE_CXXFLAGS += -std=c++11

RESOURCES += \
    res.qrc
