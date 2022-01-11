/* INCLUDES */
#include "dapqmlstyle.h"
#include "style/qssmap.h"
#include <QDebug>

/* VARS */
static DapQmlStyle *s_globalSignal = nullptr;
static bool s_gsHook = false;
static thread_local double s_screenWidth = 428, s_screenHeight = 926;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlStyle::DapQmlStyle(QObject *parent)
  : QObject (parent)
  , m_item (nullptr)
{
  /* global instance */
  if (s_globalSignal == nullptr && s_gsHook == false)
    {
      s_gsHook        = true;
      s_globalSignal  = new DapQmlStyle;
      s_gsHook        = false;
    }

  /* signals */
  connect (this, &DapQmlStyle::qssChanged,
           this, &DapQmlStyle::_applyStyle,
           Qt::QueuedConnection);
  connect (this, &DapQmlStyle::itemChanged,
           this, &DapQmlStyle::_applyStyle,
           Qt::QueuedConnection);

  if (s_globalSignal)
    connect (s_globalSignal, &DapQmlStyle::resized,
             this, &DapQmlStyle::_resized,
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

void DapQmlStyle::windowResized(int a_width, int a_height)
{
  if (s_globalSignal)
    emit s_globalSignal->resized (a_width, a_height);
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
          /* set scale */
          if (it.key() == "scaledRect")
            {
//              auto value  = it.value(); // .value<Style::Scaled> ();
//              auto object = qvariant_cast<QObject*> (value);
//              auto scaled = qobject_cast<Style::Scaled*> (object);
//              if (scaled)
//                scaled->adjust (m_item, s_screenWidth, s_screenHeight);
              auto scaled  = it.value().value<Style::Scaled> ();
              scaled.adjust (m_item, s_screenWidth, s_screenHeight);
              continue;
            }

          /* set properties */
          m_item->setProperty (it.key().toStdString().c_str(), it.value());
        }
    }
}

void DapQmlStyle::_resized(int a_width, int a_height)
{
  //qDebug() << __PRETTY_FUNCTION__ << a_width << a_height;
  s_screenWidth   = a_width;
  s_screenHeight  = a_height;
  _applyStyle();
}

/*-----------------------------------------*/
