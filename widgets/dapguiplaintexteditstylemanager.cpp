/* INCLUDES */
#include "dapguiplaintexteditstylemanager.h"
#include "dapguiplaintextedit.h"
#include <QTimer>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapGuiPlainTextEditStyleManager::DapGuiPlainTextEditStyleManager (QWidget *parent)
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

DapGuiPlainTextEditStyleManager::DapGuiPlainTextEditStyleManager (DapGuiPlainTextEditStyleManager &&src)
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
QPlainTextEdit
{
color: #04004E;
background-color: #F7F8FA;
font: 16px/25px;
font-weight: normal;
font-style: normal;
font-size: normal;
font-family: 'Lato';
}

QPlainTextEdit[plainText=""]
{
color: #9C9B9D;
}
 * METHODS
 *******************************************/

void DapGuiPlainTextEditStyleManager::updatePlaceholderStyle (bool a_isEmpty)
{
  auto p  = qobject_cast<DapGuiPlainTextEdit *> (m_widget);
  if (p)
    {
      auto s  =
        (!a_isEmpty)
        ? QString() + styleByClassName ("cpxte-text")
        : QString() + styleByClassName ("cpxte-text") + styleByClassName ("cpxte-text-placeholder");
      p->editWidget()->setStyleSheet (s);
      p->editWidget()->style()->polish (p->editWidget());
      s = QString (
             "#%1{%2}"
           ).arg (p->objectName(), styleByClassList (QString ("cpxte-holder nobackground noborder").split (" ")));
      //s   = styleByClassList (QString ("cpxte-holder nobackground noborder").split (" "));
      p->setStyleSheet (s);
    }
}

QString DapGuiPlainTextEditStyleManager::styleByClassName (const QString &className)
{
  return DapGuiStyleManager::styleByClassName (className);
}

/********************************************
 * SLOTS
 *******************************************/

void DapGuiPlainTextEditStyleManager::forcedPteStyleUpdate()
{
  QTimer::singleShot (500, this,
                      &DapGuiPlainTextEditStyleManager::slotTimedStyleUpdate);
}

void DapGuiPlainTextEditStyleManager::slotTimedStyleUpdate()
{
  auto p  = qobject_cast<DapGuiPlainTextEdit *> (m_widget);
  if (p == nullptr)
    return;

#ifdef Q_OS_ANDROID
  /* update for android */
  updatePlaceholderStyle (p->plainText().isEmpty());
#else
  /* update for pc */
  auto s = QString (
         "*{%1}\n"
         "*[plainText=\"\"]{%2}\n"
       ).arg (styleByClassName ("cpxte-text"), styleByClassName ("cpxte-text-placeholder"));
  p->editWidget()->setStyleSheet (s);
  p->editWidget()->style()->polish (p->editWidget());
  s = QString (
         "#%1{%2}"
       ).arg (p->objectName(), styleByClassList (QString ("cpxte-holder nobackground noborder").split (" ")));
  //s   = styleByClassList (QString ("cpxte-holder nobackground noborder").split (" "));
  p->setStyleSheet (s);
#endif // Q_OS_ANDROID
}

/*-----------------------------------------*/
