#ifndef APPLANGUAGECONTROLLER_H
#define APPLANGUAGECONTROLLER_H

#include <QTranslator>
#include <QApplication>
#include "defines.h"

class AppLanguageController : public QObject
{
    Q_OBJECT
    explicit AppLanguageController(QObject* a_parent = nullptr);
public:

    static AppLanguageController* instance();

    void setLocale(QLocale a_locale);

public slots:
    static void retranslateApp(QLocale a_locale = QLocale());

private:

    QTranslator m_qtLanguageTranslator;

    const QString SETTING_LOCALE{"language"};
    const QString SETTING_SYS_LOCALE{"SysLanguage"};

    QApplication* m_application;
};

#endif // APPLANGUAGECONTROLLER_H
