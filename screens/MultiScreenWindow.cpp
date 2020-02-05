#include "MultiScreenWindow.h"

MultiScreenWindow::MultiScreenWindow(MultiScreenAbstract* a_centralScreen /*= nullptr*/, QWidget *a_parent /*= nullptr*/)
    : QMainWindow(a_parent)
{
    if (!a_centralScreen)
        a_centralScreen = new DefaultMultiScreen(this);
}
