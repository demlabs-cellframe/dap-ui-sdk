#include "DapServersData.h"
#include "DapBaseDataLocal.h"

DapServersData::DapServersData()
{
    this->loadDatas();
}

DapServersData *DapServersData::instance()
{
    static DapServersData s_instance;
    return &s_instance;
}


void DapServersData::addServer(const DapServerInfo& dsi) {
    int row = m_servers.count();
    insertRows(row, 1);
    setData(index(row, 0), QVariant::fromValue(dsi));
    m_locationInfo.addLocation(dsi.location());
    emit this->serverAdded(dsi);
}

void DapServersData::addServer(const QString& location, const QString& name,
                          const QString & address, quint16 port)
{
    DapServerInfo ss;

    ss.setName (name);
    ss.setLocation (location);
    ss.setAddress (address);
    ss.setPort (port);

    addServer (std::move(ss));
}

void DapServersData::setCurrentServer (int a_serverIndex)
{
    if (!_setCurrentServerByIndex (a_serverIndex))
        return;

    emit currentServerNameChanged (this->currentServerName());
}

void DapServersData::setCurrentServerNotSignal (int a_serverIndex)
{
  if (!_setCurrentServerByIndex (a_serverIndex))
      return;

    emit currentServerNameChangedNotRequestChangingServer (this->currentServerName());
}

void DapServersData::setCurrentServer (const DapServerInfo *a_server)
{
  qDebug() << "Selecting the current server: " << (a_server ? a_server->name() : "null");

  if (a_server == nullptr)
    return;

  setCurrentServer (m_servers.lastIndexOf (*a_server));
}

void DapServersData::setCurrentServerFromService (const DapServerInfo *a_server)
{
  qDebug() << "Selecting the current server received from the service: " << (a_server ? a_server->name() : "null");

  if (a_server == nullptr)
    return;

  int index = -1;
  index = m_servers.lastIndexOf (*a_server);
  if (index == -1)
  {
    addServer (*a_server);
    index = m_servers.lastIndexOf (*a_server);
  }

  setCurrentServerNotSignal (index);
}

/// Set server name.
/// @param server Server name.
void DapServersData::setCurrentServer(const QString &a_serverName)
{
    if (currentServerName() == a_serverName)
        return;

    for (int i = 0; i < this->serversCount(); i++)
    {
        if (m_servers.at(i).name() == a_serverName)
        {
            setCurrentServer(i);
            return;
        }
    }
    this->setCurrentServer(-1);

//    qFatal("There is no server with name %s", qPrintable(a_serverName));
}

void DapServersData::saveDatas() const
{

}

bool DapServersData::_setCurrentServerByIndex(int a_serverIndex)
{
  Q_ASSERT (a_serverIndex >= 0 && a_serverIndex < m_servers.count());

  auto &targetServer  = m_servers[a_serverIndex];
  if (m_currentServer.name() == targetServer.name()
      && m_currentServer == targetServer)
    return false;

  m_currentServer     = targetServer;
  return true;
}

void DapServersData::loadDatas()
{

}

const DapServerInfo &DapServersData::currentServer() const
{
    return m_currentServer;
}

int DapServersData::serversCount() const
{
    return m_servers.count();
}

void DapServersData::clearServerList()
{
    m_servers.clear();
    //this->setCurrentServer(-1); // what for?..

    emit this->serversCleared();
}

/// Get server name.
/// @return Server name.
QString DapServersData::currentServerName() const
{
    return DapServersData::currentServer().name();
}

QString DapServersData::currentServerAdress() const
{
    return DapServersData::currentServer().address();
}

bool DapServersData::currentServerIsAuto() const
{
    return DapServersData::currentServer().isAuto();
}

int DapServersData::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_servers.count();
}

void DapServersData::packServerList()
{
  m_servers.clear();
  m_servers << m_bestRegionServerList << m_pingServerList;
}

