/* INCLUDES */
#include "dapqmlmodelfullserverlist.h"
#include "dapqml-abstract/abstractserverlistmodelbridge.h"
#include "DapServerInfo.h"


/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlModelFullServerList::DapQmlModelFullServerList()
  : m_bridge (nullptr)
  , m_current (-1)
{
  setBridge (AbstractServerListModelBridge::getDefaultBridge());
}

/********************************************
 * METHODS
 *******************************************/

DapQmlModelFullServerList *DapQmlModelFullServerList::instance()
{
  static DapQmlModelFullServerList i;
  return &i;
}

const AbstractServerListModelBridge *DapQmlModelFullServerList::bridge() const
{
  return m_bridge;
}

void DapQmlModelFullServerList::setBridge (AbstractServerListModelBridge *a_newBridge)
{
  /* variables */
//  auto updateLambda     = [this]
//  {
//    beginResetModel();
//    _getSizes();
//    _getCurrent();
//    endResetModel();
//  };
  auto autoServerList   = a_newBridge->autoServerList();
  auto serverList       = a_newBridge->serverList();

  /* setup */
  beginResetModel();
  {
    if (m_bridge
        && m_bridge != a_newBridge
        && m_bridge != AbstractServerListModelBridge::getDefaultBridge())
      delete m_bridge;

    m_bridge  = a_newBridge;
    _getSizes();
    _getRoles();
  }
  endResetModel();

  /* clear connections */
  _detach();

  /* create connections */
//  _conn << connect (autoServerList,   &QAbstractItemModel::rowsInserted, updateLambda);
//  _conn << connect (autoServerList,   &QAbstractItemModel::rowsRemoved,  updateLambda);
//  _conn << connect (autoServerList,   &QAbstractItemModel::rowsMoved,    updateLambda);
//  _conn << connect (autoServerList,   &QAbstractItemModel::modelReset,   updateLambda);
//  _conn << connect (serverList,       &QAbstractItemModel::rowsInserted, updateLambda);
//  _conn << connect (serverList,       &QAbstractItemModel::rowsRemoved,  updateLambda);
//  _conn << connect (serverList,       &QAbstractItemModel::rowsMoved,    updateLambda);
//  _conn << connect (serverList,       &QAbstractItemModel::modelReset,   updateLambda);
  _attach (autoServerList);
  _attach (serverList);
}

int DapQmlModelFullServerList::size() const
{
  return _size.full;
}

int DapQmlModelFullServerList::current() const
{
  return m_current;
}

void DapQmlModelFullServerList::setCurrent (int a_newCurrent)
{
  /* checks */
  if (a_newCurrent != -2)
    if (m_current == a_newCurrent
        || a_newCurrent >= _size.full)
      return;

  /* reapply auto server */
  if (a_newCurrent == -2)
  {
    if (m_current < _size.autoServer)
      a_newCurrent  = m_current;
    else
      return;
  }

  auto autoServerList = m_bridge->autoServerList();
  auto serverList     = m_bridge->serverList();
  auto oldCurrent     = m_current;
  int oldAutoCurrent  = autoServerList->current();
  int oldListCurrent  = serverList->current();

  m_current   = a_newCurrent;

  autoServerList->setCurrent ((m_current < _size.autoServer)  ? m_current : -1);
  serverList->setCurrent ((m_current >= _size.autoServer) ? m_current - _size.autoServer : -1);

  auto autoData = autoServerList->sortIndexData();
  auto listData = serverList->sortIndexData();

  qDebug("%s : old [n:%s,i:%d,a:%d,s:%d] new [n:%s,i:%d,a:%d,s:%d]",
         "DapQmlModelFullServerList::setCurrent",
         (oldAutoCurrent != -1)
         ? autoData.first.name.toUtf8().data()
         : listData.first.name.toUtf8().data(),

         oldCurrent,
         oldAutoCurrent,
         oldListCurrent,

         (autoServerList->current() != -1)
         ? autoData.second.name.toUtf8().data()
         : listData.second.name.toUtf8().data(),

         m_current,
         autoServerList->current(),
         serverList->current());

  emit currentChanged();
}

bool DapQmlModelFullServerList::isCurrentAuto() const
{
  if (m_current < 0)
    return false;
  return m_current < _size.autoServer;
}

QVariant DapQmlModelFullServerList::value (int a_row, const QString &a_name) const
{
  int fieldId = _roleNamesMap.key (a_name.toUtf8(), -1);

  if (fieldId == -1)
    return QVariant();

  return data (index (a_row, 0), fieldId);
}

