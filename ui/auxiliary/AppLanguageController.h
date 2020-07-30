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

    static Language appLanguage();
    static QString appLanguageString();


public slots:
    static void retranslateApp(Language a_language = Language::Undefined);

signals:
    void appRetranslated();

private:
    void setAppLanguage(QString a_language);

    static QString   languageToString(Language a_language);
    static Language stringToLanguage(const QString& a_Languagetr);

    QTranslator m_qtLanguageTranslator;
    const QString SETTING_NAME{"language"};

    const QMap<Language, QString> m_languageStrings {{Language::En, "en"}, {Language::Zh, "zh"}};

    QApplication* m_application;
};

#endif // APPLANGUAGECONTROLLER_H
