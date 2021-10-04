/* INCLUDES */
#include "chooseservermodel.h"
#include "ui/chooseserver.h"

#include <QBoxLayout>
#include <QTimer>
#include <QDebug>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

ChooseServerModel::ChooseServerModel (QWidget *parent)
  : ModelBase (parent)
  , _hook (false)
{

}

ChooseServerModel::~ChooseServerModel()
{

}

/********************************************
 * PUBLIC METHODS
 *******************************************/

void ChooseServerModel::setModel (QAbstractListModel *model, ChooseServer *cs)
{
  /* store and invoke setup */
  m_model = model;
  m_cs    = cs;
  QTimer::singleShot (0, this, &ChooseServerModel::slotSetup);
}

QAbstractListModel *ChooseServerModel::model() const
{
  return m_model;
}

/********************************************
 * PUBLIC SLOTS
 *******************************************/

void ChooseServerModel::slotSetup()
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
      auto item = new KelGuiRadio;
      m_list << item;

      /* setup */
      item->setText (text);
      item->setSeparator (i + 1 < size);
      lay->addWidget (item);

      /* signals */
      connect (item, &KelGuiRadio::toggled,
               this, &ChooseServerModel::slotToggled);
    }

  QSpacerItem *sp = new QSpacerItem (20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
  lay->addItem (sp);

  if (!m_list.isEmpty())
    emit filled();
}

void ChooseServerModel::slotToggled(bool checked)
{
//  if(_hook)
//    {
//      qDebug() << __PRETTY_FUNCTION__ << "skipped";
//      return;
//    }

  /* get sender radio */
  auto s = qobject_cast<KelGuiRadio*> (sender());
  if (!s || checked == false)
    return;

  /* get it's index */
  int index = m_list.indexOf (s);
  if (index == -1)
    return;

//  /* uncheck other radio's */
//  _hook = true;
//  int counter = 0, found = 0;

//  for (auto i = m_list.begin(), e = m_list.end(); i != e; i++)
//    {
//      if (*i != s)
//        {
//          (*i)->setChecked (false);
//          counter++;
//        }
//    }

//  qDebug() << __PRETTY_FUNCTION__ << "changed" << counter << "found" << found;

//  _hook = false;

  /* send selection event signal */
  emit m_cs->sigSelect (index, s->text());
}

/*-----------------------------------------*/