const DapServerInfo &DapQmlModelFullServerList::currentServer() const
{
  static DapServerInfo dummy;
  if (current() == -1 || size() == 0)
    return dummy;
  return at (current());
}

const DapServerInfo &DapQmlModelFullServerList::at (int a_index) const
{
  auto autoServerList  = m_bridge->autoServerList();
  auto serverList      = m_bridge->serverList();

  /* if auto server boundaries */
  if (a_index < _size.autoServer)
    return autoServerList->getList().at (a_index);

  /* server boundaries */
  int newIndexValue     = a_index - _size.autoServer;
  return serverList->at (newIndexValue);
}

DapQmlModelFullServerList::ConstIterator DapQmlModelFullServerList::begin() const
{
  return ConstIterator (0, this);
}

DapQmlModelFullServerList::ConstIterator DapQmlModelFullServerList::end() const
{
  return ConstIterator (size(), this);
}

DapQmlModelFullServerList::Index DapQmlModelFullServerList::indexOfName (const QString &a_name) const
{
  auto autoServerList  = m_bridge->autoServerList();
  auto serverList      = m_bridge->serverList();

  int result  = autoServerList->getList().indexOfName (a_name);
  if (result != -1)
    return Index (result, _size.autoServer, true);

  result      = serverList->indexOfName (a_name);
  if (result != -1)
    return Index (result + _size.autoServer, _size.autoServer, false);

  return Index();
}

DapQmlModelFullServerList::Index DapQmlModelFullServerList::indexOfAddress (const QString &a_address) const
{
  auto autoServerList  = m_bridge->autoServerList();
  auto serverList      = m_bridge->serverList();

  int result  = autoServerList->getList().indexOfAddress (a_address);
  if (result != -1)
    return Index (result, _size.autoServer, true);

  result      = serverList->indexOfAddress (a_address);
  if (result != -1)
    return Index (result + _size.autoServer, _size.autoServer, false);

  return Index();
}

void DapQmlModelFullServerList::refresh()
{
  setBridge (m_bridge);
}

void DapQmlModelFullServerList::_getSizes()
{
  auto autoServerList  = m_bridge->autoServerList();
  auto serverList      = m_bridge->serverList();
  _size.autoServer      = autoServerList->rowCount (QModelIndex());
  _size.server          = serverList->rowCount (QModelIndex());
  _size.full            = _size.autoServer + _size.server;
}

void DapQmlModelFullServerList::_getRoles()
{
  _roleNamesMap       = DapServerList::serverRoleNames();
  const auto &custom  =  m_bridge->customRoleNames();

  for (auto i = custom.begin(), e = custom.end(); i != e; i++)
    _roleNamesMap.insert (i.key(), i.value());
}

void DapQmlModelFullServerList::_getCurrent()
{
  auto autoServerList  = m_bridge->autoServerList();
  auto serverList      = m_bridge->serverList();

  if (autoServerList->current() == -1)
    {
      if (serverList->current() == -1)
        return setCurrent (0);
      return setCurrent (serverList->current() + _size.autoServer);
    }

  setCurrent (autoServerList->current());
}

DapQmlModelFullServerList::SenderType DapQmlModelFullServerList::_getSenderType() const
{
  auto *sender = QObject::sender();
  if (sender == bridge()->autoServerList())
    return Auto;
  else if (sender == bridge()->serverList())
    return List;
  return Invalid;
}

void DapQmlModelFullServerList::_attach (QAbstractListModel *a_model)
{
  _conn << QObject::connect (a_model,   &QAbstractListModel::rowsAboutToBeInserted,  this, &DapQmlModelFullServerList::rowsAboutToBeInserted);
  _conn << QObject::connect (a_model,   &QAbstractListModel::rowsInserted,           this, &DapQmlModelFullServerList::rowsInserted);
  _conn << QObject::connect (a_model,   &QAbstractListModel::rowsAboutToBeMoved,     this, &DapQmlModelFullServerList::rowsAboutToBeMoved);
  _conn << QObject::connect (a_model,   &QAbstractListModel::rowsMoved,              this, &DapQmlModelFullServerList::rowsMoved);
  _conn << QObject::connect (a_model,   &QAbstractListModel::rowsAboutToBeRemoved,   this, &DapQmlModelFullServerList::rowsAboutToBeRemoved);
  _conn << QObject::connect (a_model,   &QAbstractListModel::rowsRemoved,            this, &DapQmlModelFullServerList::rowsRemoved);
  _conn << QObject::connect (a_model,   &QAbstractListModel::modelAboutToBeReset,    this, &DapQmlModelFullServerList::modelAboutToBeReset);
  _conn << QObject::connect (a_model,   &QAbstractListModel::modelReset,             this, &DapQmlModelFullServerList::modelReset);
  _conn << QObject::connect (a_model,   &QAbstractListModel::dataChanged,            this, &DapQmlModelFullServerList::dataChanged);
}

