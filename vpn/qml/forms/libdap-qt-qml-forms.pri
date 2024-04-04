INCLUDEPATH += $$PWD

win32: LIBS += -lgdi32
unix: !mac: !android: LIBS += -lX11 -lXrandr
macos:LIBS += -framework AppKit

INCLUDEPATH += $$PWD

include(../../../../config.pri)

RESOURCES += \
    $$PWD/dapqml-forms-asset/$$BRAND/dapqml-forms-asset.qrc \
    $$PWD/dapqml-forms.qrc

HEADERS += \
    $$PWD/dapqml-forms/doc-group-qml-forms.h

SOURCES += \
