#include "MultiScreenWindow.h"

#include "StartScreen.h"

MultiScreenWindow::MultiScreenWindow(MultiScreenAbstract* a_centralScreen /*= nullptr*/, QWidget *a_parent /*= nullptr*/)
    : QMainWindow(a_parent)
{
    if (!a_centralScreen)
        a_centralScreen = new DefaultMultiScreen(this);

    m_centralScreen = a_centralScreen;
    setCentralWidget(a_centralScreen);
}

void MultiScreenWindow::activateScreen(const QString &a_screenName)
{
    if (a_screenName == StartScreen::SCREEN_NAME)
        m_centralScreen->activateScreen<StartScreen>();
    else
        return;

    m_activeScreen = a_screenName;
}

QString MultiScreenWindow::activeScreen()
{

}

MultiScreenAbstract *MultiScreenWindow::centralScreen()
{
    return m_centralScreen;
}
