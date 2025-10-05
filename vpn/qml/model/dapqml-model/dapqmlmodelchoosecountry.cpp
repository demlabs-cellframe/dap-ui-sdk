/* INCLUDES */
#include "dapqmlmodelchoosecountry.h"
#include "DapServerList.h"
#include "DapDataLocal.h"
#include <algorithm>

/* DEFS */
#define DEBUGINFO qDebug() << (QString ("DapQmlModelChooseCountry::") + __func__)

enum FieldId
{
  name,
  checked
};

class AbstractModelReset
{
  class ModelAccessor : public QAbstractListModel
  {
  public:
    static ModelAccessor *access (QAbstractListModel *a_model)
    {
      return reinterpret_cast<ModelAccessor *> (a_model);
    }
    void beginResetModel()
    {
      QAbstractListModel::beginResetModel();
    }
    void endResetModel()
    {
      QAbstractListModel::endResetModel();
    }
  };

  ModelAccessor *_model;

public:
  AbstractModelReset (QAbstractListModel *a_model)
    : _model (ModelAccessor::access (a_model))
  {
    _model->beginResetModel();
  }
  ~AbstractModelReset()
  {
    _model->endResetModel();
  }
};

/* VARS */
QHash<int, QByteArray> s_fieldMap =
{
  { FieldId::name,    "name" },
  { FieldId::checked, "checked" },
};

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelChooseCountry::DapQmlModelChooseCountry (QObject *parent)
  : QAbstractListModel {parent}
  , m_current (-1)
{
  _list = DapAbstractServerList::countryMap().keys();
  std::sort(_list.begin(), _list.end());
  _applyFiltering();
}

/********************************************
 * METHODS
 *******************************************/

void DapQmlModelChooseCountry::setRowFilter (const QString &a_filter)
{
  _filter = a_filter;
  _applyFiltering();
}

bool DapQmlModelChooseCountry::countryExist()
{
#ifndef BRAND_RISEVPN
  auto country = DapDataLocal::instance()->getSetting (DapDataLocal::COUNTRY_NAME).toString();
  return !country.isNull() && !country.isEmpty();
#else
  return true;
#endif
}

int DapQmlModelChooseCountry::current() const
{
  return m_current;
}

void DapQmlModelChooseCountry::setCurrent (int newCurrent)
{
  if (newCurrent >= 0
      && newCurrent <= _indexes.size())
  {
    int newSortedIndex = _indexes.value (newCurrent);
    int oldSortedIndex = m_current;
    if (m_current == newSortedIndex)
      return;

    int oldCurrent  = _indexes.indexOf (oldSortedIndex);
    m_current       = newSortedIndex;
    emit currentChanged();

    auto oldIndex = index (oldCurrent);
    auto newIndex = index (newCurrent);
    emit dataChanged (oldIndex, oldIndex);
    emit dataChanged (newIndex, newIndex);

    DEBUGINFO << "New current:" << newSortedIndex << _list.value (newSortedIndex);

    return;
  }

  m_current = -1;
  DEBUGINFO << "New current:" << m_current;
  emit currentChanged();
}

void DapQmlModelChooseCountry::setCurrent (const QString &a_name)
{
  int index = _list.indexOf (a_name);
  DEBUGINFO << a_name;
  if (index == -1)
    return;
  setCurrent (_indexes.indexOf (index));
}

void DapQmlModelChooseCountry::_applyFiltering()
{
  AbstractModelReset _reset (this);

  _indexes.clear();

  int index = 0;
  for (auto i = _list.cbegin(), e = _list.cend(); i != e; i++, index++)
    {
      if (i->startsWith (_filter, Qt::CaseInsensitive))
        {
          if (m_current == index)
            _indexes.prepend (index);
          else
            _indexes << index;
        }
    }
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelChooseCountry::rowCount (const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return _indexes.size();
}

int DapQmlModelChooseCountry::columnCount (const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  return s_fieldMap.size();
}

QVariant DapQmlModelChooseCountry::data (const QModelIndex &index, int role) const
{
  /* check index and role */
  if (!index.isValid()
      || !s_fieldMap.contains (role)
      || index.row() >= _indexes.size())
    return QVariant();

  /* lambda */
  auto capitalLetterName = [] (const QString & a_value)
  {
    if (a_value.size() <= 1)
      return a_value;
    return a_value[0].toUpper()  + a_value.mid (1);
  };

  /* get value index */
  auto sortedIndex  = _indexes.value (index.row());

  /* get result */
  switch (FieldId (role))
    {
    case FieldId::name:     return capitalLetterName (_list.value (sortedIndex));
    case FieldId::checked:  return quint32 (m_current) == sortedIndex;
    }

  return QVariant();
}

QHash<int, QByteArray> DapQmlModelChooseCountry::roleNames() const
{
  return s_fieldMap;
}

/*-----------------------------------------*/
