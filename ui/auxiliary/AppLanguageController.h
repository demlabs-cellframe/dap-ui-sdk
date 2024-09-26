#ifndef APPLANGUAGECONTROLLER_H
#define APPLANGUAGECONTROLLER_H

#include <QTranslator>
#include <QCoreApplication>
#include "defines.h"
#include "LanguagesModel.h"

class AppLanguageController : public QObject
{
    Q_OBJECT
public:
    explicit AppLanguageController(QList<QLocale::Language> a_languages, QObject* a_parent = nullptr);

    LanguagesModel* languagesModel();

    QLocale::Language currentLanguage() const;

public slots:
    void setCurrentLanguage(QLocale::Language a_language);
    void setCurrentLanguageIndex(int a_index);

signals:
    void appRetranslated();

private:
    void retranslateApp(QLocale::Language a_language);

    QTranslator m_qtLanguageTranslator;

    const QString SETTING_LOCALE{"language"};
    const QString SETTING_SYS_LOCALE{"SysLanguage"};

    LanguagesModel m_languagesModel;
    QLocale::Language m_language;
    QLocale::Language m_defaultLanguage{QLocale::English};
};

QDataStream &operator<<(QDataStream &a_outStream, QLocale::Language a_serialKeyData);
QDataStream &operator>>(QDataStream &a_inStream, QLocale::Language &a_serialKeyData);

#endif // APPLANGUAGECONTROLLER_H
