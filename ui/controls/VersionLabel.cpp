#include "VersionLabel.h"

VersionLabel::VersionLabel(QWidget *a_parent)
    :QLabel (a_parent)
{

}

void VersionLabel::setText(const QString &a_text)
{
    QLabel::setText(a_text.arg(DAP_VERSION).arg(__DATE__));
}
