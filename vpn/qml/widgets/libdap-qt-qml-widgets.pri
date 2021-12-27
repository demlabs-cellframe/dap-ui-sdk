INCLUDEPATH += $$PWD

win32: LIBS += -lgdi32
unix: !mac: !android: LIBS += -lX11 -lXrandr
mac:LIBS += -framework AppKit

INCLUDEPATH += $$PWD

include(../config.pri)

RESOURCES += \
    $$PWD/dapqml-widgets-asset/brand/$$BRAND/dapqml-widgets-asset-brand.qrc \
    $$PWD/dapqml-widgets-asset/nonbrand/nonbrandAsset.qrc \
    $$PWD/dapqml-widgets.qrc
