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

    template <typename T>
    T* screen();

    template <typename T>
    T* activateScreen();

signals:
    void screenChangingFinished();

private:
    QString m_activeScreen;
};


template <typename T>
T* MultiScreenWindow::screen(){
    return this->centralScreen()->subScreen<T>();
}

template <typename T>
T* MultiScreenWindow::activateScreen()
{
    return this->centralScreen()->activateScreen<T>();
}


#endif // MULTISCREENWINDOW_H
