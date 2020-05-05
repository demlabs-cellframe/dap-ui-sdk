#include "SettingsMoreMenu.h"
#include "ui_settings_more_menu.h"

SettingsMoreMenu::SettingsMoreMenu(QWidget *parent) :
    QMenu(parent),
    ui(new Ui::SettingsMoreMenu)
{
    qDebug() << "[SettingsMoreMenu] Initializing menu \"More\"";

    ui->setupUi(this);

    //buttons:
    QPushButton *btRestore  = this->findChild<QPushButton*>("btRestore");
    QPushButton *btEdit     = this->findChild<QPushButton*>("btEdit");
    QPushButton *btRemove   = this->findChild<QPushButton*>("btRemove");

    //connections:
    connect(btRestore   , SIGNAL(clicked()), SLOT(restore()));
    connect(btRestore   , SIGNAL(clicked()), SLOT(hide()));
    connect(btEdit      , &QPushButton::clicked, [this] { emit editClicked(curentServer()); });
    connect(btEdit      , SIGNAL(clicked()), SLOT(hide()));
    connect(btRemove    , SIGNAL(clicked()), SLOT(remove()) );
    connect(btRemove    , SIGNAL(clicked()), SLOT(hide()));

}

SettingsMoreMenu::~SettingsMoreMenu()
{
    delete ui;
}

void SettingsMoreMenu::setCurentServer(const DapServerInfo *a_server)
{
    m_curentServer = a_server;
}

const DapServerInfo *SettingsMoreMenu::curentServer() const
{
    return m_curentServer;
}

void SettingsMoreMenu::restore()
{
    qDebug()<<"[SettingsMoreMenu] Action \"Restore\" pressed for server " << m_curentServer->name;
}

void SettingsMoreMenu::edit()
{
    emit editClicked(curentServer());
}

void SettingsMoreMenu::remove()
{
    qDebug()<<"[SettingsMoreMenu] Action \"remove\" pressed for server " << m_curentServer->name;
}

QToolButton *SettingsMoreMenu::parentButton() const
{
    return m_parentButton;
}

void SettingsMoreMenu::setParentButton(QToolButton *parentButton)
{
    m_parentButton = parentButton;
}

void SettingsMoreMenu::hideEvent(QHideEvent *)
{
    //make parent button unhover
    parentButton()->update();
}
