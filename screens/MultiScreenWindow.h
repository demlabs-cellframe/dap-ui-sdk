#ifndef MULTISCREENWINDOW_H
#define MULTISCREENWINDOW_H

#include <QMainWindow>
#include "DefaultMultiScreen.h"
class MultiScreenWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MultiScreenWindow(MultiScreenAbstract* a_centralScreen = nullptr, QWidget *a_parent = nullptr);

signals:

};

#endif // MULTISCREENWINDOW_H
