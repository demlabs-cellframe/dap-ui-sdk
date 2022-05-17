/* INCLUDES */
#include "menufooter.h"
#include "ui_menufooter.h"
#include <QApplication>
#include <QScreen>
#include <QDebug>
#include "ui/helper/auxiliary/UiScaling.h"

/* DEFS */
#define HEIGHT (172)

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

MenuFooter::MenuFooter (QWidget *parent) :
    BaseForm (parent),
  ui (new Ui::MenuFooter),
  m_state (BS_CONNECTION),
  m_lock (false),
  m_active (false),
  position ({0,0,0})
{
  /* setup */
  ui->setupUi (this);
  setVisible (false);
  ui->progress->setVisible (false);
  setCssStyle ("footer backgroundcolor");

  qRegisterMetaType<ButtonState> ("ButtonState");
  qRegisterMetaType<MenuFooter::ButtonState> ("MenuFooter::ButtonState");
  //setWindowFlags(Qt::WindowStaysOnTopHint);

  /* setup animation */
  m_posAnim  = new QPropertyAnimation (this, "pos");
  m_posAnim->setDuration (100);

  /* fill state map */
  m_statesMap =
  {
    {ui->btnAccount,    BS_ACCAUNT},
    {ui->btnSettings,   BS_SETTINGS},
    {ui->btnConnection, BS_CONNECTION},
  };

  /* signals */
  connect (ui->btnAccount, &QPushButton::toggled,
           this, &MenuFooter::slotButtonToggled);
  connect (ui->btnSettings, &QPushButton::toggled,
           this, &MenuFooter::slotButtonToggled);
  connect (ui->btnConnection, &QPushButton::toggled,
           this, &MenuFooter::slotButtonToggled);

  /* move to bottom */
  auto p = dynamic_cast<QWidget *> (parent);
  position.screenHeight = p->height();
  position.height       = UiScaling::pointsToPixels (HEIGHT, UiScaling::getNativDPI());
  position.y            = position.screenHeight - position.height;
  move (0, position.screenHeight);
  QMetaObject::invokeMethod (this, &MenuFooter::slotMoveToBottom, Qt::QueuedConnection);
}

MenuFooter::~MenuFooter()
{
  delete ui;
}

/********************************************
 * METHODS
 *******************************************/

bool MenuFooter::active() const
{
  return m_active;
}

void MenuFooter::setActive(bool newActive)
{
  m_active = newActive;
  QMetaObject::invokeMethod (this, &MenuFooter::slotMoveToBottom, Qt::QueuedConnection);
}

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
  if (state != BS_NONE)
    m_state = state;

  /* setup buttons */
  ui->btnSettings->setChecked (m_state == BS_SETTINGS);
  ui->btnConnection->setChecked (m_state == BS_CONNECTION);
  ui->btnAccount->setChecked (m_state == BS_ACCAUNT);

  m_lock = false;
}

void MenuFooter::slotMoveToBottom()
{
  auto p = dynamic_cast<QWidget *> (parent());
  raise();

  if (p == nullptr)
    return;

  position.screenHeight = p->height();
  position.height       = UiScaling::pointsToPixels (HEIGHT, UiScaling::getNativDPI());
  position.y            = position.screenHeight - position.height;

//  if ((y() != position.y)
//      && (y() != position.y))
//    move (0, position.y);
  //resize (p->width(), position.height);

  setMinimumSize (p->width(), position.height);
  setMaximumSize (p->width(), position.height);
  ui->Background->setMinimumSize (p->width(), position.height);
  ui->Background->setMaximumSize (p->width(), position.height);

  _setAnimByState();
  _startAnim();

  qDebug() << __PRETTY_FUNCTION__ << "y:" << position.y << ",screen:" << p->width() << p->height() << height();
}

void MenuFooter::slotResizeEvent(QSize a_oldSize, QSize a_newSize)
{
  qDebug() << __PRETTY_FUNCTION__ << a_oldSize << a_newSize;
  Q_UNUSED (a_oldSize)
  Q_UNUSED (a_newSize)
  slotMoveToBottom();
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

void MenuFooter::_setAnimByState()
{
  m_posAnim->setStartValue (pos());
  m_posAnim->setEndValue (
      m_active
      ? QPoint (0, position.y)
      : QPoint (0, position.screenHeight)
        );
}

void MenuFooter::_startAnim()
{
  m_posAnim->start();
}

void MenuFooter::slotProgramSettingButtonPress()
{
    qDebug() << "slotProgramSettingButtonPress";
    ui->btnSettings->toggle();
}

void MenuFooter::slotProgramStatisticButtonPress()
{
    qDebug() << "slotProgramStatisticButtonPress";
    ui->btnAccount->toggle();
}

void MenuFooter::slotProgramMiddleButtonPress()
{
    ui->btnConnection->toggle();
}

/*-----------------------------------------*/