void DapQmlModelFullServerList::_detach()
{
  for (auto &conn : _conn)
    QObject::disconnect (conn);
  _conn.clear();
}

/********************************************
 * OVERRIDE
 *******************************************/

int DapQmlModelFullServerList::rowCount (const QModelIndex &parent) const
{
  Q_UNUSED (parent)
//  return m_bridge->serverList()     .rowCount (parent)
//       + m_bridge->autoServerList() .rowCount (parent);
  return _size.full;
}

QVariant DapQmlModelFullServerList::data (const QModelIndex &index, int role) const
{
  auto autoServerList   = m_bridge->autoServerList();
  auto serverList       = m_bridge->serverList();
  auto &customRoleNames = m_bridge->customRoleNames();

  /* check boundaries */
  if (index.row() < 0 || index.row() >= _size.full)
    return QVariant();

  /* handle custom */
  if (customRoleNames.contains (role))
    return m_bridge->customData (index, role, _size.autoServer, _size.server);

  /* if auto server boundaries */
  if (index.row() < _size.autoServer)
    return autoServerList->data (index, role);

  /* server boundaries */
  int newIndexValue     = index.row() - _size.autoServer;
  QModelIndex newIndex  = QAbstractListModel::index (newIndexValue, index.column(), index.parent());
  return serverList->data (newIndex, role);
}

QHash<int, QByteArray> DapQmlModelFullServerList::roleNames() const
{
  return _roleNamesMap;
}

/********************************************
 * SLOTS
 *******************************************/

void DapQmlModelFullServerList::rowsAboutToBeInserted (const QModelIndex &, int start, int end)
{
  /* vars */
  SenderType type = _getSenderType();

  /* check issue */
  if (type == Invalid)
    return;

  /* fix indexes */
  if (type == List)
    {
      start += _size.autoServer;
      end   += _size.autoServer;
    }

  /* initiate update */
  beginInsertRows (QModelIndex(), start, end);
}

void DapQmlModelFullServerList::rowsInserted (const QModelIndex &, int, int)
{
  _getSizes();
  _getCurrent();
  endInsertRows();
}

void DapQmlModelFullServerList::rowsAboutToBeMoved (const QModelIndex &, int first, int last, const QModelIndex &, int dest)
{
  /* vars */
  SenderType type = _getSenderType();

  /* check issue */
  if (type == Invalid)
    return;

  /* fix indexes */
  if (type == List)
    {
      first += _size.autoServer;
      last  += _size.autoServer;
      dest  += _size.autoServer;
    }

  /* initiate update */
  QModelIndex dummy;
  beginMoveRows (dummy, first, last, dummy, dest);
}

void DapQmlModelFullServerList::rowsMoved (const QModelIndex &, int, int, const QModelIndex &, int)
{
  _getSizes();
  _getCurrent();
  endMoveRows();
}

void DapQmlModelFullServerList::rowsAboutToBeRemoved (const QModelIndex &, int first, int last)
{
  /* vars */
  SenderType type = _getSenderType();

  /* check issue */
  if (type == Invalid)
    return;

  /* fix indexes */
  if (type == List)
    {
      first += _size.autoServer;
      last  += _size.autoServer;
    }

  /* initiate update */
  beginRemoveRows (QModelIndex(), first, last);
}

void DapQmlModelFullServerList::rowsRemoved (const QModelIndex &, int, int)
{
  _getSizes();
  _getCurrent();
  endRemoveRows();
}

void DapQmlModelFullServerList::modelAboutToBeReset()
{
  beginResetModel();
}

void DapQmlModelFullServerList::modelReset()
{
  _getSizes();
  _getCurrent();
  endResetModel();
}

void DapQmlModelFullServerList::dataChanged (const QModelIndex &a_topLeft, const QModelIndex &a_bottomRight, const QVector<int> &roles)
{
  /* vars */
  SenderType type         = _getSenderType();
  QModelIndex topLeft, bottomRight;

  /* check issue */
  if (type == Invalid)
    return;

  _getSizes();
  _getCurrent();

  /* fix indexes */
  if (type == List)
    {
      topLeft     = index (a_topLeft.row()      + _size.autoServer, a_topLeft.column());
      bottomRight = index (a_bottomRight.row()  + _size.autoServer, a_bottomRight.column());
    }
  else
    {
      topLeft     = index (a_topLeft.row(),     a_topLeft.column());
      bottomRight = index (a_bottomRight.row(), a_bottomRight.column());
    }

  /* initiate update */
  emit QAbstractListModel::dataChanged (topLeft, bottomRight, roles);
}

