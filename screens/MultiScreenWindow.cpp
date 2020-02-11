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

QString MultiScreenWindow::activeScreenName()
{
    return centralScreen()->activeScreenName();
}

MultiScreenAbstract *MultiScreenWindow::centralScreen()
{
    return m_centralScreen;
}
