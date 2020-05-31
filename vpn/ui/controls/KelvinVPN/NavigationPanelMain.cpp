#include "NavigationPanelMain.h"

const QString NavigationPanelMain::SCREEN_NAME = "NavigationPanelMain";

NavigationPanelMain::NavigationPanelMain(QWidget *parent)
    : AdaptiveScreen(parent)
{
    this->create<Ui::NavigationPanelMain>();

    AdaptiveScreen::initScreen(this);
}

QString NavigationPanelMain::screenName()
{
    return NavigationPanelMain::SCREEN_NAME;
}

void NavigationPanelMain::initVariantUi(QWidget *a_widget)
{

    m_lblLeftLicense    = a_widget->findChild<QLabel*>(BTN_LEFTLICENSE);            Q_ASSERT(m_lblLeftLicense);
    m_btnFAQ            = a_widget->findChild<QPushButton*>(BTN_HELP);              Q_ASSERT(m_btnFAQ);
    m_btnBug            = a_widget->findChild<QPushButton*>(BTN_BUG);               Q_ASSERT(m_btnBug);
    m_btnSettings       = a_widget->findChild<QPushButton*>(BTN_SETTINGS);          Q_ASSERT(m_btnSettings);
#ifndef ANDROID
    m_btnBack           = a_widget->findChild<QPushButton*>(BTN_BACK);              Q_ASSERT(m_btnBack);
    m_btnSettings       = a_widget->findChild<QPushButton*>(BTN_SETTINGS);          Q_ASSERT(m_btnSettings);
    m_btnBack           = a_widget->findChild<QPushButton*>(BTN_BACK);              Q_ASSERT(m_btnBack);

    m_btnAccount        = a_widget->findChild<QPushButton*>(BTN_ACCOUNT);           Q_ASSERT(m_btnAccount);
    m_btnConnections     = a_widget->findChild<QPushButton*>(BTN_CONNECTION);        Q_ASSERT(m_btnConnections);
    m_btnLogOut         = a_widget->findChild<QPushButton*>(BTN_LOGOUT);            Q_ASSERT(m_btnLogOut);


    this->connectBtnToSignall(BTN_BACK, &NavigationPanelMain::goBack, a_widget);


    connect(m_btnConnections   , &QPushButton::clicked,this,&NavigationPanelMain::goToConnections);
   // connect(m_btnAccount     , &QPushButton::clicked,this,&NavigationPanelMain::goToAccount);
#else
    connect(m_btnSettings, &QPushButton::clicked,this,&NavigationPanelMain::goToSettings);
#endif
   // connect(m_btnSettings, &QPushButton::clicked,this,&NavigationPanelMain::goToSettings);
    connect(m_btnBug     , &QPushButton::clicked,this,&NavigationPanelMain::goToBugReport);
    connect(m_btnFAQ     , &QPushButton::clicked,this,&NavigationPanelMain::goToFAQ);

}

#ifndef ANDROID
void NavigationPanelMain::setBackState(bool a_backState)
{
    if (a_backState == m_backState)
        return;

    this->setChildProperties(BTN_BACK, Properties::VISIBLE, a_backState);
    m_backState = a_backState;
}

bool NavigationPanelMain::backState()
{
    return m_backState;
}
//*****************************************************************************
//Configuring buttons on the panel
//*****************************************************************************
void NavigationPanelMain::setActiveScreen(const QString &a_nameScreen)
{
    if(a_nameScreen=="SettingsScreen")
    {
        m_btnBack->setVisible(false);
        m_btnLogOut->setVisible(false);

        setCheckeButton(m_btnSettings);
        setVisibleButton(m_btnAccount);
        setVisibleButton(m_btnConnections);

        setVisibleButton(m_btnBug);
        setVisibleButton(m_btnFAQ);

        m_lblLeftLicense->setVisible(true);
        return;
    }

    if(a_nameScreen=="Main")
    {
        m_btnBack->setVisible(false);
        m_btnLogOut->setVisible(false);

        setCheckeButton(m_btnConnections);
        setVisibleButton(m_btnAccount);
        setVisibleButton(m_btnSettings);

        setVisibleButton(m_btnBug);
        setVisibleButton(m_btnFAQ);

        m_lblLeftLicense->setVisible(true);

        return;
    }

    if(a_nameScreen=="Account")
    {
        m_btnBack->setVisible(false);
        m_btnLogOut->setVisible(true);

        setCheckeButton(m_btnAccount);
        setVisibleButton(m_btnSettings);
        setVisibleButton(m_btnConnections);

        setVisibleButton(m_btnBug);
        setVisibleButton(m_btnFAQ);

        m_lblLeftLicense->setVisible(false);

        return;
    }

    if(a_nameScreen=="BugReport")
    {
        m_btnBack->setVisible(true);
        m_btnLogOut->setVisible(false);

        setVisibleButton(m_btnSettings,false);
        setVisibleButton(m_btnAccount,false);
        setVisibleButton(m_btnConnections,false);

        setVisibleButton(m_btnBug,false);
        setVisibleButton(m_btnFAQ,false);

        m_lblLeftLicense->setVisible(true);

        return;
    }

    if(a_nameScreen=="FAQ")
    {
        m_btnBack->setVisible(true);
        m_btnLogOut->setVisible(false);

        setVisibleButton(m_btnSettings,false);
        setVisibleButton(m_btnAccount,false);
        setVisibleButton(m_btnConnections,false);;

        setVisibleButton(m_btnBug,false);
        setVisibleButton(m_btnFAQ,false);

        m_lblLeftLicense->setVisible(true);

        return;
    }
}

#endif
void NavigationPanelMain::setVisibleButton(QPushButton *a_button, bool a_visible)
{
    a_button->setVisible(a_visible);
    a_button->setCheckable(false);
    a_button->setChecked(false);
    a_button->setEnabled(true);
}

void NavigationPanelMain::setCheckeButton(QPushButton *a_button)
{
    a_button->setVisible(true);
    a_button->setCheckable(true);
    a_button->setChecked(true);
    a_button->setEnabled(false);
}
