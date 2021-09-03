/* INCLUDES */
#include "chooseservermodel.h"

#include <QBoxLayout>
#include <QTimer>

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

ChooseServerModel::ChooseServerModel(QWidget *parent)
  : ModelBase (parent)
{

}

ChooseServerModel::~ChooseServerModel()
{

}

/********************************************
 * PUBLIC METHODS
 *******************************************/

void ChooseServerModel::setModel(QAbstractListModel *model)
{
  /* store and invoke setup */
  m_model = model;
  QTimer::singleShot(0, this, &ChooseServerModel::slotSetup);
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
  if(m_model == nullptr)
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
      item->setText (text);
      lay->addWidget (item);
    }
}

/*-----------------------------------------*/
