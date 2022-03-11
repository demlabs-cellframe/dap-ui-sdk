INCLUDEPATH += $$PWD

win32: LIBS += -lgdi32
unix: !mac: !android: LIBS += -lX11 -lXrandr
mac:LIBS += -framework AppKit

INCLUDEPATH += $$PWD

include(../config.pri)

RESOURCES += \
    $$PWD/dapqml-forms-asset.qrc \
    $$PWD/dapqml-forms.qrc

HEADERS += \
    $$PWD/dapqml-model/dapqmlmodelbugreports.h \
    $$PWD/dapqml-model/dapqmlmodelchooseserver.h \
    $$PWD/dapqml-model/dapqmlmodelserialhistory.h \
    $$PWD/dapqml-model/dapqmlmodelsettings.h

SOURCES += \
    $$PWD/dapqml-model/dapqmlmodelbugreports.cpp \
    $$PWD/dapqml-model/dapqmlmodelchooseserver.cpp \
    $$PWD/dapqml-model/dapqmlmodelserialhistory.cpp \
    $$PWD/dapqml-model/dapqmlmodelsettings.cpp
