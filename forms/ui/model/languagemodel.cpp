/* INCLUDES */
#include "languagemodel.h"

/* DEFS */
struct _Language
{
  QString name;
};

/* VARS */
static QList<_Language> s_langauges =
{
  {"English"},
  //{"Русский"},
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

LanguageModel::LanguageModel (QWidget *parent)
  : ModelBase (parent)
{

}

LanguageModel::~LanguageModel()
{

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

  /* create new buttons */
  for (auto i = s_langauges.begin(), e = s_langauges.end(); i != e; i++)
    {
      /* get data */
      QString text = (*i).name;

      /* create item */
      auto item = new KelGuiRadio;
      m_list << item;
      item->setText (text);
      lay->addWidget (item);
    }

  QSpacerItem *sp = new QSpacerItem (20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
  lay->addItem (sp);
}

/*-----------------------------------------*/
