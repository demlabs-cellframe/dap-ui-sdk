/* INCLUDES */
#include "DapSerialKeyHistory.h"
#include "DapDataLocal.h"
#include "DapSerialKeyData.h"

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapSerialKeyHistory::DapSerialKeyHistory (QObject *parent)
  : QObject(parent)
{
  qRegisterMetaType<DapSerialKeyHistoryItem> ("DapSerialHeyHistoryItem");
}

/********************************************
 * METHODS
 *******************************************/

const QList<DapSerialKeyHistoryItem> &DapSerialKeyHistory::list() const
{
  return m_list;
}

int DapSerialKeyHistory::size() const
{
  return m_list.size();
}

DapSerialKeyHistoryItem &DapSerialKeyHistory::value (int a_index)
{
  return m_list[a_index];
}

const DapSerialKeyHistoryItem &DapSerialKeyHistory::value (int a_index) const
{
  return m_list[a_index];
}

void DapSerialKeyHistory::load()
{
  DapDataLocal::instance()->loadFromSettings (TEXT_SERIAL_KEY_HISTORY, m_list);
}

void DapSerialKeyHistory::save()
{
  DapDataLocal::instance()->saveToSettings (TEXT_SERIAL_KEY_HISTORY, m_list);
}

/********************************************
 * SLOTS
 *******************************************/

void DapSerialKeyHistory::slotNewKey()
{
  /* vars */
  auto key  = DapDataLocal::instance()->serialKeyData()->serialKey();

  /* check if already exists */
  if (m_list.contains (key))
    return;

  /* store and save */
  m_list.prepend (key);
  save();

  /* notify */
  emit sigNewKey();
}

/*-----------------------------------------*/
