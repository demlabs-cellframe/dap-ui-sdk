/* INCLUDES */
#include "dapqmlstyle.h"
#include "style/qssmap.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlStyle::DapQmlStyle(QObject *parent)
  : QObject (parent)
  , m_item (nullptr)
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

double DapQmlStyle::centerHor (QObject *a_root, QObject *a_item)
{
  /* vars */
  double width[2];
  bool ok[2];

  /* get values */
  width[0] = a_root->property ("width").toDouble (&ok[0]);
  width[1] = a_item->property ("width").toDouble (&ok[1]);

  /* check issues */
  if (!ok[0] || !ok[1])
    return 0;

  /* calc result */
  return (width[0] / 2) - (width[1] / 2);
}

double DapQmlStyle::centerVer (QObject *a_root, QObject *a_item)
{
  /* vars */
  double height[2];
  bool ok[2];

  /* get values */
  height[0] = a_root->property ("height").toDouble (&ok[0]);
  height[1] = a_item->property ("height").toDouble (&ok[1]);

  /* check issues */
  if (!ok[0] || !ok[1])
    return 0;

  /* calc result */
  return (height[0] / 2) - (height[1] / 2);
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
