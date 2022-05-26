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
  , m_model (nullptr)
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
  QMetaObject::invokeMethod(this, &ChooseServerModel::slotSetup, Qt::QueuedConnection);
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
      auto item = new DapGuiRadioBase;
      m_list << item;

      /* setup */
      item->setText (text);
      item->setSeparator (i + 1 < size);
      item->setCssStyle ("choser-item");
      lay->addWidget (item);

      /* signals */
      connect (item, &DapGuiRadioBase::clicked,
               this, &ChooseServerModel::slotToggled);
    }

  QSpacerItem *sp = new QSpacerItem (20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
  lay->addItem (sp);

  if (!m_list.isEmpty())
    emit filled();
}

void ChooseServerModel::slotToggled()
{
//  if(_hook)
//    {
//      qDebug() << __PRETTY_FUNCTION__ << "skipped";
//      return;
//    }

  /* get sender radio */
  auto s  = qobject_cast<DapGuiRadioBase*> (sender());
  if (!s)
    return;

  /* get it's index */
  m_currentIndex = m_list.indexOf (s);
  if (m_currentIndex == -1)
    return;

  /* get current server */
  m_currentText = s->text();

  /* send selection event signal */
  emit m_cs->sigSelect (m_currentIndex, m_currentText);
}

void ChooseServerModel::setSelectedItem(QString name)
{
    if (m_model == nullptr)
      return;
    int listSize = m_list.length();
    for (int row=0; row<m_model->rowCount(); row++)
    {
        QString text = m_model->data(m_model->index(row)).toString();
        if (row < listSize)
            m_list.at(row)->setIndicatorChecked(text == name);
    }
}

/*-----------------------------------------*/
