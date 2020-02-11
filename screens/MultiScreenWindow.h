#ifndef MULTISCREENWINDOW_H
#define MULTISCREENWINDOW_H

#include <QMainWindow>
#include "DefaultMultiScreen.h"
class MultiScreenWindow : public QMainWindow
{
    Q_OBJECT

    using QMainWindow::setCentralWidget; //disable access to setCentralWidget.
public:
    explicit MultiScreenWindow(MultiScreenAbstract* a_centralScreen = nullptr, QWidget *a_parent = nullptr);

    MultiScreenAbstract* centralScreen();
    MultiScreenAbstract* m_centralScreen;

    QString activeScreenName();
private:
    QString m_activeScreen;
};

#endif // MULTISCREENWINDOW_H
