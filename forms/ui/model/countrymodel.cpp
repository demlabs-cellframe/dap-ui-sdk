/* INCLUDES */
#include "countrymodel.h"
#include "ui/country.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QBoxLayout>
#include <QTimer>
#include <QDebug>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

CountryModel::CountryModel (QWidget *parent)
  : ModelBase (parent)
  , _hook (false)
{

}

CountryModel::~CountryModel()
{

}

/********************************************
 * PUBLIC METHODS
 *******************************************/

void CountryModel::setCountryList (const QStringList &a_array, Country *a_cs)
{
  m_array = a_array;
  /* store and invoke setup */
  m_cs    = a_cs;
  QMetaObject::invokeMethod(this, &CountryModel::slotSetup, Qt::QueuedConnection);
}

void CountryModel::setCurrentCountry(const QString a_country)
{
  _hook = true;
  /* delete old buttons */
  foreach (auto *item, m_list)
    {
      if (item->text() != a_country)
        continue;
      item->setChecked (true);
      break;
    }
  _hook = false;
}

const QStringList &CountryModel::array() const
{
  return m_array;
}

/********************************************
 * PUBLIC SLOTS
 *******************************************/

void CountryModel::slotSetup()
{
  setupLayout();

  /* delete old buttons */
  foreach (auto *oldItem, m_list)
    {
      lay->removeWidget (oldItem);
      disconnect (oldItem, &DapGuiRadio::toggled,
                  this, &CountryModel::slotToggled);
      delete oldItem;
    }
  m_list.clear();

  /* create new buttons */
  int size  = m_array.size();
  for (int i = 0; i < size; i++)
    {
      /* get data */
      QString text = m_array.value(i);

      /* create item */
      auto item = new DapGuiRadio;
      m_list << item;

      /* setup */
      item->setText (text);
      item->setSeparator (i + 1 < size);
      item->setCssStyle ("choser-item");
      lay->addWidget (item);

      /* signals */
      connect (item, &DapGuiRadio::toggled,
               this, &CountryModel::slotToggled);
    }

//  QSpacerItem *sp = new QSpacerItem (20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
//  lay->addItem (sp);

  if (!m_list.isEmpty())
    emit filled();
}

void CountryModel::viewFilter(QStringList showItems)
{
//    QSet<QString> setA = QSet<QString>(showItems.begin(), showItems.end());
    QSet<QString> setA = showItems.toSet();
    foreach (auto *item, m_list)
        item->setVisible(setA.contains(item->text()));
}

void CountryModel::slotRetranslate()
{
  /* start cycling */
  int size = m_array.size();
  if (lay->count() < size)
    return;

  for (int i = 0; i < size; i++)
    {
      const auto &item  = m_array[i];
      auto button       = qobject_cast<DapGuiRadio*> (lay->itemAt(i)->widget());
      if (button == nullptr)
        continue;
      button->setText (item);
    }
}

void CountryModel::slotToggled(bool checked)
{
  if(_hook)
    {
      qDebug() << __PRETTY_FUNCTION__ << "skipped";
      return;
    }

  /* get sender radio */
  auto s = qobject_cast<DapGuiRadio*> (sender());
  if (!s || checked == false)
    return;

  /* get it's index */
  m_currentIndex = m_list.indexOf (s);
  if (m_currentIndex == -1)
    return;

  /* get current text */
  m_currentText = s->text();

  /* show selection and select */
  QTimer::singleShot(300, [&]
  {
    /* send selection event signal */
    emit m_cs->sigSelect (m_currentIndex, m_currentText);
  });
}

/*-----------------------------------------*/
