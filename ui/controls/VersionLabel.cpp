#include "VersionLabel.h"

VersionLabel::VersionLabel(QWidget *a_parent)
    :QLabel (a_parent)
{

}

void VersionLabel::setText(const QString &a_text)
{
    if(m_originalText != a_text)
        m_originalText = a_text;

    if(!m_standartVersionFormat)
        QLabel::setText(a_text.arg(DAP_VERSION).arg(QDate(QLocale("en_US").toDate(QString(__DATE__).simplified(), "MMM d yyyy")).toString(Qt::ISODate)));
    else
        QLabel::setText(a_text.arg(DAP_VERSION).arg(__DATE__));

}

QString VersionLabel::getVersion(){

    return QString("%1 %2").arg(DAP_VERSION).arg(QDate(QLocale("en_US").toDate(QString(__DATE__).simplified(), "MMM d yyyy")).toString(Qt::ISODate));
}

void VersionLabel::setStandartVersionFormat(bool a_format)
{
    if(m_standartVersionFormat != a_format)
    {
        m_standartVersionFormat = a_format;
        this->setText(m_originalText);
    }

}
