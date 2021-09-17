/* INCLUDES */
#include "menufooter.h"
#include "ui_menufooter.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

MenuFooter::MenuFooter (QWidget *parent) :
    BaseForm (parent),
  ui (new Ui::MenuFooter),
  m_state (BS_NONE),
  m_lock (false)
{
  /* setup */
  ui->setupUi (this);
  this->setVisible (false);
  setCssStyle ("footer");
  qRegisterMetaType<ButtonState> ("ButtonState");
  qRegisterMetaType<MenuFooter::ButtonState> ("MenuFooter::ButtonState");
  //setWindowFlags(Qt::WindowStaysOnTopHint);

  /* fill state map */
  m_statesMap =
  {
    {ui->btnAccaunt,    BS_ACCAUNT},
    {ui->btnSettings,   BS_SETTINGS},
    {ui->btnConnection, BS_CONNECTION},
  };

  /* signals */
  connect (ui->btnAccaunt, &QPushButton::toggled,
           this, &MenuFooter::slotButtonToggled);
  connect (ui->btnSettings, &QPushButton::toggled,
           this, &MenuFooter::slotButtonToggled);
  connect (ui->btnConnection, &QPushButton::toggled,
           this, &MenuFooter::slotButtonToggled);

  /* move to bottom */
  slotMoveToBottom();
}

MenuFooter::~MenuFooter()
{
  delete ui;
}

/********************************************
 * METHODS
 *******************************************/

MenuFooter::ButtonState MenuFooter::state() const
{
    return m_state;
}

/********************************************
 * SLOTS
 *******************************************/

void MenuFooter::slotSetProgressValue (int value)
{
  ui->progress->setValue (value);
}

void MenuFooter::slotSetProgressRange (int minimum, int maximum)
{
  ui->progress->setRange (minimum, maximum);
}

void MenuFooter::slotSetButtonState (MenuFooter::ButtonState state)
{
  m_lock = true;

  /* store state */
  m_state = state;

  /* setup buttons */
  ui->btnSettings->setChecked (m_state == BS_SETTINGS);
  ui->btnConnection->setChecked (m_state == BS_CONNECTION);
  ui->btnAccaunt->setChecked (m_state == BS_ACCAUNT);

  m_lock = false;
}

void MenuFooter::slotMoveToBottom()
{
  raise();
  if (auto p = dynamic_cast<QWidget *> (parent()))
    move (0, p->height() - height());
}

void MenuFooter::slotButtonToggled (bool checked)
{
  /* ignore if locked (prevent recursion) */
  if (m_lock)
    return;

  /* if unset */
  if (checked == false)
    return slotSetButtonState (BS_NONE);

  /* if set, find in map and setup */
  auto state = m_statesMap.value (sender(), BS_NONE);
  slotSetButtonState (state);
  emit sigStateChanged (state);
}

/*-----------------------------------------*/
