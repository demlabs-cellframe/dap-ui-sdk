/* INCLUDES */
#include "dapqmlmodelserialhistory.h"

/* VARS */
static DapQmlModelSerialHistory *__inst = nullptr;
static QStringList *s_history           = nullptr;

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelSerialHistory::DapQmlModelSerialHistory(QObject *parent)
  : QAbstractTableModel{parent}
{
  s_history = new QStringList();
}

/********************************************
 * METHODS
 *******************************************/

DapQmlModelSerialHistory *DapQmlModelSerialHistory::instance()
{
  if (__inst == nullptr)
    __inst  = new DapQmlModelSerialHistory;
  return __inst;
}

QObject *DapQmlModelSerialHistory::singletonProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
  Q_UNUSED (engine)
  Q_UNUSED (scriptEngine)
  return DapQmlModelSerialHistory::instance();
}

int DapQmlModelSerialHistory::length() const
{
  return rowCount();
}

void DapQmlModelSerialHistory::attachAnimation(QObject *a_copyButton)
{
  new _DapQmlModelSerialHistoryCopyButtonAnimation (a_copyButton);
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelSerialHistory::rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return s_history->size();
}

int DapQmlModelSerialHistory::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return 1;
}

QVariant DapQmlModelSerialHistory::data(const QModelIndex &index, int role) const
{
  /* check index */
  if (!index.isValid() || role != 0)
    return QVariant();

  /* check boundaries */
  if (index.row() >= s_history->size())
    return QVariant();

  /* return value */
  auto item   = (*s_history)[index.row()];
  return item;
}

QHash<int, QByteArray> DapQmlModelSerialHistory::roleNames() const
{
  QHash<int, QByteArray> names;

  names.insert (0, "key");

  return names;
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlModelSerialHistory::slotSetup(DapCmdDataLocal * pCmdDataLocal)
{
//  *s_history  = QStringList{
//      "keytest1",
//      "keytest2",
//      "keytest3",
//      "keytest4",
//      "keytest5",
//      "keytest6",
//      "keytest7",
//      "keytest8",
//      "keytest9",
//      "keytest10",
//      "keytest11",
//      "keytest12",
//      "keytest13",
//      "keytest14",
//      "keytest15",
//      "keytest16",
//      "keytest17",
//      "keytest18",
//      "keytest19",
//      "keytest20",
//      "keytest21",
//  };

  m_pCmdDataLocal = pCmdDataLocal;

  QObject::connect(pCmdDataLocal, &DapCmdDataLocal::sigGotValue, [this](QString name, QVariant value, const int a_msgId) {
      Q_UNUSED(a_msgId);
      if (name == "serial_history_key")
      {
          beginResetModel();

          auto source = value.toList();

          QStringList result;
          for (const auto &key : source)
              result << key.toString();

          *s_history = result;
          endResetModel();
      }
  });
}

void DapQmlModelSerialHistory::update()
{
    m_pCmdDataLocal->requestValue("serial_history_key");
}

/*-----------------------------------------*/

_DapQmlModelSerialHistoryCopyButtonAnimation::_DapQmlModelSerialHistoryCopyButtonAnimation (QObject *a_copyButton)
  : QObject (a_copyButton)
  , m_timer (new QTimer (this))
{
  m_timer->setInterval (100);
  m_timer->setSingleShot (true);

  connect (a_copyButton, SIGNAL (sigStartAnimation()),
           this, SLOT (slotStart()));
  connect (m_timer.get(), &QTimer::timeout,
           this, &_DapQmlModelSerialHistoryCopyButtonAnimation::slotFinish);
}

void _DapQmlModelSerialHistoryCopyButtonAnimation::slotStart()
{
  if (parent())
    parent()->setProperty ("iconRight", QString ("ic_copy_pressed"));
  m_timer->start();
}

void _DapQmlModelSerialHistoryCopyButtonAnimation::slotFinish()
{
  if (parent())
    parent()->setProperty ("iconRight", QString ("ic_copy"));
}

/*-----------------------------------------*/
