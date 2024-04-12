/* INCLUDES */
#include "dapqmlservermanager.h"
#include "DapDataLocal.h"

#include "dapqml-model/dapqmlmodelautoserverlist.h"
#include "dapqml-model/dapqmlmodelfullserverlist.h"
#include "dapqml-abstract/abstractserverlistmodelbridge.h"

#include <QVariant>
#include <QTimer>
#include <QMetaEnum>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

/* DEFS */
typedef QString ServerAddress;

/* bridge class */
class ServerListModelBridge : public AbstractServerListModelBridge
{
public:
  ServerListModelBridge();
  ~ServerListModelBridge() override;

  DapSortedServerList *serverList() const override;
  DapQmlModelAutoServerList *autoServerList() const override;
  QVariant customData (const QModelIndex &a_index, int a_role, int a_sizeAuto, int a_size) const override;
  const QHash<int, QByteArray> &customRoleNames() const override;
};

/* custom data for servers */
class CustomServerData
{
  QString m_address;    ///< ip4
  quint16 m_port;       ///< port
  bool m_favorite : 1;  ///< is this favorited
  bool m_deleted : 1;   ///< is this deleted server
  bool m_custom : 1;    ///< is this custom server created by user

public:
  CustomServerData();
  CustomServerData (const QString &a_src);
  CustomServerData (const QJsonObject &a_src);
  CustomServerData (const DapServerInfo &a_src);
  CustomServerData (const CustomServerData &a_src);
  CustomServerData (CustomServerData &&a_src);
  ~CustomServerData();

  const QString &address() const              { return m_address; }
  void setAddress (const QString &a_address)  { m_address = a_address; }

  quint16 port() const                        { return m_port; }
  void setPort (quint16 a_port)               { m_port = a_port; }

  bool favorite() const                       { return m_favorite; }
  void setFavorite (bool a_favorite)          { m_favorite = a_favorite; }

  bool deleted() const                        { return m_deleted; }
  void setDeleted (bool a_deleted)            { m_deleted = a_deleted; }

  bool custom() const                         { return m_custom; }
  void setCustom (bool a_custom)              { m_custom = a_custom; }

  QString toString() const;
  void fromString (const QString &a_src);

  QJsonObject toJson() const;
  void fromJson (const QJsonObject &a_src);

  CustomServerData &operator= (const CustomServerData &a_src);
  CustomServerData &operator= (CustomServerData &&a_src);
  CustomServerData &operator= (const QJsonObject &a_src);
};
typedef QMap<ServerAddress, CustomServerData> CustomServerDataMap;

/* save delay */
#define SAVE_DELAY (10 * 1000)
#define SAVE s_saveDelay->start
class ManageServersSaveDelay
{
  QTimer _timer;
  DapQmlServerManager &_parent;

public:
  ManageServersSaveDelay (DapQmlServerManager &a_parent)
    : _parent (a_parent)
  {
    _timer.setSingleShot (true);
    _timer.setInterval (SAVE_DELAY);
    QObject::connect (&_timer, &QTimer::timeout, [this] { _parent.save(); });
  }

  void start() { _timer.start(); }
};

/* VARS */
static const QString SETTING_SERVERS {"Servers"};

static bool s_enabled = false;
static DapSortedServerList *s_servers             = nullptr;  ///< sorted server list
static DapQmlModelAutoServerList *s_autoServers   = nullptr;  ///< auto servers list
static ServerListModelBridge *s_serverListBridge  = nullptr;  ///< bridge instance
static CustomServerDataMap *s_customServers       = nullptr;  ///< custom server data map
static ManageServersSaveDelay *s_saveDelay        = nullptr;  ///< custom list save delay

/********************************************
 * CONSTRUCT/DESTRUCT
 *******************************************/

DapQmlServerManager::DapQmlServerManager()
{

}

/********************************************
 * METHODS
 *******************************************/

DapQmlServerManager *DapQmlServerManager::instance()
{
  static DapQmlServerManager i;
  return &i;
}

bool DapQmlServerManager::enabled() const
{
  return s_enabled;
}

void DapQmlServerManager::setEnabled (bool a_enable)
{
  s_enabled = a_enable;
  auto setup = [this]
  {
    s_servers           = s_enabled ? new DapSortedServerList                   : nullptr;
    s_autoServers       = s_enabled ? new DapQmlModelAutoServerList (s_servers) : nullptr;
    s_serverListBridge  = s_enabled ? new ServerListModelBridge                 : nullptr;
    s_customServers     = s_enabled ? new CustomServerDataMap                   : nullptr;
    s_saveDelay         = s_enabled ? new ManageServersSaveDelay (*this)        : nullptr;
  };

  /* clear memory */
  if (!s_enabled)
    {
      delete s_servers;
      delete s_autoServers;
      //delete s_serverListBridge; // DapQmlModelFullServerList is responsible for deleting this object
      delete s_customServers;
      delete s_saveDelay;

      if (s_serverListBridge)
        DapQmlModelFullServerList::instance()->setBridge (AbstractServerListModelBridge::getDefaultBridge());

      setup();
    }

  /* invoke delayed loading and bridging */
  else
    //QTimer::singleShot (5000, this, [this]
    {
      setup();
      load();
      DapQmlModelFullServerList::instance()->setBridge (s_serverListBridge);
    }//);

}

