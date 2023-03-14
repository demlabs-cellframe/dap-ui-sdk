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

/* DEFS */
typedef QString ServerAddress;

/* bridge class */
class ServerListModelBridge : public AbstractServerListModelBridge
{
public:
  ServerListModelBridge();
  ~ServerListModelBridge() override;

  DapAbstractServerList *serverList() const override;
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

  /* clear memory */
  if (!s_enabled)
    {
      delete s_servers;
      delete s_autoServers;
      delete s_serverListBridge;
      delete s_customServers;
      delete s_saveDelay;

      if (s_serverListBridge)
        DapQmlModelFullServerList::instance()->setBridge (AbstractServerListModelBridge::getDefaultBridge());
    }

  /* invoke delayed loading and bridging */
  else
    QTimer::singleShot (5000, this, [this]
     {
        load();
        DapQmlModelFullServerList::instance()->setBridge (s_serverListBridge);
      });

  s_servers           = s_enabled ? new DapSortedServerList                   : nullptr;
  s_autoServers       = s_enabled ? new DapQmlModelAutoServerList (s_servers) : nullptr;
  s_serverListBridge  = s_enabled ? new ServerListModelBridge                 : nullptr;
  s_customServers     = s_enabled ? new CustomServerDataMap                   : nullptr;
  s_saveDelay         = s_enabled ? new ManageServersSaveDelay (*this)        : nullptr;
}

void DapQmlServerManager::importServers (const DapServerInfoList *a_servers)
{
  if (!enabled())
    {
      auto serverList = DapQmlModelFullServerList::instance()->bridge()->serverList();
      for (const auto &server : *a_servers)
        if (-1 == serverList->indexOfAddress (server.address()))
          serverList->append (server);
      return;
    }

  for (auto i = a_servers->begin(), e = a_servers->end(); i != e; i++)
    if (!s_customServers->contains (i->address()))
      {
        s_servers->append (*i);
        s_customServers->insert (i->address(), *i);
      }
}

#define _parseFieldSingle(map,item,name,conversion) \
  if (map.contains (#name)) \
    item.name (map[#name].conversion());

#define _parseFieldDouble(map,item1,item2,name,conversion) \
  if (map.contains (#name)) \
    { \
      auto val = map[#name].conversion(); \
      item1.name (val); \
      item2.name (val); \
    }

static void parseServerData(
  /* in  */ const QVariant &a_data,
  /* out */ DapServerInfo &a_server,
  /* out */ CustomServerData &a_custom)
{
  auto value = a_data.toMap();

  _parseFieldSingle (value, a_server, setName, toString);
  _parseFieldSingle (value, a_server, setLocation, toString);
  _parseFieldDouble (value, a_server, a_custom, setAddress, toString);
  _parseFieldDouble (value, a_server, a_custom, setPort, toInt);
  _parseFieldSingle (value, a_custom, setFavorite, toBool);
}

void DapQmlServerManager::append (const QVariant &a_data)
{
  if (!enabled())
    return;

  /* fill classes with data */
  DapServerInfo server;
  CustomServerData custom;
  parseServerData (a_data, server, custom);
  custom.setCustom (true); // new item marked as custom

  /* store result */
  s_customServers->insert (custom.address(), std::move (custom));
  s_servers->append (std::move (server));
}

void DapQmlServerManager::replace (int a_index, const QVariant &a_data)
{
  if (!enabled()
      || a_index >= s_servers->size())
    return;

  /* fill classes with data */
  DapServerInfo &server     = s_servers->at (a_index);
  CustomServerData &custom  = (*s_customServers) [server.address()];
  parseServerData (a_data, server, custom);
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
  for (auto i = jarr.cbegin(), e = jarr.cend(); i != e; i++)
    {
      auto jobj = i->toObject();
      CustomServerData item (jobj);
      DapServerInfo server (jobj);
      s_servers->append (std::move (server));
      s_customServers->insert (item.address(), item);
    }
}

void DapQmlServerManager::save()
{
  if (!enabled())
    return;

  QJsonArray jarr;
  for (const auto &item : qAsConst (*s_customServers))
    jarr << item.toJson();
  DapDataLocal::instance()->saveSetting (SETTING_SERVERS, jarr);
}

/*-----------------------------------------*/

ServerListModelBridge::ServerListModelBridge()                            {}
ServerListModelBridge::~ServerListModelBridge()                           {}
DapAbstractServerList *ServerListModelBridge::serverList() const          { return dynamic_cast<DapAbstractServerList*> (s_servers); }
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
  setAddress  (a_src.address());
  setPort     (a_src.port());
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
  setAddress  (a_src.value ("address")  .toString());
  setPort     (a_src.value ("port")     .toInt());
  setFavorite (a_src.value ("favorite") .toBool());
  setDeleted  (a_src.value ("deleted")  .toBool());
  setCustom   (a_src.value ("custom")   .toBool());
}

CustomServerData &CustomServerData::operator= (const CustomServerData &a_src)
{
  if (&a_src != this)
    {
      setAddress  (a_src.address());
      setPort     (a_src.port());
      setFavorite (a_src.favorite());
      setDeleted  (a_src.deleted());
      setCustom   (a_src.custom());
    }
  return *this;
}

CustomServerData &CustomServerData::operator= (CustomServerData &&a_src)
{
  if (&a_src != this)
    {
      m_address   = std::move (a_src.m_address);
      setPort     (a_src.port());
      setFavorite (a_src.favorite());
      setDeleted  (a_src.deleted());
      setCustom   (a_src.custom());
    }
  return *this;
}

CustomServerData &CustomServerData::operator= (const QJsonObject &a_src)
{
  fromJson (a_src);
  return *this;
}

/*-----------------------------------------*/
