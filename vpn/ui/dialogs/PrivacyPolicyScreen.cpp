#include "PrivacyPolicyScreen.h"

const QString PrivacyPolicyScreen::SCREEN_NAME = "PrivacyPolicy";

PrivacyPolicyScreen::PrivacyPolicyScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::PrivacyPolicyScreen>();
    AdaptiveScreen::initScreen(this);
}
QString PrivacyPolicyScreen::screenName()
{
    return PrivacyPolicyScreen::SCREEN_NAME;
}

void PrivacyPolicyScreen::initVariantUi(QWidget *a_widget)
{
#ifndef Q_OS_ANDROID
    Q_UNUSED(a_widget)
#else
    QScrollArea   *scrPrivacy = a_widget->findChild<QScrollArea*>("scrPrivacy");  Q_ASSERT(scrPrivacy);
    QScroller::grabGesture(scrPrivacy->viewport(), QScroller::LeftMouseButtonGesture);
#endif
}
