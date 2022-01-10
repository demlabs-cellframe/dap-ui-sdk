INCLUDEPATH += $$PWD

win32: LIBS += -lgdi32
unix: !mac: !android: LIBS += -lX11 -lXrandr
mac:LIBS += -framework AppKit

INCLUDEPATH += $$PWD

include(../config.pri)

RESOURCES += \
    $$PWD/dapqml-widgets-asset/brand/$$BRAND/dapqml-widgets-asset-brand.qrc \
    $$PWD/dapqml-widgets-asset/brand/$$BRAND/dapqml-widgets-style-brand.qrc \
    $$PWD/dapqml-widgets-asset/nonbrand/nonbrandAsset.qrc \
    $$PWD/dapqml-widgets.qrc

HEADERS += \
    $$PWD/dapqml-widgets/dapqmlstyle.h \
    $$PWD/dapqml-widgets/style/qssitem.h \
    $$PWD/dapqml-widgets/style/qssmap.h \
    $$PWD/dapqml-widgets/style/qssparser.h \
    $$PWD/dapqml-widgets/style/scaled.h

SOURCES += \
    $$PWD/dapqml-widgets/dapqmlstyle.cpp \
    $$PWD/dapqml-widgets/style/qssitem.cpp \
    $$PWD/dapqml-widgets/style/qssmap.cpp \
    $$PWD/dapqml-widgets/style/qssparser.cpp \
    $$PWD/dapqml-widgets/style/scaled.cpp
