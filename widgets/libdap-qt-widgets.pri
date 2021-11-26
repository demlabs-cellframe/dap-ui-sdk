INCLUDEPATH += $$PWD

win32: LIBS += -lgdi32
unix: !mac: !android: LIBS += -lX11 -lXrandr
mac:LIBS += -framework AppKit

SOURCES     += \
    $$PWD/dapguicommon.cpp

# add plugin definitions for QtDesigner
CONFIG(plugin) {
HEADERS     += \
    $$PWD/dapguibuttonplugin.h \
    $$PWD/dapguicheckboxplugin.h \
    $$PWD/dapguigraphicsviewplugin.h \
    $$PWD/dapguilabelplugin.h \
    $$PWD/dapguilineeditplugin.h \
    $$PWD/dapguiplaintexteditplugin.h \
    $$PWD/dapguipushbuttonplugin.h \
    $$PWD/dapguiradioplugin.h \
    $$PWD/dapguiseparatorplugin.h \
    $$PWD/dapguiswitchplugin.h \
    $$PWD/dapguitexteditplugin.h \
    $$PWD/dapguiwidgetplugin.h \
    $$PWD/dapvpnneuromorphic.h \
    $$PWD/widgetsdebugsettings.h

SOURCES     += \
    $$PWD/dapguibuttonplugin.cpp \
    $$PWD/dapguicheckboxplugin.cpp \
    $$PWD/dapguigraphicsviewplugin.cpp \
    $$PWD/dapguilabelplugin.cpp \
    $$PWD/dapguilineeditplugin.cpp \
    $$PWD/dapguiplaintexteditplugin.cpp \
    $$PWD/dapguipushbuttonplugin.cpp \
    $$PWD/dapguiradioplugin.cpp \
    $$PWD/dapguiseparatorplugin.cpp \
    $$PWD/dapguiswitchplugin.cpp \
    $$PWD/dapguitexteditplugin.cpp \
    $$PWD/dapguiwidgetplugin.cpp \
    $$PWD/dapvpnneuromorphic.cpp \
    $$PWD/widgetsdebugsettings.cpp

FORMS += \
    $$PWD/widgetsdebugsettings.ui
# message ($$HEADERS)
}

RESOURCES += \
    $$PWD/brand_widgets/$$BRAND/widgetsRes.qrc

DISTFILES += \
    $$PWD/DoxyLayout.xml \
    $$PWD/Doxyfile \
    $$PWD/README.md

INCLUDEPATH += $$PWD

include(dapguibutton.pri)
include(dapguicheckbox.pri)
include(dapguigraphicsview.pri)
include(dapguiplaintextedit.pri)
include(dapguipushbutton.pri)
include(dapguiradio.pri)
include(dapguiseparator.pri)
include(dapguiswitch.pri)
include(dapguitextedit.pri)
include(dapguilabel.pri)
include(dapguilineedit.pri)
include(dapguiwidget.pri)
include(style/dapguistylemanager.pri)

HEADERS += \
    $$PWD/dapguicommon.h
