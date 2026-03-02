#-------------------------------------------------
#
# Project created by QtCreator 2018-01-12T09:27:25
#
#-------------------------------------------------

greaterThan(QT_MAJOR_VERSION, 5): QT += statemachine core5compat

SOURCES += \
    $$PWD/schedule.cpp\
    $$PWD/schedules.cpp


HEADERS  += \
    $$PWD/schedule.h\
    $$PWD/schedules.h \
    $$PWD/SheduleElement.h

include(auxiliary/auxiliary.pri)
include(controls/controls.pri)
include(screens/screens.pri)

INCLUDEPATH += $$PWD

win32: LIBS += -lgdi32
unix: !mac: !android: LIBS += -lX11 -lXrandr
mac:LIBS += -framework AppKit

DISTFILES +=
