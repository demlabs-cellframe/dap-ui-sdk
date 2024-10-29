/* INCLUDES */
#include "languagemodel.h"
#include <QTimer>
/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

LanguageModel::LanguageModel (QWidget *parent)
  : ModelBase (parent)
  , m_model (nullptr)
  , _hook (false)
  , m_currentIndex (0)
{

}

LanguageModel::~LanguageModel()
{

}

/********************************************
 * PUBLIC METHODS
 *******************************************/

void LanguageModel::setModel(QAbstractListModel *model)
{
  /* store and invoke setup */
  m_model = model;
  QMetaObject::invokeMethod(this, &LanguageModel::slotSetup, Qt::QueuedConnection);
}

void LanguageModel::setCurrentLanguage(const QLocale::Language, const QString a_languageName)
{
  _hook = true;
  int i = 0;
  foreach (auto *item, m_list)
    {
      i++;
      if (item->text() != a_languageName)
        continue;
      item->setChecked (true);
      m_currentIndex  = i - 1;
      m_currentText   = a_languageName;
      break;
    }
  _hook = false;
}

QAbstractListModel *LanguageModel::model() const
{
  return m_model;
}

/********************************************
 * PUBLIC SLOTS
 *******************************************/

void LanguageModel::slotSetup()
{
  setupLayout();

  /* delete old buttons */
  foreach (auto *oldItem, m_list)
    {
      lay->removeWidget (oldItem);
      delete oldItem;
    }
  m_list.clear();

  /* check */
  if (m_model == nullptr)
    return;

  /* create new buttons */
  int size  = m_model->rowCount();
  for (int i = 0; i < size; i++)
    {
      /* get data */
      QString text = m_model->data (m_model->index (i)).toString();

      /* create item */
      auto item = new DapGuiRadio;
      m_list << item;
      item->setText (text);
      item->setSeparator (i + 1 < size);
      item->setCssStyle ("choser-item");
      if (text == m_currentText)
        item->setChecked (true);
      lay->addWidget (item);

      /* signals */
      connect (item, &DapGuiRadio::toggled,
               this, &LanguageModel::slotToggled);
    }

//  QSpacerItem *sp = new QSpacerItem (20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
//  lay->addItem (sp);
}

void LanguageModel::slotToggled(bool checked)
{
  /* check hook */
  if (_hook)
    return;

  /* get sender radio */
  auto s = qobject_cast<DapGuiRadio*> (sender());
  if (!s || checked == false)
    return;

  /* get it's index */
  m_currentIndex = m_list.indexOf (s);
  if (m_currentIndex == -1)
    return;

  /* get current server */
  m_currentText = s->text();

  /* show selection and select */
  QTimer::singleShot(300, [&]
  {
    /* send selection event signal */
    emit sigSelect (m_currentIndex, m_currentText);
  });
}

/*-----------------------------------------*/
