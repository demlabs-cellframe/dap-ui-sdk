#-------------------------------------------------
#
# Project created by QtCreator 2018-01-12T09:27:25
#
#-------------------------------------------------

SOURCES += \
    $$PWD/schedule.cpp\
    $$PWD/schedules.cpp


HEADERS  += \
    $$PWD/schedule.h\
    $$PWD/schedules.h \
    $$PWD/SheduleElement.h

include(auxiliary/auxiliary.pri)
include(auxiliary/dap-developer-mode.pri)
include(controls/controls.pri)
include(screens/screens.pri)

INCLUDEPATH += $$PWD

win32: LIBS += -lgdi32
unix: !mac: !android: LIBS += -lX11 -lXrandr
mac:LIBS += -framework AppKit

DISTFILES +=
