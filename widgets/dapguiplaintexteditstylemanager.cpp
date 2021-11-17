/* INCLUDES */
#include "dapguiplaintexteditstylemanager.h"
#include "dapguiplaintextedit.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapGuiPlainTextEditStyleManager::DapGuiPlainTextEditStyleManager(QWidget *parent)
  : DapGuiStyleManager (parent)
{
  /* reconnect into different slot */
  if (this != &_signal())
    {
      disconnect (&_signal(), &DapGuiStyleManager::forceStyleUpdate,
                  this, &DapGuiPlainTextEditStyleManager::forcedStyleUpdate);
      connect (&_signal(), &DapGuiStyleManager::forceStyleUpdate,
               this, &DapGuiPlainTextEditStyleManager::forcedPteStyleUpdate);
    }
}

DapGuiPlainTextEditStyleManager::DapGuiPlainTextEditStyleManager(DapGuiPlainTextEditStyleManager &&src)
  : DapGuiStyleManager ((DapGuiStyleManager &&)src)
{
  /* reconnect into different slot */
  if (this != &_signal())
    {
      disconnect (&_signal(), &DapGuiStyleManager::forceStyleUpdate,
                  this, &DapGuiPlainTextEditStyleManager::forcedStyleUpdate);
      connect (&_signal(), &DapGuiStyleManager::forceStyleUpdate,
               this, &DapGuiPlainTextEditStyleManager::forcedPteStyleUpdate);
    }
}

DapGuiPlainTextEditStyleManager::~DapGuiPlainTextEditStyleManager()
{

}

/********************************************
 * METHODS
 *******************************************/

void DapGuiPlainTextEditStyleManager::updatePlaceholderStyle (bool a_isEmpty)
{
  auto p  = qobject_cast<DapGuiPlainTextEdit *> (m_widget);
  if (p)
    {
      auto s  =
          (!a_isEmpty)
          ? QString() + styleByClassName ("cpte-text")
          : QString() + styleByClassName ("cpte-text") + styleByClassName ("cpte-text-placeholder");
      p->editWidget()->setStyleSheet (s);
    }
}

/********************************************
 * SLOTS
 *******************************************/

void DapGuiPlainTextEditStyleManager::forcedPteStyleUpdate()
{
  auto p  = qobject_cast<DapGuiPlainTextEdit *> (m_widget);
  if (p)
    updatePlaceholderStyle (p->plainText().isEmpty());
}

/*-----------------------------------------*/
