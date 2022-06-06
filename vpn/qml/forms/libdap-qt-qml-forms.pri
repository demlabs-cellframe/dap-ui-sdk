INCLUDEPATH += $$PWD

win32: LIBS += -lgdi32
unix: !mac: !android: LIBS += -lX11 -lXrandr
mac:LIBS += -framework AppKit

INCLUDEPATH += $$PWD

include(../config.pri)

RESOURCES += \
    $$PWD/dapqml-forms-asset/$$BRAND/dapqml-forms-asset.qrc \
    $$PWD/dapqml-forms.qrc

HEADERS += \
    $$PWD/dapqml-forms/doc-group-QmlForms.h \
    $$PWD/dapqml-model/dapqmlmodelbugreports.h \
    $$PWD/dapqml-model/dapqmlmodelchooseserver.h \
    $$PWD/dapqml-model/dapqmlmodelserialhistory.h \
    $$PWD/dapqml-model/dapqmlmodelsettings.h

SOURCES += \
    $$PWD/dapqml-model/dapqmlmodelbugreports.cpp \
    $$PWD/dapqml-model/dapqmlmodelchooseserver.cpp \
    $$PWD/dapqml-model/dapqmlmodelserialhistory.cpp \
    $$PWD/dapqml-model/dapqmlmodelsettings.cpp

DISTFILES += \
    $$PWD/dapqml-forms/QuiBugReportForm.md \
    $$PWD/dapqml-forms/QuiBugReportHistoryForm.md \
    $$PWD/dapqml-forms/QuiChooseLanguageForm.md \
    $$PWD/dapqml-forms/QuiChooseServerForm.md \
    $$PWD/dapqml-forms/QuiChooseThemeForm.md \
    $$PWD/dapqml-forms/QuiDashboardForm.md \
    $$PWD/dapqml-forms/QuiDocumentViewer.md \
    $$PWD/dapqml-forms/QuiLoginForm.md \
    $$PWD/dapqml-forms/QuiPurchaseForm.md \
    $$PWD/dapqml-forms/QuiPurchaseHistoryForm.md \
    $$PWD/dapqml-forms/QuiSettingsForm.md \
    $$PWD/dapqml-forms/QuiSplashScreenForm.md \
    $$PWD/dapqml-forms/QuiStatisticsForm.md
