/* INCLUDES */
#include "choosethememodel.h"
#include "ui/choosetheme.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QBoxLayout>
#include <QTimer>
#include <QDebug>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

ChooseThemeModel::ChooseThemeModel (QWidget *parent)
  : ModelBase (parent)
  , _hook (false)
{

}

ChooseThemeModel::~ChooseThemeModel()
{

}

/********************************************
 * PUBLIC METHODS
 *******************************************/

void ChooseThemeModel::setColorThemeArray (const QJsonArray &a_array, ChooseTheme *a_cs)
{
  /* parse and store languages */
  for (auto i = a_array.constBegin(), e = a_array.constEnd(); i != e; i++)
    {
      auto j  = i->toObject();
      auto k  = j.value("name").toString();
      m_array.append (k);
    }

  /* store and invoke setup */
  m_cs    = a_cs;
  QMetaObject::invokeMethod(this, &ChooseThemeModel::slotSetup, Qt::QueuedConnection);
}

void ChooseThemeModel::setCurrentColorTheme(const QString a_colorTheme)
{
  _hook = true;
  /* delete old buttons */
  foreach (auto *item, m_list)
    {
      if (item->text() != themeName (a_colorTheme))
        continue;
      item->setChecked (true);
      break;
    }
  _hook = false;
}

const QStringList &ChooseThemeModel::array() const
{
  return m_array;
}

QString ChooseThemeModel::themeName (const QString a_colorTheme, const QString a_transColorTheme)
{
  QMap<QString, QString> ctName =
  {
    { "Light Theme",  tr("Light Theme") },
    { "Dark Theme",   tr("Dark Theme") },
  };
  if (!a_colorTheme.isEmpty())
    return ctName.value(a_colorTheme, ctName.first());
  return ctName.key(a_transColorTheme, ctName.first());
}

/********************************************
 * PUBLIC SLOTS
 *******************************************/

void ChooseThemeModel::slotSetup()
{
  setupLayout();

  /* delete old buttons */
  foreach (auto *oldItem, m_list)
    {
      lay->removeWidget (oldItem);
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
      item->setText (themeName (text));
      item->setSeparator (i + 1 < size);
      item->setCssStyle ("choser-item");
      lay->addWidget (item);

      /* signals */
      connect (item, &DapGuiRadio::toggled,
               this, &ChooseThemeModel::slotToggled);
    }

  QSpacerItem *sp = new QSpacerItem (20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
  lay->addItem (sp);

  if (!m_list.isEmpty())
    emit filled();
}

void ChooseThemeModel::slotRetranslate()
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
      button->setText (themeName (item));
    }
}

void ChooseThemeModel::slotToggled(bool checked)
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
  m_currentText = themeName(QString(), s->text());

  /* show selection and select */
  QTimer::singleShot(300, [&]
  {
    /* send selection event signal */
    emit m_cs->sigSelect (m_currentIndex, m_currentText);
  });
}

/*-----------------------------------------*/