/*-----------------------------------------*/

DapQmlModelFullServerList::ConstIterator::ConstIterator() : p (nullptr), i (0) {}
DapQmlModelFullServerList::ConstIterator::ConstIterator (int n, const DapQmlModelFullServerList *p) : p (p), i (n) {}
DapQmlModelFullServerList::ConstIterator::ConstIterator (const DapQmlModelFullServerList::ConstIterator &o) : p (o.p), i (o.i) {}
const DapServerInfo &DapQmlModelFullServerList::ConstIterator::operator*() const        { return p->at (i); }
const DapServerInfo *DapQmlModelFullServerList::ConstIterator::operator->() const       { return &p->at (i); }
const DapServerInfo &DapQmlModelFullServerList::ConstIterator::operator[] (int j) const { return p->at (i + j); }
bool DapQmlModelFullServerList::ConstIterator::operator== (const DapQmlModelFullServerList::ConstIterator &o) const     { return i == o.i; }
bool DapQmlModelFullServerList::ConstIterator::operator!= (const DapQmlModelFullServerList::ConstIterator &o) const     { return i != o.i; }
bool DapQmlModelFullServerList::ConstIterator::operator< (const DapQmlModelFullServerList::ConstIterator &other) const  { return i < other.i; }
bool DapQmlModelFullServerList::ConstIterator::operator<= (const DapQmlModelFullServerList::ConstIterator &other) const { return i <= other.i; }
bool DapQmlModelFullServerList::ConstIterator::operator> (const DapQmlModelFullServerList::ConstIterator &other) const  { return i < other.i; }
bool DapQmlModelFullServerList::ConstIterator::operator>= (const DapQmlModelFullServerList::ConstIterator &other) const { return i >= other.i; }
DapQmlModelFullServerList::ConstIterator DapQmlModelFullServerList::ConstIterator::operator++ (int)         { ConstIterator n (i, p); ++i; return n; }
DapQmlModelFullServerList::ConstIterator DapQmlModelFullServerList::ConstIterator::operator-- (int)         { ConstIterator n (i, p); i--; return n; }
DapQmlModelFullServerList::ConstIterator &DapQmlModelFullServerList::ConstIterator::operator+= (int j)      { i += j; return *this; }
DapQmlModelFullServerList::ConstIterator &DapQmlModelFullServerList::ConstIterator::operator-= (int j)      { i -= j; return *this; }
int DapQmlModelFullServerList::ConstIterator::operator- (ConstIterator j) const                             { return i - j.i; }
DapQmlModelFullServerList::ConstIterator::operator const DapServerInfo *() const                            { return operator->(); }
int DapQmlModelFullServerList::ConstIterator::internalIndex() const                                         { return i; }
DapQmlModelFullServerList::ConstIterator DapQmlModelFullServerList::ConstIterator::operator- (int j) const  { return ConstIterator (i - j, p); }
DapQmlModelFullServerList::ConstIterator DapQmlModelFullServerList::ConstIterator::operator+ (int j) const  { return ConstIterator (i + j, p); }
DapQmlModelFullServerList::ConstIterator &DapQmlModelFullServerList::ConstIterator::operator--()    { i--; return *this; }
DapQmlModelFullServerList::ConstIterator &DapQmlModelFullServerList::ConstIterator::operator++()    { i++; return *this; }

/*-----------------------------------------*/

DapQmlModelFullServerList::Index::Index()
  : _value (-1)
  , _autoSize (-1)
  , _isAuto (false)
{

}

DapQmlModelFullServerList::Index::Index (int a_value, int a_autoSize, bool a_isAuto)
  : _value (a_value)
  , _autoSize (a_autoSize)
  , _isAuto (a_isAuto)
{

}

DapQmlModelFullServerList::Index::Index (const DapQmlModelFullServerList::Index &a_src)
{
  _value    = a_src._value;
  _autoSize = a_src._autoSize;
  _isAuto   = a_src._isAuto;
}

int DapQmlModelFullServerList::Index::internal() const
{
  return (!_isAuto ? _value - _autoSize : _value);
}

int DapQmlModelFullServerList::Index::value() const
{
  return _value;
}

bool DapQmlModelFullServerList::Index::isAuto() const
{
  return _isAuto;
}

bool DapQmlModelFullServerList::Index::isSorted() const
{
  return !_isAuto;
}

DapQmlModelFullServerList::Index::operator int()
{
  return value();
}

/*-----------------------------------------*/
