#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QStackedWidget>
#include <QStackedLayout>
#include <QSizePolicy>
#include "SideBar.h"
#include "ScreenSettings.h"

class MainMenu : public QObject
{
    Q_OBJECT
public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu(){}
    SideBar *sideBar(){return m_sideBar;}
    
public slots:
    void resizePage(int a_height, int a_width);
private:
    SideBar* m_sideBar;
    enum pages {Profile, Settings};
    QStackedWidget *m_pages;

    ScreenSettings *m_scrSettings;
};

#endif // MAINMENU_H
