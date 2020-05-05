#-------------------------------------------------
#
# Project created by QtCreator 2018-01-12T09:27:25
#
#-------------------------------------------------

SOURCES += \
    $$PWD/DapUIAnimationScreenAbstract.cpp\
    $$PWD/DapUIAnimationScreen.cpp\
    $$PWD/DapUiMainWindow.cpp\
    $$PWD/DapUiDialog.cpp\
    $$PWD/DapUiScreen.cpp\
    $$PWD/schedule.cpp\
    $$PWD/schedules.cpp\
    $$PWD/SideBar.cpp


HEADERS  += \
    $$PWD/DapUIAnimationScreenAbstract.h\
    $$PWD/DapUIAnimationScreen.h\
    $$PWD/DapUiDialog.h\
    $$PWD/DapUiMainWindow.h\
    $$PWD/DapUiScreen.h\
    $$PWD/UiScaling.h \
    $$PWD/schedule.h\
    $$PWD/schedules.h\
    $$PWD/SideBar.h

include(auxiliary/auxiliary.pri)
include(controls/controls.pri)
include(screens/screens.pri)

INCLUDEPATH += $$PWD

DISTFILES +=