const QMap<QString, QString> DapServersData::m_countryMap = {
    {"Andorra"                          , "AD"},
    {"United Arab Emirates"             , "AE"},
    {"Afghanistan"                      , "AF"},
    {"Antigua and Barbuda"              , "AG"},
    {"Anguilla"                         , "AI"},
    {"Albania"                          , "AL"},
    {"Armenia"                          , "AM"},
    {"Netherlands Antilles"             , "AN"},
    {"Angola"                           , "AO"},
    {"Argentina"                        , "AR"},
    {"Austria"                          , "AT"},
    {"Australia"                        , "AU"},
    {"Aruba"                            , "AW"},
    {"Åland Islands"                    , "AX"},
    {"Azerbaijan"                       , "AZ"},
    {"Bosnia and Herzegovina"           , "BA"},
    {"Barbados"                         , "BB"},
    {"Bangladesh"                       , "BD"},
    {"Belgium"                          , "BE"},
    {"Burkina Faso"                     , "BF"},
    {"Bulgaria"                         , "BG"},
    {"Bahrain"                          , "BH"},
    {"Burundi"                          , "BI"},
    {"Benin"                            , "BJ"},
    {"Bermuda"                          , "BM"},
    {"Brunei Darussalam"                , "BN"},
    {"Bolivia"                          , "BO"},
    {"Brazil"                           , "BR"},
    {"Bahamas"                          , "BS"},
    {"Bhutan"                           , "BT"},
    {"Botswana"                         , "BW"},
    {"Belarus"                          , "BY"},
    {"Belize"                           , "BZ"},
    {"Canada"                           , "CA"},
    {"Congo (Democratic Republic of the Congo)", "CD"},
    {"Central African Republic"         , "CF"},
    {"Congo"                            , "CG"},
    {"Switzerland"                      , "CH"},
    {"Côte d'Ivoire"                    , "CI"},
    {"Chile"                            , "CL"},
    {"Cameroon"                         , "CM"},
    {"China"                            , "CN"},
    {"Colombia"                         , "CO"},
    {"Costa Rica"                       , "CR"},
    {"Cuba"                             , "CU"},
    {"Cape Verde"                       , "CV"},
    {"Cyprus"                           , "CY"},
    {"Czech Republic"                   , "CZ"},
    {"Germany"                          , "DE"},
    {"Djibouti"                         , "DJ"},
    {"Denmark"                          , "DK"},
    {"Dominica"                         , "DM"},
    {"Dominican Republic"               , "DO"},
    {"Algeria"                          , "DZ"},
    {"Ecuador"                          , "EC"},
    {"Estonia"                          , "EE"},
    {"Egypt"                            , "EG"},
    {"Eritrea"                          , "ER"},
    {"Spain"                            , "ES"},
    {"Ethiopia"                         , "ET"},
    {"Europe"                           , "EU"},
    {"Finland"                          , "FI"},
    {"Fiji"                             , "FJ"},
    {"Falkland Islands (Malvinas)"      , "FK"},
    {"Micronesia"                       , "FM"},
    {"France"                           , "FR"},
    {"Gabon"                            , "GA"},
    {"United Kingdom"                   , "GB"},
    {"Grenada"                          , "GD"},
    {"Georgia"                          , "GE"},
    {"Guernsey"                         , "GG"},
    {"Ghana"                            , "GH"},
    {"Gibraltar"                        , "GI"},
    {"Gambia"                           , "GM"},
    {"Guinea"                           , "GN"},
    {"Equatorial Guinea"                , "GQ"},
    {"Greece"                           , "GR"},
    {"Guatemala"                        , "GT"},
    {"Guinea-Bissau"                    , "GW"},
    {"Guyana"                           , "GY"},
    {"Hong Kong"                        , "HK"},
    {"Honduras"                         , "HN"},
    {"Croatia"                          , "HR"},
    {"Haiti"                            , "HT"},
    {"Hungary"                          , "HU"},
    {"Indonesia"                        , "ID"},
    {"Ireland"                          , "IE"},
    {"Israel"                           , "IL"},
    {"Isle of Man"                      , "IM"},
    {"India"                            , "IN"},
    {"Iraq"                             , "IQ"},
    {"Iran (Islamic Republic of)"       , "IR"},
    {"Iceland"                          , "IS"},
    {"Italy"                            , "IT"},
    {"Jersey"                           , "JE"},
    {"Jamaica"                          , "JM"},
    {"Jordan"                           , "JO"},
    {"Japan"                            , "JP"},
    {"Kenya"                            , "KE"},
    {"Kyrgyzstan"                       , "KG"},
    {"Cambodia"                         , "KH"},
    {"Comoros"                          , "KM"},
    {"Saint Kitts and Nevis"            , "KN"},
    {"Korea (Democratic People's Republic of)", "KP"},
    {"Korea (Republic of)"              , "KR"},
    {"Kuwait"                           , "KW"},
    {"Cayman Islands"                   , "KY"},
    {"Kazakhstan"                       , "KZ"},
    {"Lao People's Democratic Republic" , "LA"},
    {"Lebanon"                          , "LB"},
    {"Saint Lucia"                      , "LC"},
    {"Liechtenstein"                    , "LI"},
    {"Sri Lanka"                        , "LK"},
    {"Liberia"                          , "LR"},
    {"Lesotho"                          , "LS"},
    {"Lithuania"                        , "LT"},
    {"Luxembourg"                       , "LU"},
    {"Latvia"                           , "LV"},
    {"Libya"                            , "LY"},
    {"Morocco"                          , "MA"},
    {"Monaco"                           , "MC"},
    {"Moldova (Republic of)"            , "MD"},
    {"Montenegro"                       , "ME"},
    {"Madagascar"                       , "MG"},
    {"Macedonia (the former Yugoslav Republic of)", "MK"},
    {"Mali"                             , "ML"},
    {"Myanmar"                          , "MM"},
    {"Mongolia"                         , "MN"},
    {"Macao"                            , "MO"},
    {"Mauritania"                       , "MR"},
    {"Montserrat"                       , "MS"},
    {"Malta"                            , "MT"},
    {"Mauritius"                        , "MU"},
    {"Maldives"                         , "MV"},
    {"Malawi"                           , "MW"},
    {"Mexico"                           , "MX"},
    {"Malaysia"                         , "MY"},
    {"Mozambique"                       , "MZ"},
    {"Namibia"                          , "NA"},
    {"Niger"                            , "NE"},
    {"Nigeria"                          , "NG"},
    {"Nicaragua"                        , "NI"},
    {"Netherlands"                      , "NL"},
    {"Norway"                           , "NO"},
    {"Nepal"                            , "NP"},
    {"New Zealand"                      , "NZ"},
    {"Oman"                             , "OM"},
    {"Panama"                           , "PA"},
    {"Peru"                             , "PE"},
    {"French Polynesia"                 , "PF"},
    {"Papua New Guinea"                 , "PG"},
    {"Philippines"                      , "PH"},
    {"Pakistan"                         , "PK"},
    {"Poland"                           , "PL"},
    {"Puerto Rico"                      , "PR"},
    {"Portugal"                         , "PT"},
    {"Palau"                            , "PW"},
    {"Paraguay"                         , "PY"},
    {"Qatar"                            , "QA"},
    {"Romania"                          , "RO"},
    {"Serbia"                           , "RS"},
    {"Russian Federation"               , "RU"},
    {"Rwanda"                           , "RW"},
    {"Saudi Arabia"                     , "SA"},
    {"Solomon Islands"                  , "SB"},
    {"Seychelles"                       , "SC"},
    {"Sudan"                            , "SD"},
    {"Sweden"                           , "SE"},
    {"Singapore"                        , "SG"},
    {"Saint Helena, Ascension and Tristan da Cunha", "SH"},
    {"Slovenia"                         , "SI"},
    {"Slovakia"                         , "SK"},
    {"Sierra Leone"                     , "SL"},
    {"San Marino"                       , "SM"},
    {"Senegal"                          , "SN"},
    {"Somalia"                          , "SO"},
    {"Suriname"                         , "SR"},
    {"Sao Tome and Principe"            , "ST"},
    {"El Salvador"                      , "SV"},
    {"Syrian Arab Republic"             , "SY"},
    {"Swaziland"                        , "SZ"},
    {"Turks and Caicos Islands"         , "TC"},
    {"Chad"                             , "TD"},
    {"Togo"                             , "TG"},
    {"Thailand"                         , "TH"},
    {"Tajikistan"                       , "TJ"},
    {"Timor-Leste"                      , "TL"},
    {"Turkmenistan"                     , "TM"},
    {"Tunisia"                          , "TN"},
    {"Tonga"                            , "TO"},
    {"Turkey"                           , "TR"},
    {"Trinidad and Tobago"              , "TT"},
    {"Taiwan (Province of China)"       , "TW"},
    {"Tanzania, United Republic of"     , "TZ"},
    {"Ukraine"                          , "UA"},
    {"Uganda"                           , "UG"},
    {"United States of America"         , "US"},
    {"Uruguay"                          , "UY"},
    {"Uzbekistan"                       , "UZ"},
    {"Saint Vincent and the Grenadines" , "VC"},
    {"Venezuela (Bolivarian Republic of)", "VE"},
    {"Virgin Islands (British)"         , "VG"},
    {"Viet Nam"                         , "VN"},
    {"Vanuatu"                          , "VU"},
    {"Samoa"                            , "WS"},
    {"Yemen"                            , "YE"},
    {"South Africa"                     , "ZA"},
    {"Zambia"                           , "ZM"},
    {"Zimbabwe"                         , "ZW"}
};