void DapQmlServerManager::importServers (const DapServerInfoList *a_servers)
{
  if (!enabled())
    {
      auto fullList     = DapQmlModelFullServerList::instance();
      auto bridge       = fullList->bridge();
      auto serverList   = bridge->serverList();

#ifdef BRAND_KELVPN

      /* store current server */
      auto current  = fullList->currentServer().name();

      /* clear servers */
      serverList->clear();

      /* add servers from scratch */
      for (auto server : *a_servers)
      {
        if (server.ping() == -1)
          server.setConnQuality (DapServerInfo::ConnectionQuality::INVALID);
        serverList->append (server);
      }

      /* refresh full model */
      fullList->refresh();

      /* restore current */
      int index = fullList->indexOfName (current);
      if (!current.isEmpty())
        fullList->setCurrent (index != -1 ? index : 0);

#else // BRAND_KELVPN

      for (const auto &server : *a_servers)
        if (-1 == serverList->indexOfAddress (server.address()))
          serverList->append (server);

#endif // BRAND_KELVPN

      return;
    }

  for (auto i = a_servers->begin(), e = a_servers->end(); i != e; i++)
    if (!s_customServers->contains (i->address()))
      {
        s_servers->append (*i);
        s_customServers->insert (i->address(), *i);
      }
}

