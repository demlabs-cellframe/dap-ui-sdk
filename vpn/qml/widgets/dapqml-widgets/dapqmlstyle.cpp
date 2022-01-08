/* INCLUDES */
#include "dapqmlstyle.h"
#include "style/qssmap.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlStyle::DapQmlStyle(QObject *parent)
  : QObject (parent)
{
  /* signals */
  connect (this, &DapQmlStyle::qssChanged,
           this, &DapQmlStyle::_applyStyle,
           Qt::QueuedConnection);
  connect (this, &DapQmlStyle::itemChanged,
           this, &DapQmlStyle::_applyStyle,
           Qt::QueuedConnection);
}

/********************************************
 * METHODS
 *******************************************/

QString DapQmlStyle::getQss() const
{
  return m_qss;
}

void DapQmlStyle::setQss(const QString &a_newQss)
{
  m_qss = a_newQss;
  emit qssChanged();
}

QObject *DapQmlStyle::getItem() const
{
  return m_item;
}

void DapQmlStyle::setItem (QObject *a_newItem)
{
  m_item = a_newItem;
  emit itemChanged();
}

void DapQmlStyle::setup(const QString &styleSheet)
{
  Style::QssMap::setup (styleSheet);
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlStyle::_applyStyle()
{
  /* check ptr */
  if (m_item == nullptr)
    return;

  /* get items by style list */
  auto items  = Style::QssMap::items (m_qss);

  /* cycle thru all items */
  for (auto i = items.cbegin(), e = items.cend(); i != e; i++)
    {
      /* ge item */
      auto item = *i;

      /* cycle thru all item properties */
      for (auto it = item->cbegin(), en = item->cend(); it != en; it++)
        {
          /* set properties */
          m_item->setProperty (it.key().toStdString().c_str(), it.value());
        }
    }
}

/*-----------------------------------------*/
