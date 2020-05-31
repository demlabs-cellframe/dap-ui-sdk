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

    m_lblLastScreen     = a_widget->findChild<QLabel*>(LBL_NAME_LAST_SCREEN);       Q_ASSERT(m_lblLastScreen);
    m_lblLeftLicense    = a_widget->findChild<QLabel*>(LBL_LEFTLICENSE);            Q_ASSERT(m_lblLeftLicense);
    m_btnBug            = a_widget->findChild<QPushButton*>(BTN_BUG);               Q_ASSERT(m_btnBug);
    m_lblCaption        = a_widget->findChild<QLabel*>(LBL_CAPTION);                Q_ASSERT(m_lblCaption);
    m_btnBack           = a_widget->findChild<QPushButton*>(BTN_BACK);              Q_ASSERT(m_btnBack);
    m_btnSettings       = a_widget->findChild<QPushButton*>(BTN_SETTINGS);          Q_ASSERT(m_btnSettings);

    this->connectBtnToSignall(BTN_BACK, &NavigationPanelMain::goBack, a_widget);

    connect(m_btnSettings, &QPushButton::clicked,this,&NavigationPanelMain::goToSettings);
    connect(m_btnBug     , &QPushButton::clicked,this,&NavigationPanelMain::goToBugReport);


}

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
        m_lblCaption->setText("Settings");
        m_btnBack->setVisible(true);
        m_btnSettings->setVisible(false);
        m_btnBug->setVisible(false);
        m_lblLeftLicense->setVisible(true);
        m_lblLastScreen->setVisible(false);
        return;
    }

    if(a_nameScreen=="Main")
    {
        m_lblCaption->setText("DiveVPN");
        m_btnBack->setVisible(false);
        m_btnSettings->setVisible(true);
        m_btnBug->setVisible(true);
        m_lblLeftLicense->setVisible(false);
        m_lblLastScreen->setVisible(false);
        return;
    }

    if(a_nameScreen=="Account")
    {
        m_lblCaption->setText("Choose plan");
        m_btnBack->setVisible(true);
        m_btnSettings->setVisible(false);
        m_btnBug->setVisible(false);
        m_lblLeftLicense->setVisible(true);
        m_lblLastScreen->setVisible(true);
        return;
    }

    if(a_nameScreen=="BugReport")
    {
        m_lblCaption->setText("Bug report");
        m_btnBack->setVisible(true);
        m_btnSettings->setVisible(false);
        m_btnBug->setVisible(true);
        m_lblLeftLicense->setVisible(false);
        m_lblLastScreen->setVisible(false);
        return;
    }
}

