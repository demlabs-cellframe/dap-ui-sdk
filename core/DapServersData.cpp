#include "DapDataLocal.h"

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

    emit this->serverAdded(dsi);
}

void DapServersData::addServer(DapServerLocation location, const QString& name,
                          const QString & address, quint16 port)
{
    DapServerInfo ss;
    ss.name = name;
    ss.location = location;
    ss.address = address;
    ss.port = port;
    addServer(std::move(ss));
}

void DapServersData::setCurrentServer(int a_serverIndex)
{
    if (m_currentServerIndex == a_serverIndex)
        return;
    Q_ASSERT(a_serverIndex < m_servers.count());
    m_currentServerIndex = a_serverIndex;

    if (!this->currentServer())
        return;

    emit currentServerNameChanged(this->currentServerName());
}

void DapServersData::setCurrentServerNotSignal(int a_serverIndex)
{
    if (m_currentServerIndex == a_serverIndex)
        return;
    Q_ASSERT(a_serverIndex < m_servers.count());
    m_currentServerIndex = a_serverIndex;

    emit currentServerNameChangedNotRequestChangingServer(this->currentServerName());
}

void DapServersData::setCurrentServer(const DapServerInfo *a_server)
{
    qDebug() << "Selecting the current server: " << (a_server ? a_server->name : "null");

    setCurrentServer(m_servers.lastIndexOf(*a_server));
}

void DapServersData::setCurrentServerFromService(const DapServerInfo *a_server)
{
    qDebug() << "Selecting the current server received from the service: " << (a_server ? a_server->name : "null");

    int index = -1;
    index = m_servers.lastIndexOf(*a_server);
    if (index == -1) {
        addServer(*a_server);
        index = m_servers.lastIndexOf(*a_server);
    }
    setCurrentServerNotSignal(index);
}

/// Set server name.
/// @param server Server name.
void DapServersData::setCurrentServer(const QString &a_serverName)
{
    if (currentServerName() == a_serverName)
        return;

    for (int i = 0; i < this->serversCount(); i++)
    {
        if (m_servers.at(i).name == a_serverName)
        {
            setCurrentServer(i);
            return;
        }
        this->setCurrentServer(-1);
    }

    qFatal("There is no server with name %s", qPrintable(a_serverName));
}

void DapServersData::saveDatas() const
{

}

void DapServersData::loadDatas()
{

}

const DapServerInfo *DapServersData::currentServer() const
{
    if (m_currentServerIndex < 0 || m_currentServerIndex >= this->serversCount())
        return nullptr;

    return &m_servers.at(this->m_currentServerIndex);
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
    const DapServerInfo* currentServer = this->currentServer();
    return currentServer ? currentServer->name : "";
}

QString DapServersData::currentServerAdress() const
{
    const DapServerInfo* currentServer = this->currentServer();
    return currentServer ? currentServer->address : "";
}

bool DapServersData::currentServerIsAuto() const
{
    return this->currentServer()->isAuto();
}

int DapServersData::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_servers.count();
}

const QStringList DapServersData::m_countries = {
    "", //UNKNOWN = 0,
    "://country/GB.png", //ENGLAND,
    "://country/FR.png", //FRANCE,
    "://country/DE.png", //GERMANY,
    "://country/US.png", //USA,
    "://country/NL.png", //NETHERLANDS,
    "://country/RU.png", //RUSSIA,
    "://country/UA.png", //UKRAINE,
    "://country/NL.png", //Netherlands,
    "://country/SG.png", //Singapore,
    "://country/DE.png", //Germany,
};

