#include "VersionLabel.h"

VersionLabel::VersionLabel(QWidget *a_parent)
    :QLabel (a_parent)
{

}

void VersionLabel::setText(const QString &a_text)
{

    QLabel::setText(a_text.arg(DAP_VERSION).arg(QDate(QLocale("en_US").toDate(QString(__DATE__).simplified(), "MMM d yyyy")).toString(Qt::ISODate)));

}

QString VersionLabel::getVersion(){

    return QString("%1 %2").arg(DAP_VERSION).arg(QDate(QLocale("en_US").toDate(QString(__DATE__).simplified(), "MMM d yyyy")).toString(Qt::ISODate));
}


