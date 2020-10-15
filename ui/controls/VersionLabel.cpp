#include "VersionLabel.h"
#include "AppController.h"

VersionLabel::VersionLabel(QWidget *a_parent)
    :QLabel (a_parent)
{

}

void VersionLabel::setText(const QString &a_text)
{
    if(AppController::instance()->languageController()->currentLanguage() == QLocale::Russian
            || AppController::instance()->languageController()->currentLanguage() == QLocale::English
            || AppController::instance()->languageController()->currentLanguage() == QLocale::French
            || AppController::instance()->languageController()->currentLanguage() == QLocale::Ukrainian)
    {
        if(AppController::instance()->languageController()->currentLanguage() == QLocale::English)
            QLabel::setText(a_text.arg(DAP_VERSION).arg(QLocale(AppController::instance()->languageController()->currentLanguage()).toString(QDate(QLocale("en_US").toDate(QString(__DATE__).simplified(), "MMM d yyyy")),"MMMM d, yyyy")));
        else
            QLabel::setText(a_text.arg(DAP_VERSION).arg(QLocale(AppController::instance()->languageController()->currentLanguage()).toString(QDate(QLocale("en_US").toDate(QString(__DATE__).simplified(), "MMM d yyyy")),"d MMMM yyyy")));
    }
    else
        QLabel::setText(a_text.arg(DAP_VERSION).arg(QDate(QLocale("en_US").toDate(QString(__DATE__).simplified(), "MMM d yyyy")).toString(Qt::ISODate)));
}


QString VersionLabel::getVersion()
{
    if(AppController::instance()->languageController()->currentLanguage() == QLocale::Russian
            || AppController::instance()->languageController()->currentLanguage() == QLocale::English
            || AppController::instance()->languageController()->currentLanguage() == QLocale::French
            || AppController::instance()->languageController()->currentLanguage() == QLocale::Ukrainian)
    {
        if(AppController::instance()->languageController()->currentLanguage() == QLocale::English)
            return QString("%1 %2").arg(DAP_VERSION).arg(QLocale(AppController::instance()->languageController()->currentLanguage()).toString(QDate(QLocale("en_US").toDate(QString(__DATE__).simplified(), "MMM d yyyy")),"MMMM d yyyy"));
        else
            return QString("%1 %2").arg(DAP_VERSION).arg(QLocale(AppController::instance()->languageController()->currentLanguage()).toString(QDate(QLocale("en_US").toDate(QString(__DATE__).simplified(), "MMM d yyyy")),"d MMMM yyyy"));
    }
    else
        return QString("%1 %2").arg(DAP_VERSION).arg(QDate(QLocale("en_US").toDate(QString(__DATE__).simplified(), "MMM d yyyy")).toString(Qt::ISODate));
}