#define _parseFieldSingle(map,item,name,set,conversion) \
  if (map.contains (#name)) \
    item.set (map[#name].conversion());

#define _parseFieldDouble(map,item1,item2,name,set,conversion) \
  if (map.contains (#name)) \
    { \
      auto val = map[#name].conversion(); \
      item1.set (val); \
      item2.set (val); \
    }

static void parseServerData (
  /* in  */ const QVariant &a_data,
  /* out */ DapServerInfo &a_server,
  /* out */ CustomServerData &a_custom)
{
  auto value = a_data.toMap();

  _parseFieldSingle (value, a_server, name, setName, toString);
  _parseFieldSingle (value, a_server, location, setLocation, toString);
  _parseFieldDouble (value, a_server, a_custom, address, setAddress, toString);
  _parseFieldDouble (value, a_server, a_custom, port, setPort, toInt);
  _parseFieldSingle (value, a_custom, favorite, setFavorite, toBool);
}

bool DapQmlServerManager::append (const QVariant &a_data)
{
  if (!enabled())
    return false;

  /* fill classes with data */
  DapServerInfo server;
  CustomServerData data;
  parseServerData (a_data, server, data);
  data.setCustom (true); // new item marked as custom

  /* check if already exists */
  if (s_customServers->contains (data.address())
      && !s_customServers->value (data.address()).deleted())
    return false;

  /* change server kind */
  if (s_customServers->contains (data.address())
      && s_customServers->value (data.address()).deleted())
    data.setCustom (false);

  /* store result */
  s_customServers->insert (data.address(), std::move (data));
  s_servers->append (std::move (server));

  SAVE();

  return true;
}

bool DapQmlServerManager::replace (int a_index, const QVariant &a_data)
{
  if (!enabled() || a_index >= s_servers->size())
    return false;

  /* copy and parse */
  DapServerInfo server      = s_servers->at (a_index);
  CustomServerData data     = (*s_customServers) [server.address()];
  ServerAddress addressCopy = server.address();
  parseServerData (a_data, server, data);

  /* rules */
  bool isChangedAddress     = server.address() != addressCopy;
  bool isServerCustom       = data.custom();

  /****************************************
   * Custom Data
   ***************************************/

  /* if user changed the address */
  if (isChangedAddress)
    {
      /* break if new adrress already set */
      if (s_customServers->contains (data.address())
          && !s_customServers->value (data.address()).deleted())
        return false;

      /* change server kind */
      if (s_customServers->contains (data.address())
          && s_customServers->value (data.address()).deleted())
        {
          /* change to not custom */
          data.setCustom (false);

          /* connect address */
          data.setAddress (server.address());

          /* replace key */
          s_customServers->insert (data.address(), std::move (data));
        }

      /* if pure custom */
      else
        {
          /* if custom server -> replace */
          if (isServerCustom)
            {
              /* connect address */
              data.setAddress (server.address());

              /* remove old key */
              s_customServers->remove (addressCopy);

              /* add new key */
              s_customServers->insert (data.address(), std::move (data));
            }

          /* if not custom -> hide */
          else
            {
              /* hide */
              remove (a_index);

              /* create new */
              data.setCustom (true);
              s_customServers->insert (data.address(), std::move (data));
            }
        }
    }

  /****************************************
   * Server Data
   ***************************************/

  /* update server data */
  s_servers->at (a_index) = server;

  SAVE();

  auto index  = s_servers->index (a_index);
  emit s_servers->dataChanged (index, index);
  return true;
}

void DapQmlServerManager::remove (int a_index)
{
  /* remove server */
  ServerAddress removeItem = s_servers->at (a_index).address();
  s_servers->remove (a_index);

  /* mark server as deleted */
  auto &removeData  = (*s_customServers)[removeItem];

  /* mark if not custom */
  if (!removeData.custom())
    removeData.setDeleted (true);

  /* delete if custom */
  else
    s_customServers->remove (removeItem);

  SAVE();
}

void DapQmlServerManager::fave (int a_index)
{
  if (!enabled())
    return;

  ServerAddress address = s_servers->at (a_index).address();
  auto &removeData      = (*s_customServers)[address];
  removeData.setFavorite (!removeData.favorite());

  SAVE();

  auto index  = s_servers->index (a_index);
  emit s_servers->dataChanged (index, index);
}

const DapServerInfo &DapQmlServerManager::at (int a_index) const
{
  return s_servers->at (a_index);
}

int DapQmlServerManager::size() const
{
  return s_servers->size();
}

void DapQmlServerManager::load()
{
  if (!enabled())
    return;

  auto jarr = DapDataLocal::instance()->getSetting (SETTING_SERVERS).toJsonArray();
  _fromJson (jarr);
}

void DapQmlServerManager::save()
{
  if (!enabled())
    return;

  QJsonArray jarr;
  _toJson (jarr);

  DapDataLocal::instance()->saveSetting (SETTING_SERVERS, jarr);
}

void DapQmlServerManager::saveTo (const QString &a_filename) const
{
  if (!enabled())
    return;

  QJsonArray jarr;
  _toJson (jarr);

  QJsonDocument doc (jarr);
  auto data = doc.toJson();
  QFile file (a_filename);
  file.open (QIODevice::WriteOnly | QIODevice::Truncate);
  file.write (data);
}

void DapQmlServerManager::loadFrom (const QString &a_filename)
{
  if (!enabled())
    return;

  QFile file (a_filename);
  file.open (QIODevice::ReadOnly);
  auto data = file.readAll();

  auto doc  = QJsonDocument::fromJson (data);
  _fromJson (doc.array());
}

void DapQmlServerManager::_hardReset()
{
  s_servers->clear();
  s_customServers->clear();
  save();
}

void DapQmlServerManager::_toJson (QJsonArray &a_dest) const
{
  QStringList keys  = s_customServers->keys();
  for (const auto &server : qAsConst (*s_servers))
    {
      auto custom = s_customServers->value (server.address());
      auto jobj   = server.toJSON();

      keys.removeOne (server.address());

      jobj.insert ("custom", custom.toJson());
      a_dest << jobj;
    }

  for (const auto &customAddress : qAsConst (keys))
    {
      QJsonObject jobj;
      auto custom = s_customServers->value (customAddress);

      jobj.insert ("custom", custom.toJson());
      a_dest << jobj;
    }
}

void DapQmlServerManager::_fromJson (const QJsonArray &a_src) const
{
  s_servers->clear();
  s_customServers->clear();

  for (auto i = a_src.cbegin(), e = a_src.cend(); i != e; i++)
    {
      auto jobj = i->toObject();

      /* only custom */
      if (!jobj.contains ("Name"))
        {
          if (!jobj.contains ("custom"))
            continue;

          /* parse and store */
          CustomServerData item (jobj.value ("custom").toObject());
          if (!item.address().isEmpty())
            s_customServers->insert (item.address(), item);
          continue;
        }

      /* parse */
      DapServerInfo server (jobj);
      CustomServerData item (jobj.value ("custom").toObject());

      /* store */
      if (!item.address().isEmpty() && !server.address().isEmpty())
        {
          s_servers->append (std::move (server));
          s_customServers->insert (item.address(), item);
        }
    }
}

bool DapQmlServerManager::_isServerWithAlreadyExists (const QString &a_address) const
{
  for (const auto &server : qAsConst (*s_servers))
    if (server.address() == a_address)
      return true;
  return false;
}

/*-----------------------------------------*/

ServerListModelBridge::ServerListModelBridge()                            {}
ServerListModelBridge::~ServerListModelBridge()                           {}
DapSortedServerList *ServerListModelBridge::serverList() const            { return s_servers; }
DapQmlModelAutoServerList *ServerListModelBridge::autoServerList() const  { return s_autoServers; }

QVariant ServerListModelBridge::customData (const QModelIndex &a_index, int a_role, int a_sizeAuto, int a_size) const
{
  /* skip auto servers */
  if (a_index.row() < a_sizeAuto)
    return QVariant();

  /* get index */
  int index = a_index.row() - a_sizeAuto;

  /* check boundaries */
  if (index >= a_size)
    return QVariant();

  /* get custom data item */
  auto baseItem = s_servers->at (index);
  auto item     = s_customServers->value (baseItem.address());
  if (item.address() != baseItem.address())
    return QVariant();

  /* get data by field id */
  switch (DapQmlServerManager::CustomFieldId (a_role))
    {
    case DapQmlServerManager::CustomFieldId::favorite:  return item.favorite();
    case DapQmlServerManager::CustomFieldId::deleted:   return item.deleted();
    case DapQmlServerManager::CustomFieldId::custom:    return item.custom();
    }

  return QVariant();
}

const QHash<int, QByteArray> &ServerListModelBridge::customRoleNames() const
{
  static QHash<int, QByteArray> result;

  if (result.isEmpty())
    {
      int enumIndex  = DapQmlServerManager::staticMetaObject.indexOfEnumerator ("CustomFieldId");
      auto metaEnum  = DapQmlServerManager::staticMetaObject.enumerator (enumIndex);
      int enumSize   = metaEnum.keyCount();

      for (int i = 0; i < enumSize; i++)
        result.insert (metaEnum.value (i), metaEnum.key (i));
    }

  return result;
}

/*-----------------------------------------*/

CustomServerData::CustomServerData()
  : m_port (0)
  , m_favorite (false)
  , m_deleted (false)
  , m_custom (false)
{

}

CustomServerData::CustomServerData (const QString &a_src)
  : m_port (0)
  , m_favorite (false)
  , m_deleted (false)
  , m_custom (false)
{
  fromString (a_src);
}

CustomServerData::CustomServerData (const QJsonObject &a_src)
  : m_port (0)
  , m_favorite (false)
  , m_deleted (false)
  , m_custom (false)
{
  fromJson (a_src);
}

CustomServerData::CustomServerData (const DapServerInfo &a_src)
  : m_favorite (false)
  , m_deleted (false)
  , m_custom (false)
{
  setAddress (a_src.address());
  setPort (a_src.port());
}

CustomServerData::CustomServerData (const CustomServerData &a_src)
{
  operator = (a_src);
}

CustomServerData::CustomServerData (CustomServerData &&a_src)
{
  operator = (std::move (a_src));
}

CustomServerData::~CustomServerData()
{

}

QString CustomServerData::toString() const
{
  QJsonObject result  = toJson();
  QJsonDocument doc (result);
  return doc.toJson();
}

void CustomServerData::fromString (const QString &a_src)
{
  auto result = QJsonDocument::fromJson (a_src.toUtf8()).object();
  fromJson (result);
}

QJsonObject CustomServerData::toJson() const
{
  return QJsonObject
  {
    {"address",   address()},
    {"port",      port()},
    {"favorite",  favorite()},
    {"deleted",   deleted()},
    {"custom",    custom()},
  };
}

void CustomServerData::fromJson (const QJsonObject &a_src)
{
  setAddress (a_src.value ("address")  .toString());
  setPort (a_src.value ("port")     .toInt());
  setFavorite (a_src.value ("favorite") .toBool());
  setDeleted (a_src.value ("deleted")  .toBool());
  setCustom (a_src.value ("custom")   .toBool());
}

CustomServerData &CustomServerData::operator= (const CustomServerData &a_src)
{
  if (&a_src != this)
    {
      setAddress (a_src.address());
      setPort (a_src.port());
      setFavorite (a_src.favorite());
      setDeleted (a_src.deleted());
      setCustom (a_src.custom());
    }
  return *this;
}

CustomServerData &CustomServerData::operator= (CustomServerData &&a_src)
{
  if (&a_src != this)
    {
      m_address   = std::move (a_src.m_address);
      setPort (a_src.port());
      setFavorite (a_src.favorite());
      setDeleted (a_src.deleted());
      setCustom (a_src.custom());
    }
  return *this;
}

CustomServerData &CustomServerData::operator= (const QJsonObject &a_src)
{
  fromJson (a_src);
  return *this;
}

/*-----------------------------------------*/
