#include "TermsOfUseScreen.h"

const QString TermsOfUseScreen::SCREEN_NAME = "TermsOfUse";

TermsOfUseScreen::TermsOfUseScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create<Ui::TermsOfUseScreen>();
    AdaptiveScreen::initScreen(this);
}
QString TermsOfUseScreen::screenName()
{
    return TermsOfUseScreen::SCREEN_NAME;
}

void TermsOfUseScreen::initVariantUi(QWidget *a_widget)
{
#ifndef Q_OS_ANDROID
    Q_UNUSED(a_widget)
#else
    QScrollArea   *scrTerms = a_widget->findChild<QScrollArea*>("scrTerms");  Q_ASSERT(scrTerms);
    QScroller::grabGesture(scrTerms->viewport(), QScroller::LeftMouseButtonGesture);
#endif
}