const QString DapServersData::findInCountriesMap(const QString& string) {
    QStringList list = string.split(".", QString::SkipEmptyParts);
    QString code;
    for (QString s: list) {
        auto it = m_countryMap.find(s);
        if (it != m_countryMap.end()) {
            code = it.value();
        }
    }
    return code.isEmpty()? code: "://country/" + code + ".png";
}

QModelIndex DapServersData::indexOf(QVariant serverData, int role)
{
    if (role == Qt::DisplayRole)
        for (int k = 0; k < m_servers.size(); k++)
        {
            QModelIndex dataIndex = index(k);
            if (data(dataIndex, Qt::DisplayRole).toString().toUpper() == serverData.toString().toUpper())
                return dataIndex;
        }
    return QModelIndex();
}

QVariant DapServersData::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_servers.size())
        return QVariant();

    switch (role)
    {
        case Qt::DisplayRole:
            return m_servers.at (index.row()).name();

        case COUTRY_FLAG_ROLE:
            {
                auto si = m_servers.at (index.row());
                if (si.name().isEmpty())
                    return QString();

                return findInCountriesMap (si.name().toUpper());
            }

        case CONNECTION_QUALITY:
            return int (m_servers.at (index.row()).connQuality());

        case PING_ROLE:
            return m_servers.at (index.row()).ping();

        case ADDRESS_ROLE:
            return m_servers.at (index.row()).address();

        default:
            break;
    }

    return QVariant();
}

Qt::ItemFlags DapServersData::flags(const QModelIndex &index) const
{
    if (!index.isValid())
            return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index);
}

bool DapServersData::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        m_servers.replace(index.row(), value.value<DapServerInfo>());
        emit dataChanged(index, index, {Qt::DisplayRole, COUTRY_FLAG_ROLE});
        return true;
    }
    return false;
}

bool DapServersData::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index)
    beginInsertRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row) {
        m_servers.insert(position, DapServerInfo());
    }
    endInsertRows();
    return true;
}

bool DapServersData::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index)
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row) {
        m_servers.removeAt(position);
    }

    endRemoveRows();
    return true;
}

QMap<int, QVariant> DapServersData::itemData(const QModelIndex &index) const
{
    QMap<int, QVariant> map;
    map[Qt::DisplayRole] = data(index, Qt::DisplayRole);
    map[COUTRY_FLAG_ROLE] = data(index, COUTRY_FLAG_ROLE);
    return map;
}
