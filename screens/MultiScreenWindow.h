#ifndef MULTISCREENWINDOW_H
#define MULTISCREENWINDOW_H

#include <QMainWindow>
#include "DefaultMultiScreen.h"
class MultiScreenWindow : public QMainWindow
{
    Q_OBJECT
    Q_PROPERTY(QString activeScreen READ activeScreen WRITE activateScreen)

    using QMainWindow::setCentralWidget; //disable access to setCentralWidget.
public:
    explicit MultiScreenWindow(MultiScreenAbstract* a_centralScreen = nullptr, QWidget *a_parent = nullptr);

    void activateScreen (const QString& a_screenName);
    QString activeScreen();

    MultiScreenAbstract* centralScreen();
    MultiScreenAbstract* m_centralScreen;

private:
    QString m_activeScreen;
};

#endif // MULTISCREENWINDOW_H
