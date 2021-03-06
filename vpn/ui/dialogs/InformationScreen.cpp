#include "InformationScreen.h"

const QString InformationScreen::SCREEN_NAME = "Information";

InformationScreen::InformationScreen(QWidget *a_parent)
    : AdaptiveScreen(a_parent)
{
    create(m_ui);
    AdaptiveScreen::initScreen(this);
}
QString InformationScreen::screenName()
{
    return InformationScreen::SCREEN_NAME;
}

void InformationScreen::setText(const QString &a_text)
{
    m_ui->lblText->setText(a_text);
}

void InformationScreen::initVariantUi(QWidget *a_widget)
{
#ifndef ANDROID
    Q_UNUSED(a_widget)
#else
    QScroller::grabGesture(m_ui->scrArea->viewport(), QScroller::LeftMouseButtonGesture);

    //TODO:
    m_ui->lblCaption->hide();
#endif
}
void InformationScreen::setTextCaption(const QString &a_text)
{
    m_ui->lblCaption->setText(a_text);
}
void InformationScreen::setVisibleCaption(bool a_visible)
{
    m_ui->lblCaption->setVisible(a_visible);
}