QMap<QString, QString> DapServersData::m_countryMap = {
    {"ANDORRA", "AD"},
    {"UNITED ARAB EMIRATES", "AE"},
    {"AFGHANISTAN", "AF"},
    {"ANTIGUA AND BARBUDA", "AG"},
    {"ANGUILLA", "AI"},
    {"ALBANIA", "AL"},
    {"ARMENIA", "AM"},
    {"NEDERLANDSE ANTILLEN", "AN"},
    {"ANGOLA", "AO"},
    {"ARGENTINA", "AR"},
    {"AUSTRIA", "AT"},
    {"AUSTRALIA", "AU"},
    {"ARUBA", "AW"},
    {"ALAND ISLANDS", "AX"},
    {"AZERBAIJAN", "AZ"},
    {"BOSNIA AND HERZEGOVINA", "BA"},
    {"BARBADOS", "BB"},
    {"BANGLADESH", "BD"},
    {"BELGIUM", "BE"},
    {"BURKINA FASO", "BF"},
    {"BULGARIA", "BG"},
    {"BAHRAIN", "BH"},
    {"BURUNDI", "BI"},
    {"BENIN", "BJ"},
    {"BERMUDA", "BM"},
    {"BRUNEI DARUSSALAM", "BN"},
    {"BOLIVIA", "BO"},
    {"BRAZIL", "BR"},
    {"BAHAMAS", "BS"},
    {"BRUTAN", "BT"},
    {"BOTSWANA", "BW"},
    {"BELARUS", "BY"},
    {"BELIZE", "BZ"},
    {"CANADA", "CA"},
    {"CONGO", "CD"},
    {"CENTRAL AFRICAN REPUBLIC", "CF"},
    {"CONGO", "CG"},
    {"SWITZERLAND", "CH"},
    {"COTE D'IVOIRE", "CI"},
    {"CHILE", "CL"},
    {"CAMEROON", "CM"},
    {"CHINA", "CN"},
    {"COLOMBIA", "CO"},
    {"COSTA RICA", "CR"},
    {"CUBA", "CU"},
    {"CAPE VERDE", "CV"},
    {"CYPRUS", "CY"},
    {"CZECH REPUBLIC", "CZ"},
    {"GERMANY", "DE"},
    {"DJIBOUTI", "DJ"},
    {"DENMARK", "DK"},
    {"DOMINICA", "DM"},
    {"DOMINICAN REPUBLIC", "DO"},
    {"ALGERIA", "DZ"},
    {"ECUADOR", "EC"},
    {"ESTONIA", "EE"},
    {"EGIPT", "EG"},
    {"ERITREA", "ER"},
    {"SPAIN", "ES"},
    {"ETHIOPIA", "ET"},
    {"EUROPE", "EU"},
    {"FINLAND", "FI"},
    {"FIJI", "FJ"},
    {"FALKLAND ISLANDS", "FK"},
    {"MALVINAS", "FK"},
    {"MICRONESIA", "FM"},
    {"FRANCE", "FR"},
    {"GABON", "GA"},
    {"UNITED KINGDOM", "GB"},
    {"ENGLAND", "GB"},
    {"GRENADA", "GD"},
    {"GEORGIA", "GE"},
    {"GUERNSEY", "GG"},
    {"GHANA", "GH"},
    {"GIBRALTAR", "GI"},
    {"GAMBIA", "GM"},
    {"GUINEA", "GN"},
    {"EQUATORIAL GUINEA", "GQ"},
    {"GREECE", "GR"},
    {"GUATEMALA", "GT"},
    {"GUINEA-BISSAU", "GW"},
    {"GUYANA", "GY"},
    {"HONG KONG", "HK"},
    {"HONDURAS", "HN"},
    {"CROATIA", "HR"},
    {"HAITI", "HT"},
    {"HUNGARY", "HU"},
    {"INDONESIA", "ID"},
    {"IRELAND", "IE"},
    {"ISRAEL", "IL"},
    {"ISLE OF MAN", "IM"},
    {"INDIA", "IN"},
    {"IRAQ", "IQ"},
    {"IRAN", "IR"},
    {"ICELAND", "IS"},
    {"ITALY", "IT"},
    {"JERSAY", "JE"},
    {"JAMAIKA", "JM"},
    {"JORDAN", "JO"},
    {"JAPAN", "JP"},
    {"KENIA", "KE"},
    {"KYRGYZSTAN", "KG"},
    {"CAMBODIA", "KH"},
    {"COMOROS", "KM"},
    {"SAINT KITTS AND NEVIS", "KN"},
    {"KOREA", "KP"},
    {"REPUBLIC OF KOREA", "KR"},
    {"KUWAIT", "KW"},
    {"CAYMAN ISLANDS", "KY"},
    {"KAZAKHSTAN", "KZ"},
    {"LAO", "LA"},
    {"LEBANON", "LB"},
    {"SAINT LUSIA", "LC"},
    {"LEICHTENSTEIN", "LI"},
    {"SRI LANKA", "LK"},
    {"LIBERIA", "LR"},
    {"LESATHO", "LS"},
    {"LITHUANIA", "LT"},
    {"LUXEMBOURG", "LU"},
    {"LATVIA", "LV"},
    {"LIBYAN ARAB JAMAHIRIYA", "LY"},
    {"MOROCCO", "MA"},
    {"MONACO", "MC"},
    {"MOLDOVA", "MD"},
    {"MONTENEGRO", "ME"},
    {"MADAGASKAR", "MG"},
    {"MACEDONIA", "MK"},
    {"MALI", "ML"},
    {"BURMA", "MM"},
    {"MONGOLIA", "MN"},
    {"MACAO", "MO"},
    {"MAURITANIA", "MR"},
    {"MONTSERRAT", "MS"},
    {"MALTA", "MT"},
    {"MAURITIUS", "MU"},
    {"MALDIVES", "MV"},
    {"MALAWI", "MW"},
    {"MEXICO", "MX"},
    {"MALAYSIA", "MY"},
    {"MOZAMBIQUE", "MZ"},
    {"NAMIBIA", "NA"},
    {"NIGER", "NE"},
    {"NIGERIA", "NG"},
    {"NICARAGUA", "NI"},
    {"NETHERLANDS", "NL"},
    {"NORWAY", "NO"},
    {"NEPAL", "NP"},
    {"NEW ZEALAND", "NZ"},
    {"OMAN", "OM"},
    {"PANAMA", "PA"},
    {"PERU", "PE"},
    {"FRENCH POLYNESIA", "PF"},
    {"PAPUA NEW GUINEA", "PG"},
    {"PHILIPPINES", "PH"},
    {"PAKISTAN", "PK"},
    {"POLAND", "PL"},
    {"PUERTO RICO", "PR"},
    {"PORTUGAL", "PT"},
    {"PALAU", "PW"},
    {"PARAGUAY", "PY"},
    {"QATAR", "QA"},
    {"ROMANIA", "RO"},
    {"SERBIA", "RS"},
    {"RUSSIA", "RU"},
    {"RUSSIAN FEDERATION", "RU"},
    {"RWANDA", "RW"},
    {"SAUDI ARABIA", "SA"},
    {"SOLOMON ISLANDS", "SB"},
    {"SEYCHELLES", "SC"},
    {"SUDAN", "SD"},
    {"SWEDEN", "SE"},
    {"SINGAPUR", "SG"},
    {"SAINT HELENA", "SH"},
    {"SLOVENIA", "SI"},
    {"SLOVAKIA", "SK"},
    {"SIERRA LIONE", "SL"},
    {"SAN MARIO", "SM"},
    {"SENEGAL", "SN"},
    {"SOMALIA", "SO"},
    {"SURINAME", "SR"},
    {"SAO TOME AND PRINCIPE", "ST"},
    {"EL SALVADOR", "SV"},
    {"SALVADOR", "SV"},
    {"SYRIAN ARAB REPUBLIC", "SY"},
    {"SWAZILAND", "SZ"},
    {"TURKS AND CAICOS ISLANDS", "TC"},
    {"CHAD", "TD"},
    {"TOGO", "TG"},
    {"THAILAND", "TH"},
    {"TAJIKISTAN", "TJ"},
    {"TIMOR-LESTE", "TL"},
    {"TURKMENISTAN", "TM"},
    {"TUNISIA", "TN"},
    {"TONGA", "TO"},
    {"TURKEY", "TR"},
    {"TRINIDAD AND TOBAGO", "TT"},
    {"TRINIDAD", "TT"},
    {"TAIWAN", "TW"},
    {"TANZANIA", "TZ"},
    {"UKRAINE", "UA"},
    {"UGANDA", "UG"},
    {"UNITED STATES", "US"},
    {"USA", "US"},
    {"URUGUAY", "UY"},
    {"UZBEKISTAN", "UZ"},
    {"SAINT VINCENT AND THE GRINADINES", "VC"},
    {"VENEZUELA", "VE"},
    {"VIRGIN ISLAND", "VG"},
    {"VIETNAM", "VN"},
    {"VANUATU", "VU"},
    {"SAMOA", "WS"},
    {"YEMEN", "YE"},
    {"SOUTH AFRICA", "ZA"},
    {"ZAMBIA", "ZM"},
    {"ZIMBABWE", "ZW"},
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

QVariant DapServersData::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= m_servers.size())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        return m_servers.at(index.row()).name;
    case COUTRY_FLAG_ROLE: {
        auto si = m_servers.at(index.row());
        if (si.name.isEmpty())
            return QString();

        return findInCountriesMap(si.name.toUpper());
    }
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
