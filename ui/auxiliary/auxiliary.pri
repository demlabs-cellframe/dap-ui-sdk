HEADERS += \
    $$PWD/AppLanguageController.h \
    $$PWD/AppStyleSheetHandler.h \
    $$PWD/LanguagesModel.h \
    $$PWD/ScreenInfo.h \
    $$PWD/TariffItem.h \
    $$PWD/UiScaling.h \
    $$PWD/Utilz.h \
    $$PWD/defines.h

SOURCES += \
    $$PWD/AppLanguageController.cpp \
    $$PWD/AppStyleSheetHandler.cpp \
    $$PWD/LanguagesModel.cpp \
    $$PWD/ScreenInfo.cpp \
    $$PWD/TariffItem.cpp \
    $$PWD/UiScaling.cpp \
    $$PWD/Utilz.cpp

INCLUDEPATH += $$PWD

win32: LIBS += -lgdi32
unix: !mac: !android: LIBS += -lX11 -lXrandr
mac:LIBS += -framework AppKit
